// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    motor.c
 * @author  Gruppo 2
 * @brief   Driver per il controllo di motori tramite segnale PWM.
 * @details Questo modulo gestisce la generazione del segnale PWM per motori
 * (es. ESC o driver DC bidirezionali) che richiedono un segnale neutro di arresto
 * e variazioni positive/negative per il controllo della direzione.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "motor.h"
#include <math.h>

/* ========================================================================== */
/* STATIC HELPER FUNCTIONS                                                    */
/* ========================================================================== */

/**
 * @brief Limita il duty cycle al valore massimo consentito.
 * @param duty Valore di duty cycle richiesto.
 * @return uint8_t Valore limitato a @ref MOTOR_MAX_DUTY.
 */
static inline uint8_t clip_duty(uint8_t duty)
{
    uint8_t res = duty;

    if (res > MOTOR_MAX_DUTY)
    {
        res = MOTOR_MAX_DUTY;
    }

    return res;
}

/**
 * @brief Calcola il valore del registro di comparazione (CCR) per il PWM.
 * @details La funzione mappa il duty cycle percentuale (0-100) nel valore
 * del contatore del timer, partendo da un valore di "Stop" (neutro).
 *
 * La formula applicata è:
 * $$PWM = PWM_{stop} \pm \text{round}\left( \frac{Duty \cdot Scale}{MaxDuty} \right)$$
 * Dove il segno dipende dalla direzione (CW o CCW).
 *
 * @param m Puntatore alla struttura del motore (contiene i dati di calibrazione).
 * @param duty Duty cycle limitato (0-100).
 * @param dir Direzione di rotazione.
 * @return uint16_t Valore da scrivere nel registro CCR del timer.
 */
static uint16_t compute_pwm(const Motor_t* m, uint8_t duty, Motor_Direction_t dir)
{
    uint16_t pwm;
    float delta;

    if (duty == MOTOR_MIN_DUTY)
    {
        pwm = m->calib.pwm_stop;
    }
    else if (dir == CLOCKWISE)
    {
        /* Calcolo incremento per rotazione oraria */
        delta = (float)duty * m->calib.pwm_scale_forward / (float)MOTOR_MAX_DUTY;
        pwm = m->calib.pwm_stop + (uint16_t)roundf(delta);
    }
    else if (dir == COUNTERCLOCKWISE)
    {
        /* Calcolo decremento per rotazione antioraria */
        delta = (float)duty * m->calib.pwm_scale_backward / (float)MOTOR_MAX_DUTY;

        /* Controllo di sicurezza per evitare underflow (anche se pwm_stop > delta solitamente) */
        if (delta > m->calib.pwm_stop)
        {
             pwm = 0;
        }
        else
        {
             pwm = m->calib.pwm_stop - (uint16_t)roundf(delta);
        }
    }
    else
    {
        /* Direzione non valida o STOP */
        pwm = m->calib.pwm_stop;
    }

    return pwm;
}

/**
 * @brief Applica fisicamente il valore PWM al registro del timer.
 * @param m Puntatore alla struttura del motore.
 * @param value Valore del registro di comparazione (CCR).
 */
static inline void apply_pwm(const Motor_t* m, uint16_t value)
{
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, value);
}

/* ========================================================================== */
/* API FUNCTIONS                                                              */
/* ========================================================================== */

/**
 * @brief Inizializza l'istanza del motore e avvia il canale PWM.
 * @param motor Puntatore alla struttura di controllo del motore.
 * @param htim Handle del timer hardware STM32.
 * @param channel Canale del timer (es. TIM_CHANNEL_1).
 * @param calib Puntatore alla configurazione di calibrazione (stop, scale).
 * @return Motor_Status_t MOTOR_OK se inizializzazione riuscita.
 */
Motor_Status_t motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, const Motor_Calibration_t* calib)
{
    Motor_Status_t status = MOTOR_ERR;

    if ((motor != NULL) && (htim != NULL) && (calib != NULL))
    {
        motor->htim = htim;
        motor->channel = channel;
        motor->calib = *calib;

        /* Imposta il PWM al valore di stop iniziale */
        apply_pwm(motor, calib->pwm_stop);

        /* Avvia il PWM hardware */
        (void)HAL_TIM_PWM_Start(htim, channel);

        status = MOTOR_OK;
    }

    return status;
}

/**
 * @brief Imposta la velocità e la direzione del motore.
 * @param motor Puntatore alla struttura del motore.
 * @param duty Duty cycle desiderato (0-100). Verrà automaticamente clippato.
 * @param dir Direzione di rotazione (CLOCKWISE o COUNTERCLOCKWISE).
 * @return Motor_Status_t MOTOR_OK se i parametri sono validi.
 */
Motor_Status_t motor_set(const Motor_t* motor, uint8_t duty, Motor_Direction_t dir)
{
    Motor_Status_t status = MOTOR_ERR;

    if (motor != NULL)
    {
        /* 1. Limita il duty cycle ai margini di sicurezza */
        uint8_t safe_duty = clip_duty(duty);

        /* 2. Calcola il valore di registro CCR e applicalo */
        apply_pwm(motor, compute_pwm(motor, safe_duty, dir));

        status = MOTOR_OK;
    }

    return status;
}
