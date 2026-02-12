/**
 * @file test_temp.c
 * @author Gruppo 2
 * @brief Suite di test unitari per il driver del sensore di temperatura interno.
 */

#include "unity.h"
#include <string.h>
#include "mock_stm32g4xx_hal.h"
#include "temp.h"

/** @name Risorse Statiche di Test
 * @{ */
static temp_t temp_sensor;           /**< Istanza del driver sotto test */
static ADC_HandleTypeDef hadc_test;   /**< Mock handle dell'ADC */
static ADC_ChannelConfTypeDef config_test; /**< Mock configurazione canale */
/** @} */

/**
 * @brief Inizializzazione prima di ogni singolo test.
 * Azzera le strutture e prepara l'handle dell'ADC.
 */
void setUp(void) {
    memset(&temp_sensor, 0, sizeof(temp_t));
    memset(&hadc_test, 0, sizeof(ADC_HandleTypeDef));
    memset(&config_test, 0, sizeof(ADC_ChannelConfTypeDef));

    config_test.SingleDiff = 0;

    /* Configurazione minima per rendere l'oggetto valido nei test di lettura */
    temp_sensor.hadc = &hadc_test;
    temp_sensor.timeout_ms = 10;
    temp_sensor.channel_cfg = config_test;
}

/**
 * @brief Helper Function per simulare la sequenza HAL completa di una lettura.
 * * @details Riproduce l'esatto ordine di chiamate della funzione interna temp_read_once_():
 * 1. ConfigChannel
 * 2. Start
 * 3. PollForConversion
 * 4. GetValue
 * 5. Stop
 * * @param returned_raw Il valore che HAL_ADC_GetValue dovrà restituire.
 */
static void expect_adc_read_sequence(uint32_t returned_raw) {
    HAL_ADC_ConfigChannel_ExpectAndReturn(&hadc_test, &temp_sensor.channel_cfg, HAL_OK);
    HAL_ADC_Start_ExpectAndReturn(&hadc_test, HAL_OK);
    HAL_ADC_PollForConversion_ExpectAndReturn(&hadc_test, temp_sensor.timeout_ms, HAL_OK);
    HAL_ADC_GetValue_ExpectAndReturn(&hadc_test, returned_raw);
    HAL_ADC_Stop_ExpectAndReturn(&hadc_test, HAL_OK);
}

/* =================================================================---------
    TEST DI INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica che l'inizializzazione esegua correttamente la calibrazione ADC.
 */
void test_temp_init_should_calibrate_and_setup(void) {
    HAL_ADCEx_Calibration_Start_ExpectAndReturn(&hadc_test, config_test.SingleDiff, HAL_OK);

    Temp_Status_t status = temp_init(&temp_sensor, &hadc_test, &config_test, 100);

    TEST_ASSERT_EQUAL(TEMP_OK, status);
    TEST_ASSERT_EQUAL_PTR(&hadc_test, temp_sensor.hadc);
    TEST_ASSERT_EQUAL(100, temp_sensor.timeout_ms);
}

/* =================================================================---------
    TEST DI CONVERSIONE E LOGICA
   ========================================================================= */

/**
 * @test Verifica la conversione a 25.0°C.
 * @details
 * Calcolo atteso:
 * V_sense = 760mV (Parametro V25).
 * Raw = (760 * 4095) / 3300 = 943.45 -> 943.
 */
void test_temp_get_celsius_at_25_degrees(void) {
    uint32_t raw_25c = (uint32_t)((TEMP_V25_MV * TEMP_ADC_MAX_CNT) / TEMP_ADC_VREF_MV);
    float temp_out = 0.0f;

    expect_adc_read_sequence(raw_25c);

    Temp_Status_t status = temp_get_celsius_once(&temp_sensor, &temp_out);

    TEST_ASSERT_EQUAL(TEMP_OK, status);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 25.0f, temp_out);
}



/**
 * @test Verifica la conversione a 45.0°C.
 * @details
 * Delta T = 20°C. Delta V = 20 * 2.5mV = 50mV.
 * V_sense = 760 + 50 = 810mV.
 * Raw = (810 * 4095) / 3300 = 1005.
 */
void test_temp_get_celsius_at_high_temp(void) {
    float target_temp = 45.0f;
    float v_expected = TEMP_V25_MV + ((target_temp - 25.0f) * TEMP_SLOPE_MV_PER_C);
    uint32_t raw_45c = (uint32_t)((v_expected * TEMP_ADC_MAX_CNT) / TEMP_ADC_VREF_MV);
    float temp_out = 0.0f;

    expect_adc_read_sequence(raw_45c);

    temp_get_celsius_once(&temp_sensor, &temp_out);

    TEST_ASSERT_FLOAT_WITHIN(0.5f, 45.0f, temp_out);
}

/* =================================================================---------
    TEST GESTIONE ERRORI
   ========================================================================= */

/**
 * @test Verifica che il driver gestisca il fallimento dell'avvio ADC.
 */
void test_temp_read_raw_fails_if_start_fails(void) {
    uint32_t raw_val;

    HAL_ADC_ConfigChannel_ExpectAndReturn(&hadc_test, &temp_sensor.channel_cfg, HAL_OK);
    HAL_ADC_Start_ExpectAndReturn(&hadc_test, HAL_ERROR);

    Temp_Status_t status = temp_read_raw_once(&temp_sensor, &raw_val);

    TEST_ASSERT_EQUAL(TEMP_ERR_COMM, status);
}

/**
 * @test Verifica la protezione contro puntatori NULL nelle API pubbliche.
 */
void test_temp_null_pointers(void) {
    float t;
    TEST_ASSERT_EQUAL(TEMP_ERR, temp_init(NULL, &hadc_test, &config_test, 10));
    TEST_ASSERT_EQUAL(TEMP_ERR, temp_get_celsius_once(NULL, &t));
    TEST_ASSERT_EQUAL(TEMP_ERR, temp_get_celsius_once(&temp_sensor, NULL));
}

/**
 * @test Verifica la conversione ai limiti fisici dell'ADC (0 e 4095).
 */
void test_temp_conversion_at_boundaries(void) {
    float temp_out;

    // Caso 0: Tensione minima
    expect_adc_read_sequence(0);
    temp_get_celsius_once(&temp_sensor, &temp_out);
    // (0 - 760)/2.5 + 25 = -304 + 25 = -279°C (valore teorico minimo)
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -279.0f, temp_out);

    // Caso 4095: Tensione massima (3300mV)
    expect_adc_read_sequence(4095);
    temp_get_celsius_once(&temp_sensor, &temp_out);
    // (3300 - 760)/2.5 + 25 = 1016 + 25 = 1041°C
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1041.0f, temp_out);
}
