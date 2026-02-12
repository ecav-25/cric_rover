/**
 * @file test_utils.c
 * @brief Unit Tests per le funzioni di utility del Rover.
 * @author Gruppo 2
 * * @details Questo file testa le funzioni inline definite in utils.h.
 * Vengono testate le rampe di accelerazione, la conversione della telemetria
 * batteria, la saturazione della temperatura e la decodifica degli stati della FSM.
 */

#include "unity.h"
#include "utils.h"

/* ===================== TEST FUNZIONE RAMP ===================== */

/**
 * @test Verifica l'incremento verso un target superiore.
 */
void test_ramp_increase(void) {
    real32_T current = 10.0f;
    real32_T target = 20.0f;
    real32_T step = 2.0f;

    real32_T result = ramp(current, target, step);

    TEST_ASSERT_EQUAL_FLOAT(12.0f, result);
}

/**
 * @test Verifica il decremento verso un target inferiore.
 */
void test_ramp_decrease(void) {
    real32_T current = 15.0f;
    real32_T target = 10.0f;
    real32_T step = 1.0f;

    real32_T result = ramp(current, target, step);

    TEST_ASSERT_EQUAL_FLOAT(14.0f, result);
}

/**
 * @test Verifica che il valore si fermi esattamente al target se la distanza è minore dello step.
 */
void test_ramp_snap_to_target(void) {
    real32_T current = 10.0f;
    real32_T target = 10.5f;
    real32_T step = 1.0f;

    real32_T result = ramp(current, target, step);

    TEST_ASSERT_EQUAL_FLOAT(10.5f, result);
}

/**
 * @test Verifica che la rampa non modifichi il valore se siamo già a target.
 */
void test_ramp_already_at_target(void) {
    real32_T result = ramp(10.0f, 10.0f, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, result);
}

/**
 * @test Verifica il comportamento con step molto piccolo (precisione float).
 */
void test_ramp_very_small_step(void) {
    real32_T result = ramp(10.0f, 10.0001f, 0.00005f);
    TEST_ASSERT_EQUAL_FLOAT(10.00005f, result);
}

/* ===================== TEST TELEMETRIA BATTERIA ===================== */

/**
 * @test Verifica i limiti della batteria (0% e 100%).
 *
 */
void test_calculate_battery_limits(void) {
    /* Sotto la soglia minima (9V) */
    TEST_ASSERT_EQUAL_UINT8(0, calculate_battery_percent(8.5f));
    TEST_ASSERT_EQUAL_UINT8(0, calculate_battery_percent(9.0f));

    /* Sopra la soglia massima (12.6V) */
    TEST_ASSERT_EQUAL_UINT8(100, calculate_battery_percent(12.6f));
    TEST_ASSERT_EQUAL_UINT8(100, calculate_battery_percent(13.0f));
}

/**
 * @test Verifica il calcolo lineare al 50%.
 * @details Con range 9.0V-12.6V (delta 3.6V), 10.8V deve essere il 50%.
 */
void test_calculate_battery_mid_range(void) {
    TEST_ASSERT_EQUAL_UINT8(50, calculate_battery_percent(10.8f));
}

/**
 * @test Verifica la precisione dell'arrotondamento della batteria.
 */
void test_calculate_battery_rounding_boundaries(void) {
    /* 9.0V + 1% di 3.6V = 9.036V -> dovrebbe essere 1% */
    TEST_ASSERT_EQUAL_UINT8(1, calculate_battery_percent(9.036f));

    /* Poco sotto il 100% */
    TEST_ASSERT_EQUAL_UINT8(99, calculate_battery_percent(12.55f));
}

/* ===================== TEST SATURAZIONE TEMPERATURA ===================== */

/**
 * @test Verifica la saturazione agli estremi di un int8_T.
 *
 */
void test_saturate_temperature_clipping(void) {
    TEST_ASSERT_EQUAL_INT8(127, saturate_temperature(150.0f));
    TEST_ASSERT_EQUAL_INT8(-128, saturate_temperature(-200.0f));
}

/**
 * @test Verifica l'arrotondamento corretto della temperatura.
 */
void test_saturate_temperature_rounding(void) {
    /* Arrotondamento positivi */
    TEST_ASSERT_EQUAL_INT8(25, saturate_temperature(25.4f));
    TEST_ASSERT_EQUAL_INT8(26, saturate_temperature(25.6f));

    /* Arrotondamento negativi */
    TEST_ASSERT_EQUAL_INT8(-10, saturate_temperature(-10.4f));
    TEST_ASSERT_EQUAL_INT8(-11, saturate_temperature(-10.6f));
}

/**
 * @test Verifica il comportamento esattamente a zero e ai confini di saturazione.
 */
void test_saturate_temperature_exact_boundaries(void) {
    /* Zero */
    TEST_ASSERT_EQUAL_INT8(0, saturate_temperature(0.0f));
    TEST_ASSERT_EQUAL_INT8(0, saturate_temperature(0.4f));
    TEST_ASSERT_EQUAL_INT8(0, saturate_temperature(-0.4f));

    /* Limiti esatti int8_t */
    TEST_ASSERT_EQUAL_INT8(127, saturate_temperature(127.0f));
    TEST_ASSERT_EQUAL_INT8(127, saturate_temperature(127.4f));
    TEST_ASSERT_EQUAL_INT8(-128, saturate_temperature(-128.0f));
    TEST_ASSERT_EQUAL_INT8(-128, saturate_temperature(-128.4f));
}

/* ===================== TEST DECODIFICA STATI (FSM) ===================== */

/**
 * @test Verifica la conversione degli stati delle luci.
 */
void test_get_light_mode_mapping(void) {
    TEST_ASSERT_EQUAL(LIGHT_AUTO, get_light_mode(Board2_IN_Lights_AUTO));
    TEST_ASSERT_EQUAL(LIGHT_ON, get_light_mode(Board2_IN_Lights_ON));
    TEST_ASSERT_EQUAL(LIGHT_OFF, get_light_mode(999)); /* Default case */
}

/**
 * @test Verifica la conversione delle modalità di guida.
 */
void test_get_driving_mode_mapping(void) {
    TEST_ASSERT_EQUAL(DM_SPORT, get_driving_mode(Board2_IN_Mode_SPORT));
    TEST_ASSERT_EQUAL(DM_ECO, get_driving_mode(Board2_IN_Mode_ECO));
    TEST_ASSERT_EQUAL(DM_DEFAULT, get_driving_mode(0)); /* Default case */
}

/**
 * @test Verifica che valori non definiti vengano gestiti dai rami else (Fail-Safe).
 */
void test_modes_unexpected_values(void) {
    /* Luci: un valore casuale deve tornare LIGHT_OFF */
    TEST_ASSERT_EQUAL(LIGHT_OFF, get_light_mode(0xFFFFFFFF));

    /* Driving Mode: un valore casuale deve tornare DM_DEFAULT */
    TEST_ASSERT_EQUAL(DM_DEFAULT, get_driving_mode(0x1234));
}
