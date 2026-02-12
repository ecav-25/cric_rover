/**
 * @file test_batt.c
 * @author Gruppo 2
 * @brief Unit Tests completi per il modulo batteria (batt.c).
 *
 * @details Il modulo valida la logica di inizializzazione, le letture singole,
 * la gestione degli errori hardware dell'ADC e la precisione dei calcoli.
 */

#include "unity.h"
#include <string.h>
#include "mock_stm32g4xx_hal.h"
#include "batt.h"

/**
 * @name Strutture di supporto
 * @{
 */
static batt_t b;               /**< Istanza del driver sotto test */
static ADC_HandleTypeDef m_hadc;  /**< Mock handle ADC */
static ADC_ChannelConfTypeDef m_cfg; /**< Mock configurazione canale */
/** @} */

/**
 * @brief Setup eseguito prima di ogni test.
 */
void setUp(void) {
    memset(&b, 0, sizeof(batt_t));
    memset(&m_hadc, 0, sizeof(ADC_HandleTypeDef));
    memset(&m_cfg, 0, sizeof(ADC_ChannelConfTypeDef));
    /* Indirizzo fittizio per superare i controlli hardware nel driver */
    m_hadc.Instance = (ADC_TypeDef *)0x40001000;
}
/* =================================================================---------
    TEST INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica l'inizializzazione corretta e la calibrazione ADC.
 */
void test_batt_init_success(void) {
    HAL_ADCEx_Calibration_Start_ExpectAndReturn(&m_hadc, m_cfg.SingleDiff, HAL_OK);

    Batt_Status_t status = batt_init(&b, &m_hadc, &m_cfg, 10);

    TEST_ASSERT_EQUAL(BATT_OK, status);
    TEST_ASSERT_EQUAL_PTR(&m_hadc, b.hadc);
}

/**
 * @test Verifica il fallimento dell'inizializzazione se la calibrazione fallisce.
 */
void test_batt_init_fails_if_calibration_fails(void) {
    HAL_ADCEx_Calibration_Start_ExpectAndReturn(&m_hadc, m_cfg.SingleDiff, HAL_ERROR);

    Batt_Status_t status = batt_init(&b, &m_hadc, &m_cfg, 10);

    TEST_ASSERT_EQUAL(BATT_ERR_COMM, status);
}

/* =================================================================---------
    TEST LETTURA E CALCOLO (PRECISIONE E LIMITI)
   ========================================================================= */

/**
 * @test Verifica la conversione della tensione in condizioni nominali.
 * @details Simula una lettura di 2048 (metà scala).
 * Formula: (2048 * 3.3 / 4095) * ((100k + 27k) / 27k)
 */
void test_batt_get_voltage_once_nominal(void) {
    float volt_out = 0.0f;
    b.hadc = &m_hadc;
    b.timeout_ms = 10;

    HAL_ADC_ConfigChannel_ExpectAndReturn(&m_hadc, &b.channel_cfg, HAL_OK);
    HAL_ADC_Start_ExpectAndReturn(&m_hadc, HAL_OK);
    HAL_ADC_PollForConversion_ExpectAndReturn(&m_hadc, 10, HAL_OK);
    HAL_ADC_GetValue_ExpectAndReturn(&m_hadc, 2048);
    HAL_ADC_Stop_ExpectAndReturn(&m_hadc, HAL_OK);

    TEST_ASSERT_EQUAL(BATT_OK, batt_get_voltage_once(&b, &volt_out));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 7.76f, volt_out);
}

/**
 * @test Verifica la corretta gestione del valore minimo dell'ADC (0V).
 */
void test_batt_get_voltage_at_zero(void) {
    float volt = -1.0f;
    b.hadc = &m_hadc;

    HAL_ADC_ConfigChannel_IgnoreAndReturn(HAL_OK);
    HAL_ADC_Start_IgnoreAndReturn(HAL_OK);
    HAL_ADC_PollForConversion_IgnoreAndReturn(HAL_OK);
    HAL_ADC_GetValue_ExpectAndReturn(&m_hadc, 0);
    HAL_ADC_Stop_IgnoreAndReturn(HAL_OK);

    batt_get_voltage_once(&b, &volt);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, volt);
}

/**
 * @test Verifica la corretta gestione del valore massimo dell'ADC.
 * @details Simula 4095 raw, corrispondenti alla massima tensione misurabile dal partitore.
 */
void test_batt_get_voltage_at_max(void) {
    float volt = 0.0f;
    b.hadc = &m_hadc;

    HAL_ADC_ConfigChannel_IgnoreAndReturn(HAL_OK);
    HAL_ADC_Start_IgnoreAndReturn(HAL_OK);
    HAL_ADC_PollForConversion_IgnoreAndReturn(HAL_OK);
    HAL_ADC_GetValue_ExpectAndReturn(&m_hadc, 4095);
    HAL_ADC_Stop_IgnoreAndReturn(HAL_OK);

    batt_get_voltage_once(&b, &volt);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 15.52f, volt);
}

/* =================================================================---------
    TEST GESTIONE ERRORI HARDWARE
   ========================================================================= */

/**
 * @test Verifica che il driver gestisca il fallimento dell'avvio ADC (HAL_ADC_Start).
 */
void test_batt_read_fails_if_start_fails(void) {
    uint32_t raw;
    b.hadc = &m_hadc;

    HAL_ADC_ConfigChannel_ExpectAndReturn(&m_hadc, &b.channel_cfg, HAL_OK);
    HAL_ADC_Start_ExpectAndReturn(&m_hadc, HAL_ERROR);

    TEST_ASSERT_EQUAL(BATT_ERR, batt_read_raw_once(&b, &raw));
}

/**
 * @test Verifica la gestione del timeout durante il polling della conversione.
 * @details Verifica anche che HAL_ADC_Stop venga chiamato correttamente dopo l'errore.
 */
void test_batt_read_fails_on_polling_timeout(void) {
    uint32_t raw;
    b.hadc = &m_hadc;
    b.timeout_ms = 10;

    HAL_ADC_ConfigChannel_IgnoreAndReturn(HAL_OK);
    HAL_ADC_Start_IgnoreAndReturn(HAL_OK);
    HAL_ADC_PollForConversion_ExpectAndReturn(&m_hadc, 10, HAL_TIMEOUT);
    HAL_ADC_Stop_ExpectAndReturn(&m_hadc, HAL_OK);

    TEST_ASSERT_EQUAL(BATT_ERR, batt_read_raw_once(&b, &raw));
}

/* =================================================================---------
    TEST ROBUSTEZZA (PUNTATORI NULLI)
   ========================================================================= */

/**
 * @test Verifica la protezione contro i puntatori NULL su tutte le API pubbliche.
 */
void test_batt_api_should_handle_null_pointers(void) {
    uint32_t raw;
    float volt;

    TEST_ASSERT_EQUAL(BATT_ERR, batt_init(NULL, &m_hadc, &m_cfg, 10));
    TEST_ASSERT_EQUAL(BATT_ERR, batt_read_raw_once(NULL, &raw));
    TEST_ASSERT_EQUAL(BATT_ERR, batt_get_voltage_once(&b, NULL));
}
