// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hcsr04.h
 * @author  Gruppo 2
 * @brief   Driver per il sensore a ultrasuoni HC-SR04 (misura distanza).
 * @details Questo driver utilizza un Timer in modalità Input Capture (IC) per misurare
 * la durata dell'impulso di ECHO, che è proporzionale alla distanza dell'ostacolo.
 * La generazione del segnale di TRIGGER è gestita via software (GPIO Bit-Banging)
 * tramite una funzione di delay esterna fornita in configurazione.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef HCSR04_H
#define HCSR04_H

/** @brief Distanza massima teorica misurabile in cm (datasheet HC-SR04). */
#define HCSR04_MAX_DISTANCE 400U

#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Stati interni della macchina a stati del driver (gestione ISR).
 */
typedef enum {
    HCSR04_IDLE = 0,    /**< Sensore a riposo, pronto per nuova misura. */
    HCSR04_WAIT_RISE,   /**< Trigger inviato, in attesa del fronte di salita su ECHO. */
    HCSR04_WAIT_FALL    /**< Fronte di salita rilevato, in attesa del fronte di discesa su ECHO. */
} hcsr04_state_t;

/**
 * @brief Codici di stato e errore del driver.
 */
typedef enum {
    HCSR04_OK = 0,          /**< Operazione completata con successo. */
    HCSR04_ERR_BAD_PARAM,   /**< Parametri di configurazione non validi (es. puntatori NULL). */
    HCSR04_ERR_BUSY,        /**< Una misurazione è già in corso. */
    HCSR04_ERR_TIMEOUT,     /**< Timeout: nessun segnale di eco ricevuto entro il tempo limite. */
    HCSR04_ERR_NOT_READY    /**< Dato non pronto o inizializzazione mancante. */
} hcsr04_status_t;

/**
 * @brief Definizione del tipo funzione per il ritardo in microsecondi.
 * @details Necessaria per generare l'impulso di Trigger di 10us.
 * Può essere implementata tramite DWT, Timer o busy-loop calibrato.
 * @param us Microsecondi di attesa.
 * @return uint8_t 1 se successo, 0 errore.
 */
typedef uint8_t (*hcsr04_delay_us_f)(uint32_t us);

/**
 * @brief Struttura di configurazione hardware e parametri del sensore.
 */
typedef struct {
    TIM_HandleTypeDef *htim;       /**< Handle del Timer utilizzato per Input Capture. */
    uint32_t channel;              /**< Canale del Timer (es. TIM_CHANNEL_1). */

    GPIO_TypeDef *trig_port;       /**< Porta GPIO del pin TRIGGER. */
    uint16_t trig_pin;             /**< Pin GPIO del pin TRIGGER. */

    uint32_t timer_hz;             /**< Frequenza del contatore del timer in Hz (fondamentale per calcolo tempo). */
    hcsr04_delay_us_f delay_us;    /**< Puntatore alla funzione di delay (obbligatoria). */
} hcsr04_cfg_t;

/**
 * @brief Handle principale per l'istanza del sensore HC-SR04.
 * @note Contiene sia la configurazione che lo stato runtime volatile aggiornato in ISR.
 */
typedef struct {
    hcsr04_cfg_t cfg;                    /**< Copia locale della configurazione. */

    volatile hcsr04_state_t state;       /**< Stato corrente della FSM (modificato in ISR). */
    volatile uint32_t t_rise;            /**< Timestamp fronte di salita (tick timer). */
    volatile uint32_t t_fall;            /**< Timestamp fronte di discesa (tick timer). */
    volatile uint32_t start_ms;          /**< Timestamp avvio misura (per gestione timeout SW). */

    volatile uint16_t last_cm;           /**< Ultima distanza calcolata in cm. */
    volatile hcsr04_status_t last_status;/**< Esito dell'ultima misurazione. */
    volatile bool ready;                 /**< Flag: true se un nuovo dato è disponibile per la lettura. */
} hcsr04_t;

/* ========================================================================== */
/* PUBLIC API                                                                 */
/* ========================================================================== */

/**
 * @brief Inizializza il driver HC-SR04.
 * @param s Puntatore all'handle del sensore.
 * @param cfg Puntatore alla struttura di configurazione.
 * @return hcsr04_status_t HCSR04_OK se inizializzazione riuscita.
 */
hcsr04_status_t HCSR04_Init(hcsr04_t *s, const hcsr04_cfg_t *cfg);

/**
 * @brief Avvia una nuova misurazione di distanza.
 * @details Genera l'impulso di Trigger (10us) e abilita l'interrupt Input Capture
 * per misurare la risposta sul pin Echo.
 * @param s Puntatore all'handle del sensore.
 * @return hcsr04_status_t HCSR04_OK se il trigger è stato inviato.
 */
hcsr04_status_t HCSR04_Start(hcsr04_t *s);

/**
 * @brief Callback per la gestione dell'Input Capture (da chiamare in ISR).
 * @note Deve essere inserita nella funzione `HAL_TIM_IC_CaptureCallback` dell'utente.
 * Gestisce la logica di calcolo dei tempi di volo (Rise/Fall).
 * @param htim Handle del timer che ha generato l'interrupt.
 */
void HCSR04_IC_Callback(TIM_HandleTypeDef *htim);

/**
 * @brief Restituisce l'ultima distanza misurata in centimetri.
 * @param s Puntatore all'handle del sensore.
 * @param cm Puntatore alla variabile dove salvare la distanza.
 * @return hcsr04_status_t HCSR04_OK se il dato è valido e aggiornato.
 */
hcsr04_status_t HCSR04_GetDistanceCm(hcsr04_t *s, uint16_t *cm);

#ifdef __cplusplus
}
#endif

#endif /* HCSR04_H */
