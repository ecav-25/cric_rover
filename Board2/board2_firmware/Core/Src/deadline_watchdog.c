// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    deadline_watchdog.c
 * @author  Gruppo 2
 * @brief   Implementazione del driver Deadline Watchdog per il monitoraggio real-time.
 * @details Questo modulo utilizza un timer hardware per garantire che un insieme di task
 * completi la propria esecuzione entro una finestra temporale definita.
 * Utilizza le primitive di FreeRTOS per proteggere l'accesso concorrente allo stato interno.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "deadline_watchdog.h"
#include "FreeRTOS.h"
#include "task.h"

/* ========================================================================== */
/* API FUNCTIONS                                                              */
/* ========================================================================== */

/**
 * @brief Inizializza il Deadline Watchdog e avvia il timer di monitoraggio.
 * @param hwd Puntatore all'istanza della struttura Deadline_Watchdog_t.
 * @param htim Puntatore all'handle del timer hardware.
 * @param target_mask Maschera di bit che rappresenta l'insieme dei task da monitorare.
 * @param callback Funzione da invocare in caso di violazione della deadline.
 */
void DWD_Init(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim, uint32_t target_mask, DWD_Callback_t callback)
{
    hwd->htim = htim;
    hwd->target_mask = target_mask;
    hwd->on_deadline_cb = callback;

    hwd->current_mask = 0;

    __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    __HAL_TIM_CLEAR_FLAG(hwd->htim, TIM_FLAG_UPDATE);

    HAL_TIM_Base_Start_IT(hwd->htim);
}

/**
 * @brief Notifica al watchdog che un task ha completato il suo ciclo.
 * @details Questa funzione è thread-safe: utilizza una sezione critica per aggiornare
 * la maschera corrente.
 * Se tutti i task previsti (target_mask) hanno notificato,il timer hardware viene resettato
 * @param hwd Puntatore all'istanza del watchdog.
 * @param flag Bitmask univoca del task chiamante.
 */
void DWD_Notify(Deadline_Watchdog_t *hwd, uint32_t flag)
{
    taskENTER_CRITICAL();

    hwd->current_mask |= flag;


    if (hwd->current_mask == hwd->target_mask)
    {
        hwd->current_mask = 0;
        __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    }

    taskEXIT_CRITICAL();
}

/**
 * @brief Verifica se l'interrupt del timer corrisponde al watchdog e gestisce il timeout.
 * @note Da chiamare all'interno della callback HAL `HAL_TIM_PeriodElapsedCallback`.
 * Se questa funzione viene eseguita per il timer associato, significa che la deadline è stata violata.
 * @param hwd Puntatore all'istanza del watchdog.
 * @param htim_irq Handle del timer che ha generato l'interrupt.
 */
void DWD_CheckStatus(Deadline_Watchdog_t *hwd, const TIM_HandleTypeDef *htim_irq)
{
    if (htim_irq->Instance == hwd->htim->Instance)
    {
        HAL_TIM_Base_Stop_IT(hwd->htim);

        if (hwd->on_deadline_cb != NULL)
        {
            hwd->on_deadline_cb();
        }
    }
}
