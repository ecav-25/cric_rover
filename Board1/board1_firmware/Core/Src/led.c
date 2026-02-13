// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    led.c
 * @author  Gruppo 2
 * @brief   Driver per la gestione dei LED di stato (Rosso e Bianco).
 * @details Questo modulo implementa una macchina a stati per controllare
 * diversi pattern di lampeggio (fisso, blinking veloce/lento) su due LED.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "led.h"

/** @brief Numero di step in cui il LED rosso rimane acceso nel lampeggio veloce. */
#define RED_STEPS_ON (4U)
/** @brief Periodo totale (step accesi + step spenti) lampeggio veloce rosso. */
#define BLINKING_RED_STEPS (8U)

/** @brief Numero di step in cui il LED rosso rimane acceso nel lampeggio lento. */
#define RED_STEPS_SLOW_ON (8U)
/** @brief Periodo totale lampeggio lento rosso. */
#define BLINKING_RED_SLOW_STEPS (16U)

/** @brief Numero di step in cui il LED bianco rimane acceso nel lampeggio. */
#define WHITE_STEPS_ON (8U)
/** @brief Periodo totale lampeggio bianco. */
#define BLINKING_WHITE_STEPS (16U)

/* ========================================================================== */
/* STATIC FUNCTION PROTOTYPES                                                 */
/* ========================================================================== */

static int8_t led_mode_off(led_t* led);
static int8_t led_mode_white(led_t* led);
static int8_t led_mode_red(led_t* led);
static int8_t led_mode_blinking_red(led_t* led);
static int8_t led_mode_blinking_red_slow(led_t* led);
static int8_t led_mode_blinking_white(led_t* led);

/* ========================================================================== */
/* API FUNCTIONS                                                              */
/* ========================================================================== */

/**
 * @brief Inizializza la struttura di controllo dei LED.
 * @param led Puntatore all'handle dei LED.
 * @param GPIOx Array dei puntatori alle porte GPIO (es. GPIOA, GPIOB).
 * @param GPIO_Pin Array dei pin corrispondenti (es. GPIO_PIN_0).
 * @param init_pin_state Stato iniziale dei pin (RESET/SET).
 * @return int8_t LED_OK se inizializzazione riuscita.
 */
int8_t led_init(led_t* led, GPIO_TypeDef* const GPIOx[LED_COUNT], const uint16_t GPIO_Pin[LED_COUNT], const pin_state_t init_pin_state[LED_COUNT])
{
	int8_t res = LED_ERR;

	if(led != NULL)
	{
		led->state = OFF;
		led->step = 0;

		for(uint8_t i = 0; i < (uint8_t)LED_COUNT; i++){
			led->pin[i] = GPIO_Pin[i];
			led->port[i] = GPIOx[i];
			led->pinState[i] = init_pin_state[i];
			HAL_GPIO_WritePin(led->port[i], led->pin[i], led->pinState[i]);
		}

		res = LED_OK;
	}

	return res;
}

/**
 * @brief Scrive direttamente lo stato logico su un LED specifico.
 * @param led Puntatore all'handle.
 * @param status Stato desiderato (1=ON/SET, 0=OFF/RESET).
 * @param controlled_led Indice del LED da controllare (LED_RED o LED_WHITE).
 * @return int8_t LED_OK se comando eseguito.
 */
int8_t led_write(led_t* led, pin_state_t status, controlled_led_t controlled_led)
{
	int8_t res = LED_ERR;

	if(led != NULL)
	{
		led->pinState[controlled_led] = status;
		HAL_GPIO_WritePin(led->port[controlled_led], led->pin[controlled_led], led->pinState[controlled_led]);
		res = LED_OK;
	}

	return res;
}

/**
 * @brief Accende un LED specifico.
 * @param led Puntatore all'handle.
 * @param controlled_led Indice del LED.
 * @return int8_t LED_OK se comando eseguito.
 */
int8_t led_on(led_t* led, controlled_led_t controlled_led)
{
	return led_write(led, 1, controlled_led);
}

/**
 * @brief Spegne un LED specifico.
 * @param led Puntatore all'handle.
 * @param controlled_led Indice del LED.
 * @return int8_t LED_OK se comando eseguito.
 */
int8_t led_off(led_t* led, controlled_led_t controlled_led)
{
	return led_write(led, 0, controlled_led);
}

/**
 * @brief Esegue un passo della macchina a stati per l'animazione dei LED.
 * @details Gestisce le transizioni di stato e invoca la funzione di gestione specifica
 * per la modalità corrente (fisso, lampeggiante, spento).
 * @param led Puntatore all'handle.
 * @param state Nuovo stato desiderato o stato corrente da mantenere.
 * @return int8_t LED_OK se l'aggiornamento è avvenuto correttamente.
 */
int8_t led_step(led_t* led, led_state_t state)
{
	int8_t res = LED_ERR;

	if (led != NULL)
	{
		/* Rilevamento cambio stato: resetta contatori */
		if (state != led->state)
		{
			led->step = 0U;
			led->state = state;
			led->first_entry = 1U;
		}

		switch (state)
		{
		case OFF:
			if (led_mode_off(led) == LED_OK) res = LED_OK;
			break;

		case WHITE:
			if (led_mode_white(led) == LED_OK) res = LED_OK;
			break;

		case RED:
			if (led_mode_red(led) == LED_OK) res = LED_OK;
			break;

		case BLINKING_RED:
			if (led_mode_blinking_red(led) == LED_OK) res = LED_OK;
			break;

		case BLINKING_WHITE:
			if (led_mode_blinking_white(led) == LED_OK) res = LED_OK;
			break;

		case BLINKING_RED_SLOW:
			if (led_mode_blinking_red_slow(led) == LED_OK) res = LED_OK;
			break;

		default:
			/* Stato invalido: spegni tutto per sicurezza */
			(void)led_off(led, LED_WHITE);
			(void)led_off(led, LED_RED);
			res = LED_ERR;
			break;
		}
	}

	return res;
}

/* ========================================================================== */
/* STATIC MODE IMPLEMENTATIONS                                                */
/* ========================================================================== */

/**
 * @brief Gestisce la modalità OFF (tutti i LED spenti).
 */
static int8_t led_mode_off(led_t* led)
{
	int8_t res = LED_OK;

	if (led->step == 0U)
	{
		if ((led_off(led, LED_RED) == LED_OK) &&
				(led_off(led, LED_WHITE) == LED_OK))
		{
			led->step++;
		}
		else
		{
			res = LED_ERR;
		}
	}

	return res;
}

/**
 * @brief Gestisce la modalità Bianco Fisso.
 */
static int8_t led_mode_white(led_t* led)
{
	int8_t res = LED_ERR;

	if (led->step != 0U)
	{
		res = LED_OK; /* Già configurato */
	}
	else
	{
		if ((led_off(led, LED_RED) == LED_OK) &&
				(led_on(led, LED_WHITE) == LED_OK))
		{
			led->step++;
			res = LED_OK;
		}
	}

	return res;
}

/**
 * @brief Gestisce la modalità Rosso Fisso.
 */
static int8_t led_mode_red(led_t* led)
{
	int8_t res = LED_ERR;

	if (led->step != 0U)
	{
		res = LED_OK; /* Già configurato */
	}
	else
	{
		if ((led_on(led, LED_RED) == LED_OK) &&
				(led_off(led, LED_WHITE) == LED_OK))
		{
			led->step++;
			res = LED_OK;
		}
	}

	return res;
}

/**
 * @brief Gestisce il lampeggio veloce del LED rosso.
 */
static int8_t led_mode_blinking_red(led_t* led)
{
	if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
		(void)led_off(led, LED_WHITE);
	}

	/* Gestione ciclo ON/OFF */
	if (led->step < RED_STEPS_ON) {
		if (led->pinState[LED_RED] != GPIO_PIN_SET){
			(void)led_on(led, LED_RED);
		}
	}
	else {
		if (led->pinState[LED_RED] != GPIO_PIN_RESET){
			(void)led_off(led, LED_RED);
		}
	}

	led->step = (led->step + 1U) % BLINKING_RED_STEPS;

	return LED_OK;
}

/**
 * @brief Gestisce il lampeggio lento del LED rosso.
 */
static int8_t led_mode_blinking_red_slow(led_t* led)
{
	if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
		(void)led_off(led, LED_WHITE);
	}

	if (led->step < RED_STEPS_SLOW_ON) {
		if (led->pinState[LED_RED] != GPIO_PIN_SET){
			(void)led_on(led, LED_RED);
		}
	}
	else {
		if (led->pinState[LED_RED] != GPIO_PIN_RESET){
			(void)led_off(led, LED_RED);
		}
	}

	led->step = (led->step + 1U) % BLINKING_RED_SLOW_STEPS;

	return LED_OK;
}

/**
 * @brief Gestisce il lampeggio del LED bianco.
 */
static int8_t led_mode_blinking_white(led_t* led)
{
	if (led->pinState[LED_RED] != GPIO_PIN_RESET){
		(void)led_off(led, LED_RED);
	}

	if (led->step < WHITE_STEPS_ON) {
		if (led->pinState[LED_WHITE] != GPIO_PIN_SET){
			(void)led_on(led, LED_WHITE);
		}
	} else {
		if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
			(void)led_off(led, LED_WHITE);
		}
	}

	led->step = (led->step + 1U) % BLINKING_WHITE_STEPS;

	return LED_OK;
}
