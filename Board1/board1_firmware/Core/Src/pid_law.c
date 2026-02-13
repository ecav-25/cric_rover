// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    pid_law.c
 * @author  Gruppo 2
 * @brief   Implementazione di un controllore PID discreto.
 * @details Questo modulo implementa una legge di controllo basata su un'equazione
 * alle differenze lineare, includendo la saturazione dell'uscita.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "pid_law.h"
#include <stddef.h>

/**
 * @brief Inizializza i coefficienti e azzera lo stato del controllore PID.
 * @param pid Puntatore alla struttura di controllo PID.
 * @param a1 Coefficiente relativo all'uscita precedente (polo del sistema discreto).
 * @param b0 Coefficiente relativo all'errore attuale.
 * @param b1 Coefficiente relativo all'errore precedente.
 * @return pid_law_status PID_LAW_OK se l'inizializzazione ha successo, PID_LAW_ERR se il puntatore è NULL.
 */
pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1){
    if (pid == NULL){
    	return PID_LAW_ERR;
    }

    pid->a1 = a1;
    pid->b0 = b0;
    pid->b1 = b1;

    pid->u_prev = 0.0f;
    pid->e_prev = 0.0f;

    return PID_LAW_OK;
}

/**
 * @brief Esegue un passo di calcolo della legge di controllo.
 * @details Implementa la seguente equazione alle differenze:
 * $$u[k] = a_1 \cdot u[k-1] + b_0 \cdot e[k] + b_1 \cdot e[k-1]$$
 * Dove:
 * - $e[k]$ è l'errore attuale (setpoint - feedback).
 * - $u[k]$ viene saturato tra @ref U_MIN e @ref U_MAX.
 *
 * @param pid Puntatore alla struttura di controllo PID.
 * @param setpoint Riferimento desiderato ($r[k]$).
 * @param feedback Valore misurato ($y[k]$).
 * @param u_out Puntatore alla variabile dove salvare l'uscita calcolata ($u[k]$).
 * @return pid_law_status PID_LAW_OK se il calcolo è avvenuto correttamente.
 */
pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out){
    if (pid == NULL || u_out == NULL){
    	return PID_LAW_ERR;
    }

    /* Calcolo errore */
    float e = setpoint - feedback;

    /* Legge di controllo (Equazione alle differenze) */
    float u = pid->a1 * pid->u_prev
            + pid->b0 * e
            + pid->b1 * pid->e_prev;

    /* Saturazione uscita (Clamping) */
    if (u > U_MAX){
    	u = U_MAX;
    }
    else if (u < U_MIN){
    	u = U_MIN;
    }

    /* Aggiornamento stato per il prossimo passo */
    pid->u_prev = u;
    pid->e_prev = e;

    *u_out = u;

    return PID_LAW_OK;
}

/**
 * @brief Resetta lo stato interno del PID (memoria integrale/storica).
 * @details Utile per evitare "bumps" o comportamenti erratici quando si riattiva
 * il controllore dopo un periodo di inattività o cambio modalità.
 * @param pid Puntatore alla struttura di controllo PID.
 * @return pid_law_status PID_LAW_OK se il reset è avvenuto.
 */
pid_law_status PID_Law_reset(PID_Law_t* pid){
    if (pid == NULL){
    	return PID_LAW_ERR;
    }

    pid->u_prev = 0.0f;
    pid->e_prev = 0.0f;

    return PID_LAW_OK;
}
