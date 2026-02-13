// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file led.h
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 * @author Gruppo 2
 * @brief Gestione dei LED di stato onboard tramite GPIO.

 * * Questo modulo fornisce un'astrazione per il controllo di LED standard (ON/OFF)
 * e supporta una gestione a stati (step) per implementare lampeggi o sequenze
 * senza bloccare l'esecuzione del codice principale.
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio.h"
#include "Board1_types.h"

/** @name Parametri di inizializzazione LED
 * @{ */
#define LED_INIT_STATE_OFF  (0)   /**< LED inizialmente spento */
#define LED_INIT_STATE_ON   (1)   /**< LED inizialmente acceso */
#define LED_TOGGLE_PERIOD   (0)   /**< Periodo di toggle di default */
/** @} */

/** @name Codici di errore
 * @{ */
#define LED_ERR             (-1)  /**< Operazione fallita */
#define LED_OK              (0)   /**< Operazione riuscita */
/** @} */

/** @brief Alias per lo stato logico del LED (es. lampeggio, fisso) definito in Board1_types. */
typedef LED_TYPE led_state_t;

/**
 * @brief Identificativi dei LED fisici controllati.
 */
typedef enum {
    LED_RED = 0,    /**< LED di segnalazione errore (Rosso) */
    LED_WHITE,      /**< LED di stato generale (Bianco) */
    LED_COUNT       /**< Numero totale di LED */
} controlled_led_t;

/**
 * @brief Struttura di gestione dell'oggetto LED.
 */
struct led_s
{
    GPIO_TypeDef* port[LED_COUNT];       /**< Porte GPIO (es. GPIOA, GPIOB) */
    uint16_t pin[LED_COUNT];             /**< Pin GPIO (es. GPIO_PIN_5) */

    led_state_t state;                   /**< Stato logico attuale (da Board1_types) */
    uint8_t step;                        /**< Contatore per la gestione interna delle sequenze */

    uint8_t pinState[LED_COUNT];         /**< Stato fisico attuale dei pin (0 o 1) */
    uint8_t first_entry;                 /**< Flag per la gestione dell'inizializzazione negli stati */
};

/** @brief Tipo opaco per la struttura LED. */
typedef struct led_s led_t;

/** @brief Alias per lo stato fisico del pin (GPIO_PIN_SET/RESET). */
typedef GPIO_PinState pin_state_t;

/* ================== API ================== */

/**
 * @brief Inizializza l'oggetto LED e configura lo stato iniziale dei pin.
 * * @param led Puntatore alla struttura dati led_t.
 * @param GPIOx Array di puntatori alle porte GPIO.
 * @param GPIO_Pin Array dei numeri dei pin.
 * @param init_pin_state Array degli stati iniziali desiderati.
 * @return int8_t LED_OK se l'inizializzazione ha successo.
 */
int8_t led_init(led_t* led, GPIO_TypeDef* const GPIOx[LED_COUNT], const uint16_t GPIO_Pin[LED_COUNT], const pin_state_t init_pin_state[LED_COUNT]);

/**
 * @brief Scrive uno stato fisico (ON/OFF) su un LED specifico.
 * * @param led Puntatore all'oggetto LED.
 * @param status Stato fisico (GPIO_PIN_SET o RESET).
 * @param controlled_led Identificativo del LED (LED_RED o LED_WHITE).
 * @return int8_t LED_OK se l'operazione ha successo.
 */
int8_t led_write(led_t* led, pin_state_t status, controlled_led_t controlled_led);

/**
 * @brief Accende un LED specifico.
 */
int8_t led_on(led_t* led, controlled_led_t controlled_led);

/**
 * @brief Spegne un LED specifico.
 */
int8_t led_off(led_t* led, controlled_led_t controlled_led);

/**
 * @brief Esegue la logica di aggiornamento per gli stati del LED.
 * * Questa funzione gestisce il comportamento del LED (fisso, lampeggiante, ecc.)
 * in base allo stato passato come parametro.
 * * @param led Puntatore all'oggetto LED.
 * @param state Stato logico desiderato.
 * @return int8_t LED_OK se l'aggiornamento è avvenuto correttamente.
 */
int8_t led_step(led_t* led, led_state_t state);

#endif /* INC_LED_H_ */
