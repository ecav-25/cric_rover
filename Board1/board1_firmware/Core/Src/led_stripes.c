// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    led_stripes.c
 * @author  Gruppo 2
 * @brief   Driver per la gestione di strisce LED RGB (WS2812/SK6812) tramite PWM e DMA.
 * @details Implementa un meccanismo di double-buffering per inviare i dati ai LED
 * riducendo l'uso della RAM. Include macchine a stati per gestire animazioni
 * (frecce, stop, luci di posizione) su diverse sezioni della striscia.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "led_stripes.h"
#include <stdio.h>
#include <string.h>

/** @brief Numero di byte per pixel (R, G, B). */
#define NUM_BPP (3)
/** @brief Dimensione totale del buffer colori in byte. */
#define NUM_BYTES (NUM_BPP * NUM_PIXELS)
/** @brief Lunghezza del buffer di scrittura DMA (2 pixel completi per ping-pong buffering). */
#define WR_BUF_LEN (NUM_BPP * 8 * 2)

/** @brief Indice iniziale della sezione targa posteriore. */
#define LED_REAR_SIGN_START 0
/** @brief Indice finale della sezione targa posteriore. */
#define LED_REAR_SIGN_END 7

/** @brief Indice iniziale della sezione luci posteriori. */
#define LED_REAR_LED_START 8
/** @brief Indice finale della sezione luci posteriori. */
#define LED_REAR_LED_END 20

/** @brief Step di spegnimento per animazione freccia. */
#define ARROW_OFF_STEPS  5
/** @brief Step di accensione completa per animazione freccia. */
#define ARROW_ON_STEPS   5

/** @brief Periodo totale (in step) per il ciclo delle luci di emergenza. */
#define EMERGENCY_PERIOD 16

/**
 * @brief Struttura per la gestione del bus hardware dei LED.
 */
typedef struct {
  led_stripes_config_t cfg;      /**< Configurazione hardware (Timer, DMA, Canale). */
  uint8_t rgb_arr[NUM_BYTES];    /**< Array contenente i valori RGB per tutti i pixel. */
  uint8_t wr_buf[WR_BUF_LEN];    /**< Buffer circolare per il trasferimento DMA PWM. */
  volatile uint_fast8_t wr_buf_p;/**< Puntatore corrente al pixel in elaborazione nel buffer DMA. */
  volatile uint8_t dirty;        /**< Flag che indica se ci sono modifiche pendenti da renderizzare. */
} led_bus_t;

/**
 * @brief Struttura per la gestione di una sezione logica della striscia LED.
 */
typedef struct {
  led_bus_t *bus;          /**< Puntatore al bus hardware condiviso. */
  uint16_t start;          /**< Indice LED iniziale della sezione. */
  uint16_t end;            /**< Indice LED finale della sezione. */
  uint8_t last_animation;  /**< Ultima animazione eseguita (per rilevare cambi di stato). */
  uint16_t step;           /**< Contatore interno per lo stato dell'animazione corrente. */
} led_part_t;

/** @brief Seed per il generatore pseudo-casuale (LFSR). */
static uint16_t lfsr = 0xACE1u;

/* Istanze statiche dei driver */
static led_bus_t g_led_bus;    /**< Istanza unica del bus LED fisico. */
static led_part_t rear_led;    /**< Istanza logica per le luci posteriori. */
static led_part_t rear_sign;   /**< Istanza logica per la targa posteriore. */

/* Prototipi funzioni statiche animazioni luci posteriori */
static led_status_t rear_led_off(void);
static led_status_t rear_led_anim_stop(void);
static led_status_t rear_led_anim_backlights(void);
static led_status_t rear_led_anim_arrow_left(void);
static led_status_t rear_led_anim_arrow_right(void);
static led_status_t rear_led_anim_backward(void);
static led_status_t rear_led_special_lights(void);
static led_status_t rear_led_emercency_lights(void);
static led_status_t rear_led_degraded_lights(void);

/* Prototipi funzioni statiche animazioni targa posteriore */
static led_status_t rear_sign_off(void);
static led_status_t rear_sign_white(void);
static led_status_t rear_sign_orange(void);
static led_status_t rear_sign_green(void);
static led_status_t rear_sign_red(void);
static led_status_t rear_sign_yellow(void);

/**
 * @brief Generatore di numeri pseudo-casuali (Linear Feedback Shift Register).
 * @return uint16_t Valore pseudo-casuale.
 */
static inline uint16_t lfsr_next(void)
{
    uint16_t lsb = lfsr & 1u;
    lfsr >>= 1;
    if (lsb)
        lfsr ^= 0xB400u;
    return lfsr;
}

/**
 * @brief Genera un colore casuale basato su una palette predefinita (Rainbow).
 * @param r Puntatore al canale Rosso.
 * @param g Puntatore al canale Verde.
 * @param b Puntatore al canale Blu.
 */
static inline void random_rainbow(uint8_t *r, uint8_t *g, uint8_t *b)
{
    switch (lfsr_next() % 6) {
        case 0: *r = 255; *g = 0;   *b = 0;   break;
        case 1: *r = 255; *g = 80;  *b = 0;   break;
        case 2: *r = 255; *g = 255; *b = 0;   break;
        case 3: *r = 0;   *g = 255; *b = 0;   break;
        case 4: *r = 0;   *g = 0;   *b = 255; break;
        default:*r = 160; *g = 0;   *b = 255; break;
    }
}

/**
 * @brief Scalatura di un valore a 8 bit (dimming).
 * @param x Valore originale.
 * @param scale Fattore di scala (0-255).
 * @return uint8_t Valore scalato.
 */
static inline uint8_t scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;
}

/**
 * @brief Inizializza il driver delle strisce LED e le strutture dati.
 * @param cfg Puntatore alla configurazione hardware.
 * @return led_status_t LED_STRIPE_OK se inizializzato correttamente.
 */
led_status_t led_stripe_init(const led_stripes_config_t *cfg){

  if (!cfg) return LED_STRIPE_ERR;

  g_led_bus.cfg = *cfg;
  g_led_bus.wr_buf_p = 0;
  g_led_bus.dirty = 0;

  memset(g_led_bus.rgb_arr, 0, sizeof(g_led_bus.rgb_arr));
  memset(g_led_bus.wr_buf,  0, sizeof(g_led_bus.wr_buf));

  rear_led.bus  = &g_led_bus;
  rear_led.start = LED_REAR_LED_START;
  rear_led.end   = LED_REAR_LED_END;
  rear_led.last_animation = IDLE;
  rear_led.step = 0;

  rear_sign.bus  = &g_led_bus;
  rear_sign.start = LED_REAR_SIGN_START;
  rear_sign.end   = LED_REAR_SIGN_END;
  rear_sign.last_animation = IDLE;
  rear_sign.step = 0;

  return LED_STRIPE_OK;
}

/**
 * @brief Esegue un passo della macchina a stati per le luci posteriori.
 * @param animation Tipo di animazione richiesta.
 * @return led_status_t LED_STRIPE_OK se operazione riuscita.
 */
led_status_t rear_led_step(REAR_LED_TYPE animation){

    if (g_led_bus.wr_buf_p != 0) {
        return LED_STRIPE_OK;
    }

	led_status_t res = LED_STRIPE_ERR;

	if (animation != rear_led.last_animation){
		rear_led.step = 0;
		rear_led.last_animation = animation;
	}

	switch(animation){
		case IDLE:
			if(rear_led_off() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case BACKLIGHTS:
			if(rear_led_anim_backlights() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case  BRAKING_LIGHTS:
			if(rear_led_anim_stop() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case  BACKWARD_LIGHTS:
			if(rear_led_anim_backward() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case  ARROW_LEFT:
			if(rear_led_anim_arrow_left() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case  ARROW_RIGHT:
			if(rear_led_anim_arrow_right() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SPECIAL_LIGHTS:
			if(rear_led_special_lights() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case  EMERGENCY_LIGHTS:
			if(rear_led_emercency_lights() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case DEGRADED_LIGHTS:
			if(rear_led_degraded_lights() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		default:
			rear_led_off();
			res = LED_STRIPE_ERR;
			break;
	}
	return res;
}

/**
 * @brief Esegue un passo della macchina a stati per la targa posteriore.
 * @param animation Tipo di illuminazione targa richiesta.
 * @return led_status_t LED_STRIPE_OK se operazione riuscita.
 */
led_status_t rear_sign_step(REAR_SIGN_TYPE animation){

    if (g_led_bus.wr_buf_p != 0) {
        return LED_STRIPE_OK;
    }

	led_status_t res = LED_STRIPE_ERR;

	if (animation != rear_sign.last_animation){
		rear_sign.step = 0;
		rear_sign.last_animation = animation;
	}

	switch(animation){
		case SIGN_OFF:
			if(rear_sign_off() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SIGN_WHITE:
			if(rear_sign_white() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SIGN_GREEN:
			if(rear_sign_green() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SIGN_ORANGE:
			if(rear_sign_orange() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SIGN_RED:
			if(rear_sign_red() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		case SIGN_YELLOW:
			if(rear_sign_yellow() == LED_STRIPE_OK) res = LED_STRIPE_OK;
			break;
		default:
			rear_sign_off();
			res = LED_STRIPE_ERR;
			break;
	}
	return res;
}

/**
 * @brief Spegne i LED della sezione rear_led.
 */
static led_status_t rear_led_off(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Spegne i LED della sezione targa posteriore.
 */
static led_status_t rear_sign_off(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'animazione "Special Lights" (Rainbow casuale).
 */
static led_status_t rear_led_special_lights(void)
{
    for (uint16_t i = rear_led.start; i <= rear_led.end; i++) {
        uint8_t r, g, b;
        random_rainbow(&r, &g, &b);
        led_set_RGB((uint8_t)i, r, g, b);
    }

    g_led_bus.dirty = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Gestisce l'animazione luci di emergenza (lampeggio rosso).
 */
static led_status_t rear_led_emercency_lights(void)
{
	led_status_t res = LED_STRIPE_ERR;
	if (rear_led.step == 0 && led_set_RGB_range(rear_led.start, rear_led.end, 255, 0, 0) == LED_STRIPE_OK){
		g_led_bus.dirty = 1;
		res = LED_STRIPE_OK;
	}else if (rear_led.step == (EMERGENCY_PERIOD / 2) && led_set_RGB_range(rear_led.start, rear_led.end, 0, 0, 0) == LED_STRIPE_OK){
		g_led_bus.dirty = 1;
		res = LED_STRIPE_OK;
	}else{
		res = LED_STRIPE_OK;
	}

	rear_led.step = (rear_led.step + 1) % EMERGENCY_PERIOD;
	return res;
}

/**
 * @brief Imposta le luci degradate (Giallo fisso).
 */
static led_status_t rear_led_degraded_lights(void)
{
	if (rear_led.step != 0) return LED_STRIPE_OK;

	if (led_set_RGB_range(rear_led.start, rear_led.end, 255, 255, 0) != LED_STRIPE_OK)
		return LED_STRIPE_ERR;

	g_led_bus.dirty = 1;
	rear_led.step = 1;
	return LED_STRIPE_OK;
}

/**
 * @brief Imposta le luci di stop (Rosso intenso fisso).
 */
static led_status_t rear_led_anim_stop(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 255, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta le luci di posizione (Rosso tenue ai lati).
 */
static led_status_t rear_led_anim_backlights(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    uint16_t len = rear_led.end - rear_led.start + 1;
    uint16_t k = (len < 4) ? len : 4;

    for (uint16_t i = 0; i < k; i++) {
        led_set_RGB((uint8_t)(rear_led.start + i), 100, 0, 0);
        led_set_RGB((uint8_t)(rear_led.end - i),   100, 0, 0);
    }

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Gestisce l'animazione freccia destra (scorrimento arancione).
 */
static led_status_t rear_led_anim_arrow_right(void)
{
    const uint8_t r = 255, g = 80, b = 0;
    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)(full_end - full_start + 1);
    const uint16_t mid        = (uint16_t)(full_start + (full_len - 1) / 2);
    const uint16_t left_len   = (uint16_t)(mid - full_start + 1);
    const uint16_t fill_steps = left_len;
    const uint16_t cycle      = (uint16_t)(ARROW_OFF_STEPS + fill_steps + ARROW_ON_STEPS);

    if (rear_led.step < ARROW_OFF_STEPS) {
        if (rear_led.step == 0) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            g_led_bus.dirty = 1;
        }
    }
    else if (rear_led.step < (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {
        uint16_t s = (uint16_t)(rear_led.step - ARROW_OFF_STEPS + 1);
        led_set_RGB_range(full_start, full_end, 0, 0, 0);
        for (uint16_t i = 0; i < s && i < left_len; i++) {
            led_set_RGB((uint8_t)(mid - i), r, g, b);
        }
        g_led_bus.dirty = 1;
    }
    else {
        if (rear_led.step == (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            for (uint16_t i = 0; i < left_len; i++) {
                led_set_RGB((uint8_t)(mid - i), r, g, b);
            }
            g_led_bus.dirty = 1;
        }
    }

    rear_led.step = (uint16_t)((rear_led.step + 1) % cycle);
    return LED_STRIPE_OK;
}

/**
 * @brief Gestisce l'animazione freccia sinistra (scorrimento arancione).
 */
static led_status_t rear_led_anim_arrow_left(void)
{
    const uint8_t r = 255, g = 80, b = 0;
    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)(full_end - full_start + 1);
    const uint16_t mid        = (uint16_t)(full_start + (full_len - 1) / 2);
    const uint16_t right_len  = (uint16_t)(full_end - mid + 1);
    const uint16_t fill_steps = right_len;
    const uint16_t cycle      = (uint16_t)(ARROW_OFF_STEPS + fill_steps + ARROW_ON_STEPS);

    if (rear_led.step < ARROW_OFF_STEPS) {
        if (rear_led.step == 0) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            g_led_bus.dirty = 1;
        }
    }
    else if (rear_led.step < (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {
        uint16_t s = (uint16_t)(rear_led.step - ARROW_OFF_STEPS + 1);
        led_set_RGB_range(full_start, full_end, 0, 0, 0);
        for (uint16_t i = 0; i < s && i < right_len; i++) {
            led_set_RGB((uint8_t)(mid + i), r, g, b);
        }
        g_led_bus.dirty = 1;
    }
    else {
        if (rear_led.step == (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            for (uint16_t i = 0; i < right_len; i++) {
                led_set_RGB((uint8_t)(mid + i), r, g, b);
            }
            g_led_bus.dirty = 1;
        }
    }

    rear_led.step = (uint16_t)((rear_led.step + 1) % cycle);
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta le luci di retromarcia (Bianco rosato).
 */
static led_status_t rear_led_anim_backward(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 255, 180, 180) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'illuminazione targa colore bianco.
 */
static led_status_t rear_sign_white(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 255, 255) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'illuminazione targa colore rosso.
 */
static led_status_t rear_sign_red(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'illuminazione targa colore verde.
 */
static led_status_t rear_sign_green(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 0, 255, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'illuminazione targa colore arancione.
 */
static led_status_t rear_sign_orange(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 40, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

/**
 * @brief Imposta l'illuminazione targa colore giallo.
 */
static led_status_t rear_sign_yellow(void)
{
	if (rear_sign.step != 0) return LED_STRIPE_OK;

	if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 255, 0) != LED_STRIPE_OK)
		return LED_STRIPE_ERR;

	g_led_bus.dirty = 1;
	rear_sign.step = 1;
	return LED_STRIPE_OK;
}

/**
 * @brief Imposta il colore di un singolo pixel.
 * @note I valori vengono salvati in formato GRB e scalati secondo configurazione.
 * @param index Indice del pixel.
 * @param r Valore rosso (0-255).
 * @param g Valore verde (0-255).
 * @param b Valore blu (0-255).
 * @return led_status_t LED_STRIPE_OK se l'indice è valido.
 */
led_status_t led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {

  g_led_bus.rgb_arr[NUM_BPP * index] = scale8(g, g_led_bus.cfg.scale_g);
  g_led_bus.rgb_arr[NUM_BPP * index + 1] = r;
  g_led_bus.rgb_arr[NUM_BPP * index + 2] = scale8(b, g_led_bus.cfg.scale_b);

  return LED_STRIPE_OK;
}

/**
 * @brief Imposta lo stesso colore per tutti i pixel della striscia.
 * @param r Valore rosso.
 * @param g Valore verde.
 * @param b Valore blu.
 * @return led_status_t Esito dell'operazione.
 */
led_status_t led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {

  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) {
	  if(led_set_RGB(i, r, g, b) != LED_STRIPE_OK)
		  return LED_STRIPE_ERR;
  }

  return LED_STRIPE_OK;
}

/**
 * @brief Imposta il colore per un intervallo di pixel.
 * @param start Indice iniziale (incluso).
 * @param end Indice finale (incluso).
 * @param r Valore rosso.
 * @param g Valore verde.
 * @param b Valore blu.
 * @return led_status_t LED_STRIPE_ERR se i parametri sono invalidi.
 */
led_status_t led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b) {
    if (start > end) return LED_STRIPE_ERR;
    if (end >= NUM_PIXELS) return LED_STRIPE_ERR;

    for (uint16_t i = start; i <= end; i++) {
        if (led_set_RGB((uint8_t)i, r, g, b) != LED_STRIPE_OK) {
            return LED_STRIPE_ERR;
        }
    }
    return LED_STRIPE_OK;
}

/**
 * @brief Avvia il trasferimento DMA per renderizzare i colori sui LED.
 * @details Utilizza un meccanismo di buffer circolare per convertire i dati RGB
 * in segnali PWM compatibili con WS2812/SK6812.
 * @return led_status_t LED_STRIPE_OK.
 */
led_status_t led_render() {

  if(!g_led_bus.dirty)
	  return LED_STRIPE_OK;

  if(g_led_bus.wr_buf_p != 0 || g_led_bus.cfg.hdma->State != HAL_DMA_STATE_READY) {
    return LED_STRIPE_OK;
  }

  for(uint_fast8_t i = 0; i < 8; ++i) {
	  g_led_bus.wr_buf[i     ] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[0] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i +  8] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[1] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 16] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[2] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 24] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 32] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[4] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 40] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[5] << i) & 0x80) > 0);
  }

  g_led_bus.dirty = 0;
  HAL_TIM_PWM_Start_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel, (uint32_t *)g_led_bus.wr_buf, WR_BUF_LEN);
  g_led_bus.wr_buf_p = 2;

  return LED_STRIPE_OK;
}

/**
 * @brief Callback per completamento metà trasferimento DMA (Ping-Pong buffering).
 * @details Riempe la prima metà del buffer mentre la seconda è in trasmissione.
 * @param htim Handle del timer.
 */
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {

  if(g_led_bus.wr_buf_p < NUM_PIXELS) {
    for(uint_fast8_t i = 0; i < 8; ++i) {
    	g_led_bus.wr_buf[i     ] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p    ] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i +  8] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 1] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 16] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 2] << i) & 0x80) > 0);
    }
    g_led_bus.wr_buf_p++;
  } else if (g_led_bus.wr_buf_p < NUM_PIXELS + 2) {
    for(uint8_t i = 0; i < WR_BUF_LEN / 2; ++i) g_led_bus.wr_buf[i] = 0;
    g_led_bus.wr_buf_p++;
  }

}

/**
 * @brief Callback per completamento trasferimento DMA.
 * @details Riempe la seconda metà del buffer o termina la trasmissione inviando il segnale di reset.
 * @param htim Handle del timer.
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {

  if(g_led_bus.wr_buf_p < NUM_PIXELS) {
    /* Siamo nel mezzo della striscia: riempi il buffer dispari */
    for(uint_fast8_t i = 0; i < 8; ++i) {
    	g_led_bus.wr_buf[i + 24] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p    ] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 32] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 1] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 40] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 2] << i) & 0x80) > 0);
    }
    g_led_bus.wr_buf_p++;
  } else if (g_led_bus.wr_buf_p < NUM_PIXELS + 2) {
    /* Fine dati: invia zeri per il reset (seconda metà del buffer) */
    for(uint8_t i = WR_BUF_LEN / 2; i < WR_BUF_LEN; ++i) g_led_bus.wr_buf[i] = 0;
    ++g_led_bus.wr_buf_p;
  } else {
    /* Trasmissione completata */
	g_led_bus.wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel);
  }
}
