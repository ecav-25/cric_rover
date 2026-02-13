// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hw_config.c
 * @author  Gruppo 2
 * @brief   Definizione delle configurazioni hardware specifiche della board.
 * @details Questo file contiene le istanze delle strutture di configurazione
 * per l'astrazione dell'hardware (Timer, GPIO, ADC, DMA) utilizzato dai driver.
 * Centralizza le dipendenze dall'hardware generato da CubeMX.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "hw_config.h"
#include "hw_calibration.h"
#include "tim.h"
#include "adc.h"

/* ========================================================================== */
/* MOTORS CONFIGURATION                                                       */
/* ========================================================================== */

/**
 * @brief Tabella di configurazione hardware dei motori.
 * @details Mappa ogni identificativo logico del motore (FA, FB, BA, BB) al relativo
 * Timer hardware, Canale PWM e valori di calibrazione specifici.
 */
const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT] = {
    [MOTOR_FA] = { &htim3, TIM_CHANNEL_3, { PWM_STOP_FA, PWM_SCALE_FORWARD_FA, PWM_SCALE_BACKWARD_FA } },
    [MOTOR_FB] = { &htim3, TIM_CHANNEL_2, { PWM_STOP_FB, PWM_SCALE_FORWARD_FB, PWM_SCALE_BACKWARD_FB } },
    [MOTOR_BA] = { &htim3, TIM_CHANNEL_1, { PWM_STOP_BA, PWM_SCALE_FORWARD_BA, PWM_SCALE_BACKWARD_BA } },
    [MOTOR_BB] = { &htim3, TIM_CHANNEL_4, { PWM_STOP_BB, PWM_SCALE_FORWARD_BB, PWM_SCALE_BACKWARD_BB } },
};

/* ========================================================================== */
/* ENCODERS CONFIGURATION                                                     */
/* ========================================================================== */

/**
 * @brief Tabella di configurazione hardware degli encoder.
 * @details Associa i timer configurati in modalità Encoder Interface ai parametri
 * fisici (Impulsi per rivoluzione e rapporto di riduzione) per ogni ruota.
 */
const Encoder_HW_Config_t ENCODER_HW_CONFIG[ENCODER_COUNT] = {
    [ENCODER_FA] = { &htim5,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_FB] = { &htim20, { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_BA] = { &htim1,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_BB] = { &htim8,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
};

/* ========================================================================== */
/* ADC CONFIGURATION                                                          */
/* ========================================================================== */

/**
 * @brief Configurazione dei canali ADC.
 * @details Definisce l'handle ADC, il canale e i tempi di campionamento per
 * i sensori analogici (temperatura interna e tensione batteria).
 */
const ADC_HW_Config_t ADC_HW_CONFIG[ADC_COUNT] = {
	[ADC_TEMP_SENSOR] = { &hadc1, { ADC_CHANNEL_TEMPSENSOR_ADC1, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_247CYCLES_5, ADC_SINGLE_ENDED, ADC_OFFSET_NONE, 0U } },
	[ADC_BATTERY_VOLTAGE] = { &hadc1, { ADC_CHANNEL_9, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_247CYCLES_5, ADC_SINGLE_ENDED, ADC_OFFSET_NONE, 0U } },
};

/* ========================================================================== */
/* LEDS CONFIGURATION                                                         */
/* ========================================================================== */

/**
 * @brief Configurazione GPIO per i gruppi LED discreti.
 * @details Mappa i LED logici (A e B) ai pin fisici per i colori Rosso e Bianco.
 */
const HW_Led_Config_t HW_LED_CONFIG[LED_COUNT_HW] = {
		[LED_A] = {
			.port = {
				FA_LED_RED_GPIO_Port,   	// LED_RED
				FA_LED_WHITE_GPIO_Port    	// LED_WHITE
			},
			.pin = {
				FA_LED_RED_Pin,   	// LED_RED
				FA_LED_WHITE_Pin    // LED_WHITE
			},
			.init_pin_state = {
					GPIO_PIN_RESET,
					GPIO_PIN_RESET
			}
		},
		[LED_B] = {
			.port = {
				FB_LED_RED_GPIO_Port,   	// LED_RED
				FB_LED_WHITE_GPIO_Port    	// LED_WHITE
			},
			.pin = {
				FB_LED_RED_Pin,   	// LED_RED
				FB_LED_WHITE_Pin    // LED_WHITE
			},
			.init_pin_state = {
					GPIO_PIN_RESET,
					GPIO_PIN_RESET
			}
		}
};

/**
 * @brief Configurazione del driver per strisce LED indirizzabili (WS2812/SK6812).
 * @details Specifica il Timer PWM, il canale DMA associato e i valori di comparazione
 * per generare i duty-cycle corrispondenti ai bit 0 e 1 del protocollo one-wire.
 */
const led_stripes_config_t led_stripes_cfg[LED_STRIPES_COUNT] = {
		[LED_STRIPES_MAIN] = {
		  .htim = &htim17,
		  .hdma = &hdma_tim17_ch1,
		  .pwm_hi = 135,          /**< Duty cycle per bit logico '1' */
		  .pwm_lo = 55,           /**< Duty cycle per bit logico '0' */
		  .reset_halves = 2,
		  .scale_b = 0xF0,        /**< Fattore di scala (dimming) canale Blu */
		  .scale_g = 0xB0,        /**< Fattore di scala (dimming) canale Verde */
		  .scale_r = 0xFF,        /**< Fattore di scala (dimming) canale Rosso */
		  .tim_channel = TIM_CHANNEL_1
		}
};
