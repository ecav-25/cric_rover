/**
 * @file test_pid_law.c
 * @author Gruppo 2
 * @brief Unit Tests per la legge di controllo PID discreta.
 *
 * @details I test verificano la corretta implementazione dell'equazione:
 * $$u[k] = -a_1 \cdot u[k-1] + b_0 \cdot e[k] + b_1 \cdot e[k-1]$$
 */

#include "unity.h"
#include "pid_law.h"
#include <string.h>

/** @name Risorse di Test
 * @{ */
static PID_Law_t pid;
/** @} */

/**
 * @brief Inizializzazione pre-test.
 */
void setUp(void) {
    memset(&pid, 0, sizeof(PID_Law_t));
}

/* =================================================================---------
    TEST INIZIALIZZAZIONE E RESET
   ========================================================================= */

/**
 * @test Verifica che l'inizializzazione imposti correttamente i coefficienti.
 */
void test_PID_Law_init_should_store_coefficients_and_reset(void) {
    pid_law_status status = PID_Law_init(&pid, 0.5f, 10.0f, 2.0f);

    TEST_ASSERT_EQUAL(PID_LAW_OK, status);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, pid.a1);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, pid.b0);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, pid.b1);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.u_prev);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.e_prev);
}

/**
 * @test Verifica che il reset azzeri gli stati ma mantenga i coefficienti.
 */
void test_PID_Law_reset_should_clear_memory_only(void) {
    PID_Law_init(&pid, 0.1f, 0.2f, 0.3f);
    pid.u_prev = 10.0f;
    pid.e_prev = 5.0f;

    PID_Law_reset(&pid);

    TEST_ASSERT_EQUAL_FLOAT(0.1f, pid.a1); // Coefficienti intatti
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.u_prev); // Memoria azzerata
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.e_prev);
}

/* =================================================================---------
    TEST CALCOLO MATEMATICO (PASSO SINGOLO E MULTIPLO)
   ========================================================================= */

/**
 * @test Verifica il calcolo al primo passo (u_prev ed e_prev sono 0).
 * @details e[0] = 10 - 0 = 10. u[0] = -a1*0 + b0*10 + b1*0 = 100.
 * Con saturazione a 12.0V, ci aspettiamo 12.0V.
 */

void test_PID_Law_compute_single_step(void) {
    /* Coefficienti semplici per calcolo a mente: b0 = 1.0 */
    PID_Law_init(&pid, 0.0f, 1.0f, 0.0f);
    float u_out = 0.0f;

    /* Setpoint = 5, Feedback = 2 -> Errore = 3 */
    PID_Law_compute(&pid, 5.0f, 2.0f, &u_out);

    TEST_ASSERT_EQUAL_FLOAT(3.0f, u_out);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, pid.e_prev);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, pid.u_prev);
}

/**
 * @test Verifica l'evoluzione dinamica su due passi.
 * @details
 * Passo 1: e[0]=1, u[0] = -0*0 + 1*1 + 0.5*0 = 1.0
 * Passo 2: e[1]=1, u[1] = -0.1*1 + 1*1 + 0.5*1 = -0.1 + 1 + 0.5 = 1.4
 */
void test_PID_Law_compute_two_steps(void) {
    PID_Law_init(&pid, 0.1f, 1.0f, 0.5f);
    float u_out = 0.0f;

    /* Passo 1 */
    PID_Law_compute(&pid, 1.0f, 0.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, u_out);

    /* Passo 2 */
    PID_Law_compute(&pid, 1.0f, 0.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(1.4f, u_out);
}

/* =================================================================---------
    TEST SATURAZIONE (ANTI-WINDUP)
   ========================================================================= */

/**
 * @test Verifica la saturazione positiva (U_MAX).
 */
void test_PID_Law_saturation_max(void) {
    PID_Law_init(&pid, 0.0f, 100.0f, 0.0f); // Guadagno enorme
    float u_out = 0.0f;

    PID_Law_compute(&pid, 10.0f, 0.0f, &u_out);

    TEST_ASSERT_EQUAL_FLOAT(U_MAX, u_out);
    /* Fondamentale: u_prev deve salvare il valore saturato, non quello calcolato */
    TEST_ASSERT_EQUAL_FLOAT(U_MAX, pid.u_prev);
}

/**
 * @test Verifica la saturazione negativa (U_MIN).
 */

void test_PID_Law_saturation_min(void) {
    PID_Law_init(&pid, 0.0f, 100.0f, 0.0f);
    float u_out = 0.0f;

    PID_Law_compute(&pid, 0.0f, 10.0f, &u_out); // Errore negativo -10

    TEST_ASSERT_EQUAL_FLOAT(U_MIN, u_out);
    TEST_ASSERT_EQUAL_FLOAT(U_MIN, pid.u_prev);
}

/* =================================================================---------
    TEST ROBUSTEZZA
   ========================================================================= */

/**
 * @test Verifica gestione puntatori NULL.
 */
void test_PID_Law_null_pointers(void) {
    float u;
    TEST_ASSERT_EQUAL(PID_LAW_ERR, PID_Law_init(NULL, 0, 0, 0));
    TEST_ASSERT_EQUAL(PID_LAW_ERR, PID_Law_compute(NULL, 0, 0, &u));
    TEST_ASSERT_EQUAL(PID_LAW_ERR, PID_Law_compute(&pid, 0, 0, NULL));
}

/**
 * @test Verifica la stabilità quando l'errore torna a zero.
 * @details Se l'errore è zero ma u_prev era alto, l'uscita deve evolvere
 * secondo il termine autoregressivo -a1 * u_prev.
 */
void test_PID_Law_convergence_to_zero(void) {
    PID_Law_init(&pid, 0.5f, 1.0f, 0.0f); // a1 = 0.5
    float u_out;

    // Passo 1: Generiamo un'uscita u[0] = 10.0
    PID_Law_compute(&pid, 10.0f, 0.0f, &u_out);

    // Passo 2: Errore diventa zero. u[1] = -0.5 * 10.0 + 1.0 * 0 = -5.0
    PID_Law_compute(&pid, 10.0f, 10.0f, &u_out);

    TEST_ASSERT_EQUAL_FLOAT(-5.0f, u_out);
}

/**
 * @test Verifica la risposta a un gradino (Step Response).
 * @details Simula un salto improvviso del setpoint e verifica che la
 * combinazione di b0 ed e_prev (b1) produca il valore atteso.
 */
void test_PID_Law_step_response(void) {
    PID_Law_init(&pid, 0.0f, 2.0f, 1.0f); // b0=2, b1=1
    float u_out;

    // Passo 1: Setpoint da 0 a 10. e_now=10, e_prev=0. u = 2*10 + 1*0 = 20 -> Saturation 12.0
    PID_Law_compute(&pid, 10.0f, 0.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(U_MAX, u_out);

    // Passo 2: Setpoint resta 10. e_now=0, e_prev=10. u = 2*0 + 1*10 = 10.0
    PID_Law_compute(&pid, 10.0f, 10.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, u_out);
}

/**
 * @test Verifica il comportamento con coefficienti molto piccoli.
 * @details Assicura che la precisione float sia sufficiente per il calcolo
 * senza accumulare errori macroscopici in un singolo passo.
 */
void test_PID_Law_very_small_coefficients(void) {
    PID_Law_init(&pid, 0.0001f, 0.0001f, 0.0001f);
    float u_out;

    PID_Law_compute(&pid, 1.0f, 0.0f, &u_out);

    // u = -0.0001*0 + 0.0001*1 + 0.0001*0 = 0.0001
    TEST_ASSERT_FLOAT_WITHIN(0.000001f, 0.0001f, u_out);
}
