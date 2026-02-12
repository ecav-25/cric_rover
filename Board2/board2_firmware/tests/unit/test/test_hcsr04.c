/**
 * @file test_hcsr04.c
 * @brief Unit tests per il driver del sensore ultrasuoni HC-SR04.
 * * @author Gruppo 2
 */

#include "unity.h"
#include "mock_stm32g4xx_hal.h"
#include "hcsr04.h"
#include <string.h>

/** @brief Istanza del sensore utilizzata per i test. */
static hcsr04_t sensor;

/** @brief Configurazione mockata del sensore. */
static hcsr04_cfg_t config;

/** @brief Handle del timer mockato. */
static TIM_HandleTypeDef htim_echo;

/** @brief Porta GPIO mockata per il segnale di Trigger. */
static GPIO_TypeDef gpio_trig;

/**
 * @brief Stub per la funzione di delay in microsecondi.
 * @param us Tempo di attesa richiesto.
 * @return Sempre 0 (Success).
 */
uint8_t fake_delay_us(uint32_t us) { (void)us; return 0; }

/**
 * @brief Funzione di setup eseguita prima di ogni test.
 * Inizializza le strutture dati e configura CMock per ignorare le chiamate
 * ripetitive alle funzioni HAL di inizializzazione.
 */
void setUp(void) {
    memset(&sensor, 0, sizeof(hcsr04_t));
    memset(&config, 0, sizeof(hcsr04_cfg_t));

    config.htim = &htim_echo;
    config.channel = TIM_CHANNEL_1;
    config.trig_port = &gpio_trig;
    config.trig_pin = 10;
    config.timer_hz = 1000000;
    config.delay_us = fake_delay_us;

    /* Ignoriamo l'avvio dell'interrupt timer per focalizzarci sulla logica di calcolo */
    HAL_TIM_IC_Start_IT_IgnoreAndReturn(HAL_OK);
}

/* --- TEST CALCOLO DISTANZA --- */

/**
 * @test Verifica una misura completa con successo (10 cm).
 * @details Simula l'invio del trigger e l'arrivo dei due interrupt (Rising e Falling)
 * con una differenza temporale di 580 microsecondi.
 */
void test_HCSR04_CompleteMeasurement_10cm(void) {
    HCSR04_Init(&sensor, &config);

    /* Aspettativa: attivazione e disattivazione pin trigger */
    HAL_GPIO_WritePin_Expect(&gpio_trig, 10, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(&gpio_trig, 10, GPIO_PIN_RESET);
    HCSR04_Start(&sensor);

    /* Simulazione fronte di salita (Rising Edge) */
    htim_echo.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    HAL_TIM_ReadCapturedValue_ExpectAndReturn(&htim_echo, TIM_CHANNEL_1, 1000);
    HCSR04_IC_Callback(&htim_echo);

    /* Simulazione fronte di discesa (Falling Edge) dopo 580us */
    HAL_TIM_ReadCapturedValue_ExpectAndReturn(&htim_echo, TIM_CHANNEL_1, 1580);
    HCSR04_IC_Callback(&htim_echo);

    uint16_t distance = 0;
    TIM_TypeDef fake_regs = { .ARR = 0xFFFF };
    htim_echo.Instance = &fake_regs;

    hcsr04_status_t status = HCSR04_GetDistanceCm(&sensor, &distance);

    TEST_ASSERT_EQUAL(HCSR04_OK, status);
    TEST_ASSERT_EQUAL(10, distance); /* (580 + 29) / 58 = 10 cm */
}

/* --- TEST OVERFLOW TIMER --- */

/**
 * @test Verifica il corretto calcolo della distanza in caso di overflow del timer.
 * @details Simula una cattura dove il valore di falling (100) è minore di quello di rising (900),
 * verificando che la logica di sottrazione circolare basata su ARR funzioni.
 */
void test_HCSR04_TimerOverflow_Calculation(void) {
    HCSR04_Init(&sensor, &config);
    sensor.last_status = HCSR04_OK;
    sensor.state = HCSR04_IDLE;

    TIM_TypeDef fake_regs = { .ARR = 1000 };
    htim_echo.Instance = &fake_regs;

    sensor.t_rise = 900;
    sensor.t_fall = 100; /* Ticks effettivi: (1000+1)-900 + 100 = 201 */

    uint16_t dist;
    HCSR04_GetDistanceCm(&sensor, &dist);

    /* us = 201. cm = (201+29)/58 = 3.96 -> 3 cm */
    TEST_ASSERT_EQUAL(3, dist);
}

/* --- TEST STATO BUSY --- */

/**
 * @test Verifica che non sia possibile avviare una nuova misura se una è già in corso.
 * @pre Lo stato del sensore è impostato su HCSR04_WAIT_RISE.
 */
void test_HCSR04_Start_Busy(void) {
    HCSR04_Init(&sensor, &config);
    sensor.state = HCSR04_WAIT_RISE;

    hcsr04_status_t status = HCSR04_Start(&sensor);
    TEST_ASSERT_EQUAL(HCSR04_ERR_BUSY, status);
}

/* --- TEST TIMEOUT --- */

/**
 * @test Verifica la gestione del timeout se i fronti d'onda non vengono rilevati.
 * @details Se viene richiesta la distanza mentre il sensore è ancora in attesa dell'eco,
 * il driver deve restituire HCSR04_ERR_TIMEOUT e resettarsi allo stato IDLE.
 */
void test_HCSR04_GetDistance_TimeoutIfStillWaiting(void) {
    HCSR04_Init(&sensor, &config);

    HAL_GPIO_WritePin_Expect(&gpio_trig, config.trig_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(&gpio_trig, config.trig_pin, GPIO_PIN_RESET);

    HCSR04_Start(&sensor);

    uint16_t dist;
    hcsr04_status_t status = HCSR04_GetDistanceCm(&sensor, &dist);

    TEST_ASSERT_EQUAL(HCSR04_ERR_TIMEOUT, status);
    TEST_ASSERT_EQUAL(HCSR04_MAX_DISTANCE, dist);
    TEST_ASSERT_EQUAL(HCSR04_IDLE, sensor.state);
}

/**
 * @test Verifica la gestione di un ostacolo fuori portata (distanza > 400cm).
 */
void test_HCSR04_OutOfRange_ReturnsMaxDistance(void) {
    HCSR04_Init(&sensor, &config);

    // Simuliamo un eco molto lungo (es. 30000 us = 30ms)
    sensor.t_rise = 0;
    sensor.t_fall = 30000;
    sensor.last_status = HCSR04_OK;
    sensor.state = HCSR04_IDLE;

    uint16_t dist;
    hcsr04_status_t status = HCSR04_GetDistanceCm(&sensor, &dist);

    TEST_ASSERT_EQUAL(HCSR04_OK, status);
    TEST_ASSERT_EQUAL(HCSR04_MAX_DISTANCE, dist); // Deve restituire 400
}

/**
 * @test Verifica che la callback ignori chiamate da canali non configurati.
 */
void test_HCSR04_Callback_WrongChannel_Ignored(void) {
    HCSR04_Init(&sensor, &config);
    sensor.state = HCSR04_WAIT_RISE;

    // Simuliamo una chiamata da un canale diverso (es. Canale 2 invece di 1)
    htim_echo.Channel = HAL_TIM_ACTIVE_CHANNEL_2;

    // Non carichiamo Expect per ReadCapturedValue: se viene chiamata, CMock darà errore
    HCSR04_IC_Callback(&htim_echo);

    // Lo stato deve essere rimasto WAIT_RISE
    TEST_ASSERT_EQUAL(HCSR04_WAIT_RISE, sensor.state);
}

/**
 * @test Verifica l'arrotondamento matematico della distanza.
 */
void test_HCSR04_Distance_Rounding(void) {
    HCSR04_Init(&sensor, &config);
    sensor.last_status = HCSR04_OK;
    sensor.state = HCSR04_IDLE;

    uint16_t dist;

    // Caso A: 144us -> 144/58 = 2.48cm -> deve arrotondare a 2cm
    sensor.t_rise = 0; sensor.t_fall = 144;
    HCSR04_GetDistanceCm(&sensor, &dist);
    TEST_ASSERT_EQUAL(2, dist);

    // Caso B: 145us -> 145/58 = 2.50cm -> deve arrotondare a 3cm
    sensor.t_rise = 0; sensor.t_fall = 145;
    HCSR04_GetDistanceCm(&sensor, &dist);
    TEST_ASSERT_EQUAL(3, dist);
}
