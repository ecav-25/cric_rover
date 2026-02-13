// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    deadline_watchdog.c
 * @author  Gruppo 2
 * @brief   Implementazione del driver Deadline Watchdog per il monitoraggio real-time dei task.
 * @details Questo modulo utilizza un timer hardware per garantire che un gruppo di task
 * completi la propria esecuzione entro una finestra temporale prestabilita.
 * Se i task non notificano il watchdog in tempo, viene invocata una callback di errore.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "deadline_watchdog.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Inizializza il Deadline Watchdog e avvia il timer hardware associato.
 * @param hwd Puntatore alla struttura di controllo del Watchdog.
 * @param htim Handle del timer hardware STM32 da utilizzare per il timeout.
 * @param target_mask Maschera di bit che rappresenta l'insieme dei task da monitorare.
 * @param callback Funzione da eseguire in caso di violazione della deadline (timeout scaduto).
 */
void DWD_Init(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim, uint32_t target_mask, DWD_Callback_t callback)
{
    hwd->htim = htim;
    hwd->target_mask = target_mask;
    hwd->on_deadline_cb = callback;

    hwd->current_mask = 0;

    /* Azzera il contatore e pulisce eventuali flag pendenti prima dell'avvio */
    __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    __HAL_TIM_CLEAR_FLAG(hwd->htim, TIM_FLAG_UPDATE);

    /* Avvia il timer in modalità interrupt */
    HAL_TIM_Base_Start_IT(hwd->htim);
}

/**
 * @brief Notifica l'attività di un task specifico al watchdog.
 * @details Questa funzione deve essere chiamata ciclicamente dai task monitorati.
 * Viene utilizzata una sezione critica per garantire l'atomicità dell'aggiornamento della maschera.
 * Se tutti i task previsti (target_mask) hanno notificato, il timer hardware viene resettato.
 * @param hwd Puntatore alla struttura di controllo.
 * @param flag Bitmask univoca del task che sta effettuando la notifica.
 */
void DWD_Notify(Deadline_Watchdog_t *hwd, uint32_t flag)
{
    taskENTER_CRITICAL();

    hwd->current_mask |= flag;

    /* Verifica se tutti i task hanno notificato */
    if (hwd->current_mask == hwd->target_mask)
    {
        /* Reset della maschera corrente e del timer hardware (kick the dog) */
        hwd->current_mask = 0;
        __HAL_TIM_SET_COUNTER(hwd->htim, 0);
    }

    taskEXIT_CRITICAL();
}

/**
 * @brief Verifica lo stato del watchdog all'interno della ISR del timer.
 * @note Questa funzione va inserita nella callback `HAL_TIM_PeriodElapsedCallback`.
 * Se viene eseguita, significa che il timer è andato in overflow prima che tutti
 * i task abbiano notificato, indicando una violazione della deadline.
 * @param hwd Puntatore alla struttura di controllo.
 * @param htim_irq Handle del timer che ha generato l'interrupt.
 */
void DWD_CheckStatus(Deadline_Watchdog_t *hwd, const TIM_HandleTypeDef *htim_irq)
{
    /* Verifica che l'interrupt provenga dal timer associato a questo watchdog */
    if (htim_irq->Instance == hwd->htim->Instance)
    {
        /* Ferma il timer per evitare chiamate multiple alla callback di errore */
        (void)HAL_TIM_Base_Stop_IT(hwd->htim);

        /* Invoca la routine di gestione dell'errore se definita */
        if (hwd->on_deadline_cb != NULL)
        {
            hwd->on_deadline_cb();
        }
    }
}
