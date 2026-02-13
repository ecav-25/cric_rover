// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file encoder.h
 * @author Gruppo 2
 * * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).

 * @brief Gestione della lettura di velocità tramite encoder rotativo.
 * * Il modulo utilizza un timer in modalità Encoder Interface per calcolare
 * la velocità angolare (RPM) di un motore o albero, gestendo il gear ratio
 * e i parametri di campionamento.
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include <stdint.h>

/**
 * @brief Codici di ritorno per il modulo encoder.
 */
typedef enum {
	ENCODER_OK = 0,    /**< Operazione completata correttamente */
	ENCODER_ERR = -1   /**< Errore generico (parametro NULL o errore hardware) */
} Encoder_Status_t;

/**
 * @brief Parametri di calibrazione meccanica dell'encoder.
 */
typedef struct {
	uint16_t pulses_per_rev; /**< Numero di impulsi per ogni giro completo dell'albero encoder (PPR) */
	float    gear_ratio;      /**< Rapporto di riduzione tra motore e carico (1.0 se non presente) */
} Encoder_Calibration_t;

/**
 * @brief Handle per la gestione dell'istanza encoder.
 */
typedef struct {
	TIM_HandleTypeDef* htim;             /**< Puntatore all'handle del timer STM32 */
	uint32_t           prev_count;       /**< Valore del contatore alla lettura precedente */
	uint32_t           arr;              /**< Valore di Auto-Reload Register del timer (fondo scala) */
	float              reading_period;   /**< Periodo di campionamento in secondi */
	Encoder_Calibration_t calib;         /**< Configurazione della calibrazione */
	float              velocity;         /**< Ultima velocità calcolata in RPM */
} EncoderHandle;

/* ================== API ================== */

/**
 * @brief Inizializza l'handle dell'encoder e avvia il timer.
 * * @param enc Puntatore alla struttura EncoderHandle.
 * @param htim Puntatore all'handle del timer configurato come Encoder.
 * @param calib Puntatore alla configurazione meccanica.
 * @param reading_period_ms Intervallo di tempo (in ms) tra due chiamate consecutive di lettura.
 * @return Encoder_Status_t ENCODER_OK se l'inizializzazione è riuscita.
 */
Encoder_Status_t encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, const Encoder_Calibration_t* calib, float reading_period_ms);

/**
 * @brief Calcola la velocità attuale in RPM (giri al minuto).
 * * La funzione gestisce l'overflow del contatore del timer e calcola la velocità media nel periodo configurato in fase di inizializzazione.
 * 	La funzione deve essere invocata ad un rate pari al period configurato.
 *  @param enc Puntatore alla struttura EncoderHandle.
 * @return Encoder_Status_t ENCODER_OK se la lettura è stata eseguita correttamente.
 */
Encoder_Status_t encoder_readRPM(EncoderHandle* enc);

#ifdef __cplusplus
}
#endif

#endif /* INC_ENCODER_H_ */
