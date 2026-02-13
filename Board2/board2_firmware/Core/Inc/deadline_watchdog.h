// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    deadline_watchdog.h
 * @author  Gruppo 2
 * @brief   Driver per Deadline Watchdog basato su Timer Hardware.
 * @details Questo modulo definisce le strutture e le interfacce per monitorare
 * il rispetto delle tempistiche real-time dei task. Utilizza un timer hardware
 * dedicato che deve essere resettato (tramite la notifica di tutti i task)
 * prima che raggiunga l'overflow.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef DEADLINE_WATCHDOG_H
#define DEADLINE_WATCHDOG_H

#include "main.h"

/**
 * @brief Definizione del tipo funzione per la callback di errore.
 * @details Questa funzione viene invocata in contesto ISR se il timer scade.
 */
typedef void (*DWD_Callback_t)(void);

/**
 * @brief Struttura di controllo per l'istanza del Deadline Watchdog.
 */
typedef struct {
    TIM_HandleTypeDef *htim;        /**< Puntatore all'handle del timer hardware STM32. */
    uint32_t target_mask;           /**< Maschera di bit rappresentante l'insieme completo dei task da monitorare. */
    volatile uint32_t current_mask; /**< Maschera accumulata dei task che hanno notificato nel ciclo corrente. */
    DWD_Callback_t on_deadline_cb;  /**< Puntatore alla funzione da eseguire in caso di violazione della deadline (timeout). */
} Deadline_Watchdog_t;

/**
 * @brief Inizializza e avvia il Deadline Watchdog.
 * @details Configura la struttura interna, azzera i contatori e avvia il timer hardware in modalità interrupt.
 * @param hwd Puntatore all'istanza della struttura Deadline_Watchdog_t.
 * @param htim Puntatore all'handle del timer hardware già configurato.
 * @param target_mask La maschera di bit (OR bitwise) di tutti i flag che il sistema si aspetta di ricevere.
 * @param callback La funzione di sicurezza da eseguire se il timer scade.
 */
void DWD_Init(Deadline_Watchdog_t *hwd, TIM_HandleTypeDef *htim, uint32_t target_mask, DWD_Callback_t callback);

/**
 * @brief Notifica al watchdog che una specifica parte del sistema (task) è viva.
 * @details Quando chiamata, imposta il bit corrispondente nella maschera corrente.
 * Se la maschera corrente eguaglia la maschera target (tutti i task hanno risposto),
 * il timer hardware viene resettato.
 * @param hwd Puntatore all'istanza del watchdog.
 * @param flag La flag univoca del chiamante
 */
void DWD_Notify(Deadline_Watchdog_t *hwd, uint32_t flag);

/**
 * @brief Gestisce l'interrupt del timer per verificare la scadenza.
 * @note Questa funzione deve essere chiamata all'interno della callback HAL: `HAL_TIM_PeriodElapsedCallback`.
 * @param hwd Puntatore all'istanza del watchdog.
 * @param htim_irq Puntatore all'handle del timer che ha generato l'interrupt corrente
 */
void DWD_CheckStatus(Deadline_Watchdog_t *hwd, const TIM_HandleTypeDef *htim_irq);

#endif /* DEADLINE_WATCHDOG_H */
