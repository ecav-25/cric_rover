// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    dwt_delay.h
 * @author  Gruppo 2
 * @brief   Driver per ritardi ad alta precisione basati su periferica DWT.
 * @details Questo modulo utilizza il registro CYCCNT (Cycle Counter) dell'unità
 * Data Watchpoint and Trace (DWT) dei core ARM Cortex-M per implementare
 * ritardi bloccanti con risoluzione al microsecondo.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef DWT_DELAY_H
#define DWT_DELAY_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Codici di stato per le operazioni DWT.
 */
typedef enum {
    DWT_OK = 0,             /**< Operazione eseguita con successo. */
    DWT_ERR_UNSUPPORTED,    /**< DWT non supportato o non presente. */
    DWT_ERR_NOT_READY       /**< DWT non inizializzato. */
} dwt_status_t;

/**
 * @brief Abilita il conteggio dei cicli DWT->CYCCNT.
 * @details Deve essere chiamata una sola volta all'avvio del sistema (post clock config).
 * @return dwt_status_t DWT_OK se l'inizializzazione ha successo.
 */
dwt_status_t DWT_Delay_Init(void);

/**
 * @brief Esegue un ritardo bloccante in microsecondi.
 * @param us Numero di microsecondi di attesa.
 * @return dwt_status_t DWT_OK se il delay è stato eseguito, errore se DWT non init.
 */
dwt_status_t DWT_Delay_us(uint32_t us);

/**
 * @brief Esegue un ritardo bloccante in millisecondi.
 * @details Utilizza il DWT invece di HAL_Delay per evitare dipendenze dal SysTick.
 * @param ms Numero di millisecondi di attesa.
 * @return dwt_status_t DWT_OK se il delay è stato eseguito.
 */
dwt_status_t DWT_Delay_ms(uint32_t ms);

/**
 * @brief Verifica se il contatore di cicli è abilitato e funzionante.
 * @return uint8_t 1 se il DWT è pronto, 0 altrimenti.
 */
uint8_t DWT_Delay_IsReady(void);

#ifdef __cplusplus
}
#endif

#endif /* DWT_DELAY_H */
