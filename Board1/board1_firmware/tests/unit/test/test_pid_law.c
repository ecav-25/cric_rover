/**
 * @file test_pid_law.c
 * @brief Unit Tests per la logica di controllo PID discreta del Rover.
 * @author Gruppo 2
 * @date 2026
 * * @details Il file verifica l'integrità della struttura dati del PID, la gestione
 * della memoria (reset) e l'applicazione dei coefficienti per l'equazione alle differenze:
 * \f[ u[k] = a_1 \cdot u[k-1] + b_0 \cdot e[k] + b_1 \cdot e[k-1] \f]
 */

#include "unity.h"
#include "pid_law.h"
#include <string.h>

/** * @name Risorse Statiche di Test
 * @{
 */
static PID_Law_t pid; /**< Istanza globale del PID utilizzata per i test di unità. */
/** @} */

/**
 * @brief Setup eseguito prima di ogni test.
 * @details Resetta l'intera struttura #pid a zero tramite memset per garantire
 * l'isolamento tra i casi di test.
 */
void setUp(void) {
    memset(&pid, 0, sizeof(PID_Law_t));
}

/* =================================================================---------
    TEST INIZIALIZZAZIONE E RESET
   ========================================================================= */

/**
 * @test **test_PID_Law_init_success**
 * @brief Verifica che la funzione di init memorizzi correttamente i coefficienti.
 * @details
 * - **Pre-condizione:** Struttura PID azzerata.
 * - **Azione:** Inizializzazione con a1=0.5, b0=10.0, b1=2.0.
 * - **Risultato atteso:** Status OK e coefficienti mappati correttamente nei campi della struttura.
 */
void test_PID_Law_init_success(void) {
    pid_law_status status = PID_Law_init(&pid, 0.5f, 10.0f, 2.0f);

    TEST_ASSERT_EQUAL(PID_LAW_OK, status);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, pid.a1);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, pid.b0);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, pid.b1);
    /* Verifica implicita che la memoria sia stata resettata durante l'init */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.u_prev);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.e_prev);
}

/**
 * @test **test_PID_Law_reset_clears_memory**
 * @brief Verifica che il reset azzeri solo le variabili di stato (memoria).
 * @details
 * Viene simulato uno stato di esecuzione precedente con valori non nulli in `u_prev` ed `e_prev`.
 * Dopo il reset, i coefficienti devono rimanere invariati mentre gli stati devono tornare a zero.
 * *
 */
void test_PID_Law_reset_clears_memory(void) {
    /* Setup dello stato */
    PID_Law_init(&pid, 0.1f, 0.2f, 0.3f);
    pid.u_prev = 100.0f;
    pid.e_prev = 50.0f;

    /* Esecuzione */
    pid_law_status status = PID_Law_reset(&pid);

    /* Verifica */
    TEST_ASSERT_EQUAL(PID_LAW_OK, status);
    TEST_ASSERT_EQUAL_FLOAT(0.1f, pid.a1);    /* Il coefficiente deve persistere */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.u_prev); /* Lo stato deve essere resettato */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pid.e_prev);
}

/* =================================================================---------
    TEST CALCOLO MATEMATICO (PASSO SINGOLO E MULTIPLO)
   ========================================================================= */

/**
 * @test **test_PID_Law_compute_two_steps**
 * @brief Verifica l'evoluzione dinamica della legge di controllo su due passi temporali.
 * @details
 * Viene verificata la corretta accumulazione degli stati precedenti (u_prev ed e_prev).
 * - **Passo 1**: \f$ e[0]=1, u[0] = (0.1 \cdot 0) + (1 \cdot 1) + (0.5 \cdot 0) = 1.0 \f$
 * - **Passo 2**: \f$ e[1]=1, u[1] = (0.1 \cdot 1.0) + (1 \cdot 1) + (0.5 \cdot 1) = 1.6 \f$
 */
void test_PID_Law_compute_two_steps(void) {
    PID_Law_init(&pid, 0.1f, 1.0f, 0.5f);
    float u_out = 0.0f;

    /* Esecuzione Passo 1 */
    PID_Law_compute(&pid, 1.0f, 0.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, u_out);

    /* Esecuzione Passo 2 */
    PID_Law_compute(&pid, 1.0f, 0.0f, &u_out);
    TEST_ASSERT_EQUAL_FLOAT(1.6f, u_out);
}

/**
 * @test **test_PID_Law_convergence_with_zero_error**
 * @brief Verifica la risposta del sistema quando l'errore di inseguimento è nullo.
 * @details In assenza di errore, l'uscita deve dipendere esclusivamente dal termine
 * autoregressivo \f$ a_1 \cdot u_{prev} \f$.
 */
void test_PID_Law_convergence_with_zero_error(void) {
    PID_Law_init(&pid, 0.5f, 1.0f, 0.0f);
    float u_out;

    pid.u_prev = 10.0f;
    pid.e_prev = 0.0f;

    /* Setpoint = Feedback (Errore = 0) */
    PID_Law_compute(&pid, 10.0f, 10.0f, &u_out);

    TEST_ASSERT_EQUAL_FLOAT(5.0f, u_out);
}

/* =================================================================---------
    TEST SATURAZIONE E STABILITÀ
   ========================================================================= */

/**
 * @test **test_PID_Law_saturation_max**
 * @brief Verifica l'intervento del limite di saturazione superiore (U_MAX).
 * @details Garantisce che l'uscita non superi i limiti fisici dell'attuatore e che
 * il valore memorizzato in `u_prev` sia quello saturato (Anti-Windup preventivo).
 *
 */
void test_PID_Law_saturation_max(void) {
    PID_Law_init(&pid, 0.0f, 1000.0f, 0.0f);
    float u_out = 0.0f;

    PID_Law_compute(&pid, 10.0f, 0.0f, &u_out);

    TEST_ASSERT_EQUAL_FLOAT(U_MAX, u_out);
    TEST_ASSERT_EQUAL_FLOAT(U_MAX, pid.u_prev);
}

/**
 * @test **test_PID_Law_stability_long_run**
 * @brief Verifica la stabilità asintotica del filtro digitale.
 * @details
 * Per un sistema stabile (\f$ |a_1| < 1 \f$), l'uscita deve decadere verso lo zero
 * in assenza di input.
 * - **Calcolo**: Con \f$ a_1 = 0.8 \f$, dopo 25 passi il valore 10.0 decade a \f$ 10 \cdot 0.8^{25} \approx 0.037 \f$.
 */
void test_PID_Law_stability_long_run(void) {
    PID_Law_init(&pid, 0.8f, 1.0f, 0.0f);
    float u_out = 0.0f;

    pid.u_prev = 10.0f;
    /* Aumentiamo i passi a 25 per garantire una convergenza sotto la soglia di 0.1 */
    for(int i = 0; i < 25; i++) {
        PID_Law_compute(&pid, 0.0f, 0.0f, &u_out);
    }

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, u_out);
}
