/**
 * @file led_stripes.h
 * @author Gruppo 2
 * @brief Gestione di strisce LED RGB WS2812B tramite PWM e DMA.
 * * Questo modulo permette il controllo individuale e collettivo di pixel RGB,
 * supportando animazioni per i segnali posteriori (freni, frecce) e rendering
 * efficiente tramite periferiche hardware.
 */

#ifndef INC_LED_STRIPES_H_
#define INC_LED_STRIPES_H_

/** @brief Numero totale di LED presenti sulla striscia hardware. */
#define NUM_PIXELS (21U)

#include "stm32g474xx.h"
#include "tim.h"
#include "dma.h"
#include "Board1_types.h"

/**
 * @brief Codici di stato per le operazioni del modulo LED.
 */
typedef enum {
    LED_STRIPE_OK = 0,      /**< Operazione riuscita */
    LED_STRIPE_ERR,         /**< Errore generico hardware */
    LED_BUSY,               /**< Periferica occupata (trasmissione DMA in corso) */
    LED_INVALID_PARAM       /**< Parametri di input fuori range */
} led_status_t;

/**
 * @brief Struttura di configurazione hardware per la striscia LED.
 */
typedef struct {
    TIM_HandleTypeDef *htim;      /**< Puntatore al Timer per la generazione del segnale PWM */
    DMA_HandleTypeDef *hdma;      /**< Puntatore al canale DMA per il trasferimento dati */
    uint32_t tim_channel;         /**< Canale del timer */
    uint16_t pwm_hi;              /**< Valore duty cycle per il bit logico '1' */
    uint16_t pwm_lo;              /**< Valore duty cycle per il bit logico '0' */
    uint8_t scale_g;              /**< Fattore di scala/luminosità per il verde (0-255) */
    uint8_t scale_r;              /**< Fattore di scala/luminosità per il rosso (0-255) */
    uint8_t scale_b;              /**< Fattore di scala/luminosità per il blu (0-255) */
    uint8_t reset_halves;         /**< Numero di impulsi di reset necessari (multipli di 1.25us) */
} led_stripes_config_t;

/* ================== API DI BASE ================== */

/**
 * @brief Inizializza la periferica e le strutture dati per i LED.
 * @param cfg Puntatore alla configurazione hardware.
 * @return led_status_t Esito dell'inizializzazione.
 */
led_status_t led_stripe_init(const led_stripes_config_t *cfg);

/**
 * @brief Imposta il colore RGB di un singolo pixel nel buffer.
 * @note Richiede la chiamata a led_render() per aggiornare fisicamente i LED.
 * @param index Indice del LED (0 a NUM_PIXELS-1).
 * @param r Intensità Rosso (0-255).
 * @param g Intensità Verde (0-255).
 * @param b Intensità Blu (0-255).
 */
void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Imposta lo stesso colore su tutti i LED della striscia.
 * @param r Intensità Rosso.
 * @param g Intensità Verde.
 * @param b Intensità Blu.
 */
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Imposta un colore su un intervallo specifico di LED.
 * @param start Indice iniziale.
 * @param end Indice finale.
 * @param r,g,b Componenti colore.
 */
void led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Trasmette i dati dal buffer ai LED tramite DMA.
 * @details Questa è l'unica funzione che genera output fisico sulla striscia.
 */
void led_render(void);

/* ================== API ANIMAZIONI ================== */

/**
 * @brief Esegue un passo dell'animazione per la striscia led installata sul coperchio del rover.
 * @param animation Tipo di animazione definita in Board1_types.h.
 */
void rear_led_step(REAR_LED_TYPE animation);

/**
 * @brief Esegue un passo dell'animazione per la targa posteriore.
 * @param animation Tipo di segnale definito in Board1_types.h.
 */
void rear_sign_step(REAR_SIGN_TYPE animation);

#endif /* INC_LED_STRIPES_H_ */
