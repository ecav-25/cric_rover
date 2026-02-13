// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    motor.h
 * @author  Gruppo 2
 * @brief   Driver per il controllo dei motori DC tramite segnali PWM.
 * @details Questo modulo permette di pilotare i motori del rover gestendo la direzione
 * di rotazione e la potenza. Utilizza i parametri di calibrazione
 * per compensare le differenze hardware tra i vari motori (punti di stop e scale).
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include <stdint.h>

/** @name Limiti Duty Cycle
 * @{ */
#define MOTOR_MAX_DUTY 100u  /**< Valore massimo del comando (100%) */
#define MOTOR_MIN_DUTY 0u    /**< Valore minimo del comando (0%) */
/** @} */

/**
 * @brief Codici di ritorno per le funzioni del modulo motore.
 */
typedef enum {
    MOTOR_OK = 0,    /**< Operazione eseguita correttamente */
    MOTOR_ERR = -1   /**< Errore di configurazione o parametri non validi */
} Motor_Status_t;

/**
 * @brief Definizioni per il senso di marcia.
 */
typedef enum {
    CLOCKWISE = 1,          /**< Rotazione oraria (Avanti) */
    COUNTERCLOCKWISE = -1   /**< Rotazione antioraria (Indietro) */
} Motor_Direction_t;

/**
 * @brief Parametri di calibrazione specifici per ogni motore.
 * @details Questi valori permettono di linearizzare la risposta dei motori, definendo
 * il punto di neutro (stop) e i guadagni per le due direzioni.
 */
typedef struct {
    uint16_t pwm_stop;             /**< Valore del registro CCR per cui il motore è fermo */
    uint16_t pwm_scale_forward;    /**< Moltiplicatore per la marcia oraria */
    uint16_t pwm_scale_backward;   /**< Moltiplicatore per la marcia antioraria */
} Motor_Calibration_t;

/**
 * @brief Handle che rappresenta un'istanza fisica di un motore.
 */
typedef struct {
    TIM_HandleTypeDef* htim;       /**< Puntatore al Timer STM32 configurato in PWM */
    uint32_t channel;              /**< Canale del timer associato al motore (es. TIM_CHANNEL_1) */
    Motor_Calibration_t calib;     /**< Parametri di calibrazione hardware */
} Motor_t;

/* ========================================================================== */
/* API                                                                        */
/* ========================================================================== */

/**
 * @brief Inizializza l'oggetto motore e avvia la generazione del segnale PWM.
 * @param motor Puntatore alla struttura Motor_t.
 * @param htim Puntatore all'handle del timer HAL.
 * @param channel Canale del timer PWM.
 * @param calib Puntatore alla configurazione di calibrazione.
 * @return Motor_Status_t MOTOR_OK se l'inizializzazione ha successo.
 */
Motor_Status_t motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, const Motor_Calibration_t* calib);

/**
 * @brief Imposta la velocità e la direzione del motore.
 * @details La funzione calcola il valore reale da scrivere nel registro del timer (CCR)
 * basandosi sulla percentuale richiesta e sulla calibrazione.
 * @param motor Puntatore alla struttura motore.
 * @param duty Valore di potenza da 0 a 100.
 * @param dir Direzione di rotazione (CLOCKWISE o COUNTERCLOCKWISE).
 * @return Motor_Status_t MOTOR_OK se il comando è valido.
 */
Motor_Status_t motor_set(const Motor_t* motor, uint8_t duty, Motor_Direction_t dir);

#endif /* INC_MOTOR_H_ */
