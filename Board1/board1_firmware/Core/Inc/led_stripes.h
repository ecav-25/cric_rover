// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file led_stripes.h
 * @author Gruppo 2
 * @brief Driver per la gestione delle strisce LED (fanaleria e targa).
 *
 * Questo modulo implementa il controllo della striscia LED utilizzando periferiche
 * PWM e DMA. Gestisce sia la fanaleria posteriore dinamica che la retroilluminazione
 * della targa del rover.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_LED_STRIPES_H_
#define INC_LED_STRIPES_H_

#include "stm32g474xx.h"
#include "tim.h"
#include "dma.h"
#include "Board1_types.h"

/** @brief Numero totale di LED (pixel) presenti nella striscia. */
#define NUM_PIXELS (21)

/**
 * @enum led_status_t
 * @brief Codici di stato per le operazioni del modulo LED.
 */
typedef enum {
    LED_STRIPE_OK = 0,      /**< Operazione completata con successo. */
    LED_STRIPE_ERR,         /**< Errore interno o hardware. */
    LED_BUSY,               /**< Periferica occupata (trasmissione DMA in corso). */
    LED_INVALID_PARAM       /**< Parametri di input fuori range. */
} led_status_t;

/**
 * @struct led_stripes_config_t
 * @brief Configurazione hardware e parametri di scaling per la striscia LED.
 */
typedef struct {
    TIM_HandleTypeDef *htim;   /**< Handle del timer per la generazione PWM. */
    DMA_HandleTypeDef *hdma;   /**< Handle del DMA per il trasferimento dati. */
    uint32_t tim_channel;      /**< Canale del timer associato  */

    uint16_t pwm_hi;           /**< Duty cycle per rappresentare il bit logico '1'. */
    uint16_t pwm_lo;           /**< Duty cycle per rappresentare il bit logico '0'. */

    /* scaling (0xFF = no scaling) */
    uint8_t scale_g;           /**< Fattore di scala per la luminosità del Verde (0-255). */
    uint8_t scale_r;           /**< Fattore di scala per la luminosità del Rosso (0-255). */
    uint8_t scale_b;           /**< Fattore di scala per la luminosità del Blu (0-255). */

    /** @brief Numero di mezze-trasmissioni di reset (buffer vuoto) necessarie. */
    uint8_t reset_halves;

} led_stripes_config_t;

/* ========================================================================== */
/* API                                                                        */
/* ========================================================================== */

/**
 * @brief Inizializza il driver LED con la configurazione specificata.
 * @param[in] cfg Puntatore alla struttura di configurazione.
 * @return LED_STRIPE_OK se l'inizializzazione ha successo.
 */
led_status_t led_stripe_init(const led_stripes_config_t *cfg);

/**
 * @brief Esegue un passo dell'animazione per i LED posteriori.
 * @param[in] animation Tipo di animazione richiesta.
 * @return Stato dell'operazione.
 */
led_status_t rear_led_step(REAR_LED_TYPE animation);

/**
 * @brief Esegue un passo dell'animazione per la retroilluminazione della targa.
 * @param[in] animation Tipo di effetto per la targa.
 * @return Stato dell'operazione.
 */
led_status_t rear_sign_step(REAR_SIGN_TYPE animation);

/**
 * @brief Imposta il colore RGB per un singolo LED.
 * @param[in] index Indice del LED (0..NUM_PIXELS-1).
 * @param[in] r Componente Rosso (0..255).
 * @param[in] g Componente Verde (0..255).
 * @param[in] b Componente Blu (0..255).
 * @return LED_STRIPE_OK o LED_INVALID_PARAM se l'indice è errato.
 */
led_status_t led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Imposta lo stesso colore RGB per tutti i LED della striscia.
 * @param[in] r Rosso.
 * @param[in] g Verde.
 * @param[in] b Blu.
 * @return Stato dell'operazione.
 */
led_status_t led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Avvia la trasmissione dei dati via DMA per aggiornare i LED fisici.
 * @details Applica le modifiche ai colori dei LED della fanaleria e della targa.
 * @return LED_STRIPE_OK se la trasmissione è avviata, LED_BUSY se il DMA è già attivo.
 */
led_status_t led_render(void);

/**
 * @brief Imposta il colore RGB per un intervallo contiguo di LED [start, end].
 *
 * @param[in] start Primo indice LED (incluso).
 * @param[in] end   Ultimo indice LED (incluso).
 * @param[in] r     Rosso (0..255).
 * @param[in] g     Verde (0..255).
 * @param[in] b     Blu   (0..255).
 * @return LED_STRIPE_OK in caso di successo, LED_STRIPE_ERR se il range non è valido.
 */
led_status_t led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b);

#endif /* INC_LED_STRIPES_H_ */
