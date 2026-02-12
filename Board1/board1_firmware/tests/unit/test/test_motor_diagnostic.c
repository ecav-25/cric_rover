/**
 * @file test_motor_diagnostic.c
 * @author Gruppo 2
 * @brief Unit Tests per il modulo di diagnostica avanzata dei motori.
 *
 * @details Questa suite di test valida l'algoritmo di monitoraggio della salute dei motori.
 * Il modulo utilizza una tecnica di analisi dell'errore integrale (IAE) con compensazione
 * del ritardo di fase (delay shift).
 *
 */

#include "unity.h"
#include <string.h>
#include "motor_diagnostic.h"

/** @name Global Test Resources
 * @{ */
static MotorDiag_Handle_t hdiag;       /**< Handle dell'istanza di diagnostica */
static MotorDiag_Config_t config;      /**< Configurazione dei parametri di soglia */
/** @} */

/**
 * @brief Inizializzazione pre-test.
 * Imposta parametri di calibrazione standard: 2 campioni di shift e 100.0 di area limite.
 */
void setUp(void) {
    memset(&hdiag, 0, sizeof(MotorDiag_Handle_t));

    config.delay_shift = 2;
    config.max_area_limit = 100.0f;
}

/* =================================================================---------
    TEST DI INIZIALIZZAZIONE E GESTIONE MEMORIA
   ========================================================================= */

/**
 * @test Verifica che l'inizializzazione resetti correttamente tutti i buffer.
 * @details L'handle viene sporcato intenzionalmente prima dell'init per verificare
 * che il memset interno al driver funzioni correttamente.
 */
void test_motor_diag_init_should_clear_all_buffers(void) {
    /* Riempimento con pattern di bit sporco (0xFF) */
    memset(&hdiag, 0xFF, sizeof(MotorDiag_Handle_t));

    MotorDiag_Status_t status = motor_diag_init(&hdiag, &config);

    TEST_ASSERT_EQUAL(M_DIAG_OK, status);
    TEST_ASSERT_EQUAL(MOTOR_HEALTHY, hdiag.health_status);
    TEST_ASSERT_EQUAL(0.0f, hdiag.buf_ref_A[0]);
    TEST_ASSERT_EQUAL(0.0f, hdiag.buf_ref_B[M_DIAG_BUF_LEN - 1]);
    TEST_ASSERT_FALSE(hdiag.is_buffer_ready);
}

/* =================================================================---------
    TEST LOGICA BUFFER CIRCOLARE (DOUBLE BUFFERING)
   ========================================================================= */

/**
 * @test Verifica lo switch tra buffer A e B dopo aver saturato la capacità.
 * @details Simula l'inserimento di campioni fino al limite M_DIAG_BUF_LEN e
 * controlla che la scrittura passi automaticamente al buffer secondario.
 */

void test_motor_diag_record_should_switch_buffers_when_full(void) {
    motor_diag_init(&hdiag, &config);

    /* Riempimento primo buffer (A) */
    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        TEST_ASSERT_EQUAL(0, hdiag.active_buf_idx);
        motor_diag_record(&hdiag, 10.0f, 10.0f);
    }

    /* Verifica trigger del flag di elaborazione e switch indice */
    TEST_ASSERT_TRUE(hdiag.is_buffer_ready);
    TEST_ASSERT_EQUAL(1, hdiag.active_buf_idx);
    TEST_ASSERT_EQUAL(0, hdiag.write_idx);
}

/* =================================================================---------
    TEST ALGORITMO DI DIAGNOSTICA (IAE SHIFTED)
   ========================================================================= */

/**
 * @test Verifica il comportamento in condizioni di motore operativo (Healthy).
 * @details Con errore zero tra riferimento e misura, l'area deve risultare nulla.
 */
void test_motor_diag_process_healthy_motor(void) {
    motor_diag_init(&hdiag, &config);

    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        motor_diag_record(&hdiag, 50.0f, 50.0f);
    }

    motor_diag_process(&hdiag);

    TEST_ASSERT_EQUAL(0.0f, hdiag.last_area_error);
    TEST_ASSERT_EQUAL(MOTOR_HEALTHY, hdiag.health_status);
}

/**
 * @test Verifica il rilevamento di un guasto critico (Motor Failure).
 * @details Simula un motore bloccato (meas = 0) mentre il setpoint è attivo.
 * Calcola matematicamente l'area attesa: (48 campioni - 2 shift) * 10.0 err = 460.0.
 */

void test_motor_diag_process_failure_detection(void) {
    motor_diag_init(&hdiag, &config);

    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        motor_diag_record(&hdiag, 10.0f, 0.0f);
    }

    motor_diag_process(&hdiag);

    /* Tolleranza floating point 0.1 */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 460.0f, hdiag.last_area_error);
    TEST_ASSERT_EQUAL(MOTOR_FAILURE, hdiag.health_status);
}

/**
 * @test Verifica l'efficacia della compensazione del ritardo (Delay Shift).
 * @details Simula errore solo nei primi N campioni. Se lo shift è impostato
 * correttamente, questi errori devono essere ignorati dall'integrale.
 */
void test_motor_diag_shift_logic(void) {
    config.delay_shift = 10;
    motor_diag_init(&hdiag, &config);

    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        /* Errore presente solo nella finestra di shift */
        float meas = (i < 10) ? 0.0f : 10.0f;
        motor_diag_record(&hdiag, 10.0f, meas);
    }

    motor_diag_process(&hdiag);

    /* L'area deve essere 0 perché l'errore è fuori dalla zona di analisi */
    TEST_ASSERT_EQUAL(0.0f, hdiag.last_area_error);
}

/* =================================================================---------
    TEST DI ROBUSTEZZA (EDGE CASES)
   ========================================================================= */

/**
 * @test Verifica che le API gestiscano correttamente puntatori NULL senza crash.
 */
void test_motor_diag_null_pointers(void) {
    TEST_ASSERT_EQUAL(M_DIAG_ERR, motor_diag_init(NULL, &config));
    TEST_ASSERT_EQUAL(M_DIAG_ERR, motor_diag_record(NULL, 1.0f, 1.0f));
    TEST_ASSERT_EQUAL(M_DIAG_ERR, motor_diag_process(NULL));
}

/**
 * @test Verifica la gestione di uno shift invalido (maggiore della lunghezza buffer).
 * @details Se lo shift è >= M_DIAG_BUF_LEN, l'algoritmo non deve calcolare nulla
 * e restituire area 0.0f senza crashare.
 */
void test_motor_diag_extreme_shift_management(void) {
    config.delay_shift = M_DIAG_BUF_LEN + 5; // Shift impossibile
    motor_diag_init(&hdiag, &config);

    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        motor_diag_record(&hdiag, 10.0f, 0.0f); // Errore massimo
    }

    motor_diag_process(&hdiag);

    /* L'area deve essere 0.0f per protezione interna */
    TEST_ASSERT_EQUAL(0.0f, hdiag.last_area_error);
    TEST_ASSERT_EQUAL(MOTOR_HEALTHY, hdiag.health_status);
}

/**
 * @test Verifica l'indipendenza dei buffer durante l'elaborazione.
 * @details Registra dati nel Buffer A, attiva il process, e mentre il process
 * è "in corso" (simulato), scrive dati diversi nel Buffer B.
 */
void test_motor_diag_buffer_isolation(void) {
    motor_diag_init(&hdiag, &config);

    /* 1. Riempiamo Buffer A con dati che portano a FAILURE */
    for (int i = 0; i < M_DIAG_BUF_LEN; i++) {
        motor_diag_record(&hdiag, 10.0f, 0.0f);
    }

    /* Ora active_buf_idx è 1 (B), ma is_buffer_ready è true (A è pronto) */

    /* 2. Scriviamo i primi campioni nel Buffer B (dati "Sani") */
    motor_diag_record(&hdiag, 50.0f, 50.0f);
    motor_diag_record(&hdiag, 50.0f, 50.0f);

    /* 3. Eseguiamo il process: deve analizzare solo il Buffer A */
    motor_diag_process(&hdiag);

    /* Deve aver calcolato l'area del Buffer A (460.0) e non quella di B */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 460.0f, hdiag.last_area_error);
    TEST_ASSERT_EQUAL(MOTOR_FAILURE, hdiag.health_status);
}

/**
 * @test Verifica la capacità di recupero dello stato di salute.
 * @details Passa da uno stato di FAILURE a uno di HEALTHY in due cicli successivi.
 */
void test_motor_diag_recovery_logic(void) {
    motor_diag_init(&hdiag, &config);

    /* Ciclo 1: Generiamo un FAILURE */
    for (int i = 0; i < M_DIAG_BUF_LEN; i++) motor_diag_record(&hdiag, 10.0f, 0.0f);
    motor_diag_process(&hdiag);
    TEST_ASSERT_EQUAL(MOTOR_FAILURE, hdiag.health_status);

    /* Ciclo 2: Inviamo dati corretti (Motore ripartito) */
    for (int i = 0; i < M_DIAG_BUF_LEN; i++) motor_diag_record(&hdiag, 10.0f, 10.0f);
    motor_diag_process(&hdiag);

    /* Lo stato deve tornare HEALTHY */
    TEST_ASSERT_EQUAL(0.0f, hdiag.last_area_error);
    TEST_ASSERT_EQUAL(MOTOR_HEALTHY, hdiag.health_status);
}
