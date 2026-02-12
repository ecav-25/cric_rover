/**
 * @file test_utils.c
 * @author Gruppo 2
 * @brief Unit Tests per le funzioni di utilità (Rampa e Controllo).
 */

#include "unity.h"
#include "utils.h" // Includiamo il file direttamente

/* =================================================================---------
    TEST FUNZIONE RAMP
   ========================================================================= */

/**
 * @test Verifica l'incremento graduale verso il target (Accelerazione).
 */
void test_ramp_should_increase_gradually(void) {
    // current=100, target=200, step=10. Risultato atteso: 110
    real32_T result = ramp(100.0f, 200.0f, 10.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(110.0f, result);
}

/**
 * @test Verifica la frenata di emergenza (EMERGENCY).
 * @details Se target è 0 e modo è EMERGENCY, deve restituire 0 istantaneamente.
 */

void test_ramp_emergency_braking_should_be_instant(void) {
    // Anche se lo step è piccolo, con EMERGENCY deve andare a zero subito
    real32_T result = ramp(500.0f, 0.0f, 10.0f, EMERGENCY);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, result);
}

/**
 * @test Verifica la frenata normale (NORMAL) verso target 0.
 * @details Deve applicare il coefficiente NORMAL_BRK_COEFF (0.5).
 * Passo atteso: 10 * 0.5 = 5. Risultato: 100 - 5 = 95.
 */
void test_ramp_normal_braking_to_zero_should_be_smoothed(void) {
    real32_T result = ramp(100.0f, 0.0f, 10.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(95.0f, result);
}

/**
 * @test Verifica che il valore si "agganci" al target se la differenza è minore dello step.
 */
void test_ramp_should_snap_to_target(void) {
    // Siamo a 195, target 200, step 10. Non deve fare 205, ma fermarsi a 200.
    real32_T result = ramp(195.0f, 200.0f, 10.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(200.0f, result);
}

/**
 * @test Verifica la decelerazione normale (non verso zero).
 * @details Se il target non è 0, anche in NORMAL lo step non viene ridotto.
 */
void test_ramp_decrease_towards_non_zero_target(void) {
    // Da 200 a 150 con step 10. Risultato atteso: 190.
    real32_T result = ramp(200.0f, 150.0f, 10.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(190.0f, result);
}

/**
 * @test Verifica che la rampa sia stabile una volta raggiunto il target.
 */
void test_ramp_steady_state(void) {
    // Siamo a 100, target 100. Deve restare 100.
    real32_T result = ramp(100.0f, 100.0f, 10.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, result);
}

/**
 * @test Verifica la robustezza con step non validi.
 */
void test_ramp_zero_step_should_not_change_speed(void) {
    // Se lo step è 0, la velocità non deve cambiare
    real32_T result = ramp(100.0f, 200.0f, 0.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, result);
}

/**
 * @test Verifica l'inversione di marcia (passaggio per lo zero).
 */
void test_ramp_crossing_zero(void) {
    // Da -10 a +10 con step 5.
    // Passo 1: -10 + 5 = -5
    real32_T result = ramp(-10.0f, 10.0f, 5.0f, NORMAL);
    TEST_ASSERT_EQUAL_FLOAT(-5.0f, result);
}
