// SPDX-License-Identifier: GPL-3.0-or-later
/**
 ******************************************************************************
 * @file    deadline_watchdog.h
 * * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 * @brief   Driver per Deadline Watchdog basato su Timer Hardware.
 ******************************************************************************
 */

#ifndef DEADLINE_WATCHDOG_H
#define DEADLINE_WATCHDOG_H

#include "main.h"


typedef void (*DWD_Callback_t)(void);

/**
 * @brief Struttura "Oggetto" del Deadline Watchdog.
 */
typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t target_mask;
	volatile uint32_t current_mask;
	DWD_Callback_t on_deadline_cb;
} Deadline_Watchdog_t;



/**
 * @brief Inizializza e avvia il Deadline Watchdog.
 * @param hwd Puntatore all'istanza della struttura Deadline_Watchdog_t.
 * @param htim Puntatore all'handle del timer hardware (es. &htim7).
 * @param target_mask La somma (OR) di tutte le flag che devono essere notificate.
 * @param callback La funzione da eseguire se il timer scade.
 */
void DWD_Init(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim, uint32_t target_mask, DWD_Callback_t callback);

/**
 * @brief Notifica al watchdog che una specifica parte del sistema è viva.
 * @param hwd Puntatore all'istanza del watchdog.
 * @param flag La flag univoca del chiamante (es. 1<<0).
 */
void DWD_Notify(Deadline_Watchdog_t *hwd, uint32_t flag);

/**
 * @brief Gestisce l'interrupt del timer. Da chiamare in HAL_TIM_PeriodElapsedCallback.
 * @param hwd Puntatore all'istanza del watchdog.
 * @param htim_irq Puntatore al timer che ha generato l'interrupt corrente.
 */
void DWD_CheckStatus(Deadline_Watchdog_t *hwd, const TIM_HandleTypeDef *htim_irq);

#endif /* DEADLINE_WATCHDOG_H */
