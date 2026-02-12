/**
 * @file test_motor.c
 * @author Gruppo 2
 * @brief Unit Tests per il driver motori DC (PWM).
 *
 * @details Questa suite valida la logica di calibrazione hardware.
 * Utilizza un'istanza reale della struttura TIM_TypeDef (definita in hal.h)
 * per intercettare le scritture nei registri CCR effettuate dal driver.
 */

#include "unity.h"
#include <string.h>

/* Mock dell'interfaccia HAL personalizzata */
#include "mock_stm32g4xx_hal.h"
#include "motor.h"

/** @name Risorse di Test
 * @{ */
static Motor_t motor_L;
static TIM_HandleTypeDef htim_test;
static TIM_TypeDef dummy_regs;      /**< Registri "fisici" simulati */
static Motor_Calibration_t calib_L;
/** @} */

/**
 * @brief Setup eseguito prima di ogni test.
 * Inizializza l'handle del timer puntandolo ai registri dummy e configura
 * i parametri di calibrazione.
 */
void setUp(void) {
    memset(&motor_L, 0, sizeof(Motor_t));
    memset(&htim_test, 0, sizeof(TIM_HandleTypeDef));
    memset(&dummy_regs, 0, sizeof(TIM_TypeDef));

    /* Colleghiamo l'handle dell'HAL ai registri simulati per il test */
    htim_test.Instance = &dummy_regs;

    /* Configurazione: Neutro a 1500, Range avanti +500, Range indietro -400 */
    calib_L.pwm_stop = 1500;
    calib_L.pwm_scale_forward = 500;
    calib_L.pwm_scale_backward = 400;
}

/* =================================================================---------
    TEST DI INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica che l'init porti il motore in posizione di stop.
 * @details Deve scrivere il valore pwm_stop nel registro CCR1 e avviare il timer.
 */
void test_motor_init_should_set_stop_value_and_start_pwm(void) {
    /* Ora HAL_TIM_PWM_Start viene riconosciuta grazie all'update di hal.h */
    HAL_TIM_PWM_Start_ExpectAndReturn(&htim_test, TIM_CHANNEL_1, HAL_OK);

    Motor_Status_t status = motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    TEST_ASSERT_EQUAL(MOTOR_OK, status);
    /* Verifica che il registro CCR1 sia stato impostato al valore di neutro */
    TEST_ASSERT_EQUAL(1500, dummy_regs.CCR1);
}

/* =================================================================---------
    TEST CALCOLO POTENZA E DIREZIONE
   ========================================================================= */

/**
 * @test Verifica la marcia avanti (Clockwise) al 50%.
 * @details Calcolo atteso: 1500 + (50 * 500 / 100) = 1750.
 */
void test_motor_set_forward_half_speed(void) {
    /* Ignoriamo l'init per focalizzarci sul calcolo PWM */
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    motor_set(&motor_L, 50, CLOCKWISE);

    /* Il driver deve aver scritto 1750 nel registro del canale 1 */
    TEST_ASSERT_EQUAL(1750, dummy_regs.CCR1);
}

/**
 * @test Verifica la marcia indietro (Counter-Clockwise) al 100%.
 * @details Calcolo atteso: 1500 - (100 * 400 / 100) = 1100.
 */

void test_motor_set_backward_full_speed(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    motor_set(&motor_L, 100, COUNTERCLOCKWISE);

    TEST_ASSERT_EQUAL(1100, dummy_regs.CCR1);
}

/**
 * @test Verifica che la potenza 0 (Stop) resetti il CCR al valore pwm_stop.
 */
void test_motor_set_zero_duty_should_go_to_stop_value(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    /* Portiamo prima il motore a 100% avanti */
    motor_set(&motor_L, 100, CLOCKWISE);

    /* Poi fermiamolo */
    motor_set(&motor_L, 0, CLOCKWISE);

    TEST_ASSERT_EQUAL(1500, dummy_regs.CCR1);
}

/* =================================================================---------
    TEST SATURAZIONE E SICUREZZA
   ========================================================================= */

/**
 * @test Verifica che valori di duty cycle > 100 vengano limitati correttamente.
 */
void test_motor_set_over_100_duty_should_be_clipped(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    /* Richiesta 200% (fuori scala), deve comportarsi come 100% */
    motor_set(&motor_L, 200, CLOCKWISE);

    TEST_ASSERT_EQUAL(2000, dummy_regs.CCR1);
}

/**
 * @test Verifica la gestione dei parametri NULL nelle API.
 */
void test_motor_api_should_return_error_on_null_ptrs(void) {
    TEST_ASSERT_EQUAL(MOTOR_ERR, motor_init(NULL, &htim_test, TIM_CHANNEL_1, &calib_L));
    TEST_ASSERT_EQUAL(MOTOR_ERR, motor_set(NULL, 50, CLOCKWISE));
}

/**
 * @test Verifica la precisione dell'arrotondamento (roundf).
 * @details Con scale 500 e duty 1, il calcolo sarebbe 1500 + (1 * 500 / 100) = 1505.
 * Ma con una scala diversa, es. 505, 1% di 505 è 5.05. Deve arrotondare a 5.
 */
void test_motor_set_rounding_check(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    calib_L.pwm_scale_forward = 505; // 1% di 505 = 5.05
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    motor_set(&motor_L, 1, CLOCKWISE);

    // 1500 + round(5.05) = 1505
    TEST_ASSERT_EQUAL(1505, dummy_regs.CCR1);
}

/**
 * @test Verifica che direzioni non valide fermino il motore per sicurezza.
 */
void test_motor_set_invalid_direction_should_stop(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    // Supponiamo un cast errato o un valore fuori enum (es. 0 o 42)
    motor_set(&motor_L, 50, (Motor_Direction_t)42);

    TEST_ASSERT_EQUAL(1500, dummy_regs.CCR1);
}

/**
 * @test Verifica il funzionamento su un canale diverso (es. Canale 2).
 */
void test_motor_init_on_channel_2(void) {
    // TIM_CHANNEL_2 di solito ha un offset di 4 o 1 rispetto a CCR1
    HAL_TIM_PWM_Start_ExpectAndReturn(&htim_test, TIM_CHANNEL_2, HAL_OK);

    motor_init(&motor_L, &htim_test, TIM_CHANNEL_2, &calib_L);
    motor_set(&motor_L, 100, CLOCKWISE);

    // Verifica che abbia scritto nel registro CCR2 e non CCR1
    TEST_ASSERT_EQUAL(2000, dummy_regs.CCR2);
    TEST_ASSERT_EQUAL(0, dummy_regs.CCR1); // CCR1 deve essere rimasto pulito
}

/**
 * @test Verifica che non ci siano overflow con scale massime.
 */
void test_motor_set_max_uint16_scale(void) {
    HAL_TIM_PWM_Start_IgnoreAndReturn(HAL_OK);
    calib_L.pwm_stop = 1000;
    calib_L.pwm_scale_forward = 60000; // Valore molto alto
    motor_init(&motor_L, &htim_test, TIM_CHANNEL_1, &calib_L);

    motor_set(&motor_L, 100, CLOCKWISE);

    // 1000 + 60000 = 61000 (deve stare in uint16 senza wrappare a valori bassi)
    TEST_ASSERT_EQUAL(61000, dummy_regs.CCR1);
}
