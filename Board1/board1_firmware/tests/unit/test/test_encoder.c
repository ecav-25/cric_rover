/**
 * @file test_encoder.c
 * @author Gruppo 2
 * @brief Unit Tests per il modulo encoder.
 *
 * @details I test verificano l'inizializzazione del timer in modalità encoder,
 * la gestione del calcolo RPM in senso orario e antiorario, e la corretta
 * gestione degli overflow del contatore (ARR).
 */

#include "unity.h"
#include <string.h>

/* Mock dell'HAL STM32 */
#include "mock_stm32g4xx_hal.h"

/* Modulo da testare */
#include "encoder.h"

/** @name Oggetti di supporto
 * @{ */
static EncoderHandle enc;
static TIM_HandleTypeDef m_htim;
static TIM_TypeDef m_instance;
static Encoder_Calibration_t m_calib;
/** @} */

/**
 * @brief Setup eseguito prima di ogni test.
 */
void setUp(void) {
    memset(&enc, 0, sizeof(EncoderHandle));
    memset(&m_htim, 0, sizeof(TIM_HandleTypeDef));
    memset(&m_instance, 0, sizeof(TIM_TypeDef));

    m_htim.Instance = &m_instance;

    /* Calibrazione standard: 100 PPR, Gear Ratio 1:1 */
    m_calib.pulses_per_rev = 100;
    m_calib.gear_ratio = 1.0f;
}

/* =================================================================---------
    TEST INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica l'inizializzazione e l'avvio del timer encoder.
 */
void test_encoder_init_success(void) {
    m_instance.CNT = 0;
    m_instance.ARR = 1000;
    float period_ms = 100.0f;

    HAL_TIM_Encoder_Start_ExpectAndReturn(&m_htim, TIM_CHANNEL_ALL, HAL_OK);

    Encoder_Status_t status = encoder_init(&enc, &m_htim, &m_calib, period_ms);

    TEST_ASSERT_EQUAL(ENCODER_OK, status);
    TEST_ASSERT_EQUAL_PTR(&m_htim, enc.htim);
    TEST_ASSERT_EQUAL(0.1f, enc.reading_period); // 100ms -> 0.1s
    TEST_ASSERT_EQUAL(1000, enc.arr);
}

/**
 * @test Verifica la protezione contro i puntatori NULL nell'init.
 */
void test_encoder_init_fails_on_null_ptr(void) {
    TEST_ASSERT_EQUAL(ENCODER_ERR, encoder_init(NULL, &m_htim, &m_calib, 100.0f));
}

/* =================================================================---------
    TEST CALCOLO VELOCITÀ (RPM)
   ========================================================================= */

/**
 * @test Calcolo RPM standard in avanti (conteggio crescente).
 * @details Formula: (60 * diff) / (PPR * Period * Gear)
 * Con PPR=100, Period=0.1s, Gear=1.0, Diff=10 -> RPM = (600 / 10) = 60 RPM.
 * Nota: Il driver inverte il segno per il conteggio crescente.
 */
void test_encoder_readRPM_forward_counting(void) {
    /* Setup stato iniziale */
    enc.htim = &m_htim;
    enc.prev_count = 100;
    enc.arr = 1000;
    enc.reading_period = 0.1f; // 100ms
    enc.calib = m_calib;

    /* Simulazione hardware: Conteggio sale a 110, direzione UP (CR1.DIR = 0) */
    m_instance.CNT = 110;
    m_instance.CR1 &= ~(0x10U); // Reset DIR bit (counting up)
    m_instance.SMCR = 0;       // Modalità x1 (non divide per 2)

    Encoder_Status_t status = encoder_readRPM(&enc);

    TEST_ASSERT_EQUAL(ENCODER_OK, status);
    /* (60 * 10) / (100 * 0.1 * 1.0) = 60. Il driver assegna segno negativo al conteggio UP */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -60.0f, enc.velocity);
    TEST_ASSERT_EQUAL(110, enc.prev_count);
}

/**
 * @test Calcolo RPM in retromarcia (conteggio decrescente).
 */
void test_encoder_readRPM_backward_counting(void) {
    enc.htim = &m_htim;
    enc.prev_count = 100;
    enc.reading_period = 0.1f;
    enc.calib = m_calib;

    /* Simulazione hardware: Conteggio scende a 90, direzione DOWN (CR1.DIR = 1) */
    m_instance.CNT = 90;
    m_instance.CR1 |= 0x10U; // Set DIR bit (counting down)

    encoder_readRPM(&enc);

    /* (60 * 10) / (100 * 0.1 * 1.0) = 60. Segno positivo per conteggio DOWN */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 60.0f, enc.velocity);
}

/* =================================================================---------
    TEST GESTIONE OVERFLOW / UNDERFLOW
   ========================================================================= */

/**
 * @test Verifica la gestione dell'overflow del contatore (da ARR a 0).
 */
void test_encoder_readRPM_overflow_up(void) {
    enc.htim = &m_htim;
    enc.prev_count = 990;
    enc.arr = 1000;
    enc.reading_period = 0.1f;
    enc.calib = m_calib;

    /* Simulazione: Da 990 scavalca ARR(1000) e arriva a 10 */
    m_instance.CNT = 10;
    m_instance.CR1 &= ~(0x10U); // Counting UP

    encoder_readRPM(&enc);

    /* diff = (1000 - 990) + 10 = 20 impulsi */
    /* RPM = (60 * 20) / (100 * 0.1) = 120 RPM (Segno - per UP) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -120.0f, enc.velocity);
}

/**
 * @test Verifica la gestione dell'underflow del contatore (da 0 a ARR).
 */
void test_encoder_readRPM_underflow_down(void) {
    enc.htim = &m_htim;
    enc.prev_count = 10;
    enc.arr = 1000;
    enc.reading_period = 0.1f;
    enc.calib = m_calib;

    /* Simulazione: Da 10 torna indietro, scavalca lo 0 e arriva a 980 */
    m_instance.CNT = 980;
    m_instance.CR1 |= 0x10U; // Counting DOWN

    encoder_readRPM(&enc);

    /* diff = (1000 - 980) + 10 = 30 impulsi */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, enc.velocity);
}

/* =================================================================---------
    TEST CONFIGURAZIONI SPECIALI
   ========================================================================= */

/**
 * @test Verifica la divisione per 2 in modalità Encoder x4 (entrambi i canali).
 */
void test_encoder_readRPM_quadrature_x4(void) {
    enc.htim = &m_htim;
    enc.prev_count = 0;
    enc.reading_period = 1.0f; // 1s per semplicità
    enc.calib = m_calib;

    m_instance.CNT = 100;
    m_instance.CR1 &= ~(0x10U); // UP
    /* Imposta SMCR bits 0-2 a 011 (3) per modalità Encoder x4 */
    m_instance.SMCR |= 0x03U;

    encoder_readRPM(&enc);

    /* (60 * 100) / (100 * 1.0) = 60 RPM.
       Dato che SMCR indica x4, il driver divide per 2 -> 30 RPM */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -30.0f, enc.velocity);
}
