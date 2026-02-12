/**
 * @file test_deadline_watchdog.c
 * @author Gruppo 2
 * @brief Unit Tests per il driver Deadline Watchdog (DWD).
 *
 * @details I test verificano il corretto avvio del timer, la logica di notifica
 * tramite maschere di bit (Check-in dei task) e l'esecuzione della callback
 * di sicurezza in caso di scadenza del timeout (Deadline).
 */

#include "unity.h"
#include <string.h>

/* Mock delle librerie HAL e RTOS */
#include "mock_stm32g4xx_hal.h"
#include "mock_task.h"  /* Per taskENTER_CRITICAL e taskEXIT_CRITICAL */

/* Modulo da testare */
#include "deadline_watchdog.h"

/** @name Oggetti di supporto e Flag di Test
 * @{ */
static Deadline_Watchdog_t dwd;
static TIM_HandleTypeDef htim_dwd;
static TIM_TypeDef timer_instance;
static uint32_t callback_call_count = 0;

/** @brief Callback di test per verificare l'esecuzione della deadline */
void test_callback(void) {
    callback_call_count++;
}
/** @} */

/**
 * @brief Setup eseguito prima di ogni test.
 */
void setUp(void) {
    memset(&dwd, 0, sizeof(Deadline_Watchdog_t));
    memset(&htim_dwd, 0, sizeof(TIM_HandleTypeDef));

    /* Colleghiamo l'istanza hardware all'handle */
    htim_dwd.Instance = &timer_instance;
    callback_call_count = 0;
}

/* =================================================================---------
    TEST INIZIALIZZAZIONE
   ========================================================================= */

/**
 * @test Verifica l'inizializzazione del watchdog e l'avvio del timer con interrupt.
 * @details Il test verifica che il timer venga azzerato e avviato in modalità IT.
 */
void test_DWD_Init_Should_SetupAndStartTimer(void) {
    uint32_t target = 0x07; // 0111 in binario

    /* Aspettative HAL */
    /* Nota: Le macro __HAL_TIM... se non sono state ridefinite come funzioni nel mock,
       potrebbero richiedere un file di supporto. Qui testiamo la funzione Start_IT. */
    HAL_TIM_Base_Start_IT_ExpectAndReturn(&htim_dwd, HAL_OK);

    DWD_Init(&dwd, &htim_dwd, target, test_callback);

    TEST_ASSERT_EQUAL_PTR(&htim_dwd, dwd.htim);
    TEST_ASSERT_EQUAL(target, dwd.target_mask);
    TEST_ASSERT_EQUAL_PTR(test_callback, dwd.on_deadline_cb);
    TEST_ASSERT_EQUAL(0, dwd.current_mask);
}

/* =================================================================---------
    TEST LOGICA DI NOTIFICA (CHECK-IN)
   ========================================================================= */

/**
 * @test Verifica che la notifica aggiorni correttamente la maschera corrente.
 * @details Simula la notifica di un singolo flag e verifica che il counter non venga azzerato
 * finché tutti i task non hanno risposto.
 */
void test_DWD_Notify_Should_UpdateMaskAndNotResetIfIncomplete(void) {
    dwd.target_mask = 0x03; // Task 1 (bit 0) e Task 2 (bit 1)
    dwd.current_mask = 0;
    dwd.htim = &htim_dwd;

    /* Ci aspettiamo l'ingresso in sezione critica FreeRTOS */
    vTaskEnterCritical_Expect();
    vTaskExitCritical_Expect();

    DWD_Notify(&dwd, 0x01); // Notifica Task 1

    TEST_ASSERT_EQUAL(0x01, dwd.current_mask);
    /* HAL_TIM_Base_Start_IT o azzeramenti non dovrebbero essere chiamati qui */
}

/**
 * @test Verifica il reset del watchdog quando tutti i task notificano la loro attività.
 * @details Quando current_mask == target_mask, il counter deve tornare a 0.
 */
void test_DWD_Notify_Should_ResetWatchdogWhenAllTasksNotified(void) {
    dwd.target_mask = 0x03;
    dwd.current_mask = 0x02; // Task 2 ha già notificato
    dwd.htim = &htim_dwd;

    vTaskEnterCritical_Expect();
    vTaskExitCritical_Expect();

    /* Qui la logica interna vede che (0x02 | 0x01) == 0x03 e resetta */
    DWD_Notify(&dwd, 0x01); // Notifica Task 1 (completa il target)

    TEST_ASSERT_EQUAL(0, dwd.current_mask);
}

/**
 * @test Verifica che notifiche multiple dello stesso flag non resettino il watchdog prematuramente.
 */
void test_DWD_Notify_Should_IgnoreRedundantFlags(void) {
    dwd.target_mask = 0x03; // Task A (bit 0) e Task B (bit 1)
    dwd.current_mask = 0;
    dwd.htim = &htim_dwd;

    vTaskEnterCritical_Expect();
    vTaskExitCritical_Expect();
    DWD_Notify(&dwd, 0x01); // Prima notifica Task A

    vTaskEnterCritical_Expect();
    vTaskExitCritical_Expect();
    DWD_Notify(&dwd, 0x01); // Seconda notifica Task A (ridondante)

    TEST_ASSERT_EQUAL(0x01, dwd.current_mask); // La maschera deve essere ancora 0x01
}

/**
 * @test Verifica la corretta sequenza di reset quando l'ultimo task notifica.
 */
void test_DWD_Notify_Should_ResetCounterPreciselyOnLastFlag(void) {
    dwd.target_mask = 0x04; // Task C (bit 2)
    dwd.current_mask = 0;
    dwd.htim = &htim_dwd;

    vTaskEnterCritical_Expect();
    // Qui il driver eseguirà __HAL_TIM_SET_COUNTER(hwd->htim, 0)
    vTaskExitCritical_Expect();

    DWD_Notify(&dwd, 0x04);

    TEST_ASSERT_EQUAL(0, dwd.current_mask);
}

/* =================================================================---------
    TEST GESTIONE INTERRUPT (DEADLINE EXPIRED)
   ========================================================================= */

/**
 * @test Verifica che l'interrupt del timer corretto scateni la callback di deadline.
 * @details Simula l'evento di timeout del timer hardware.
 */
void test_DWD_CheckStatus_Should_CallCallbackAndStopTimerOnMatch(void) {
    dwd.htim = &htim_dwd;
    dwd.on_deadline_cb = test_callback;

    /* Mock della fermata del timer per sicurezza */
    HAL_TIM_Base_Stop_IT_ExpectAndReturn(&htim_dwd, HAL_OK);

    /* Chiamiamo il check status passando lo stesso timer */
    DWD_CheckStatus(&dwd, &htim_dwd);

    TEST_ASSERT_EQUAL(1, callback_call_count);
}

/**
 * @test Verifica che l'interrupt di un timer diverso venga ignorato.
 * @details Il watchdog deve agire solo se l'handle dell'IRQ corrisponde a quello registrato.
 */
void test_DWD_CheckStatus_Should_IgnoreOtherTimers(void) {
    TIM_HandleTypeDef htim_altro;
    TIM_TypeDef instance_altra;
    htim_altro.Instance = &instance_altra;

    dwd.htim = &htim_dwd;
    dwd.on_deadline_cb = test_callback;

    /* Non ci aspettiamo chiamate a HAL_TIM_Base_Stop_IT */

    DWD_CheckStatus(&dwd, &htim_altro);

    TEST_ASSERT_EQUAL(0, callback_call_count);
}

/**
 * @test Verifica la robustezza nel caso la callback sia NULL.
 */
void test_DWD_CheckStatus_Should_NotCrashIfCallbackIsNull(void) {
    dwd.htim = &htim_dwd;
    dwd.on_deadline_cb = NULL;

    HAL_TIM_Base_Stop_IT_ExpectAndReturn(&htim_dwd, HAL_OK);

    /* Non deve crashare nonostante l'assenza di callback */
    DWD_CheckStatus(&dwd, &htim_dwd);
}
