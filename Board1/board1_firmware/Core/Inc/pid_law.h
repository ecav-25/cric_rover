// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file pid_law.h
 * @brief Implementazione di una legge di controllo PID discreta.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 *
 * @author Gruppo 2
 */

#ifndef PID_LAW_H_
#define PID_LAW_H_

/** @brief Limite massimo di saturazione dell'uscita di controllo (12V). */
#define U_MAX (12.0f)
/** @brief Limite minimo di saturazione dell'uscita di controllo (-12V). */
#define U_MIN (-12.0f)

/**
 * @enum pid_law_status
 * @brief Codici di stato e di errore per il modulo PID.
 */
typedef enum {
    PID_LAW_OK  = 0,  /**< Operazione completata con successo. */
    PID_LAW_ERR = -1  /**< Errore di puntatore o parametro non valido. */
} pid_law_status;

/**
 * @struct PID_Law_t
 * @brief Struttura dati per il controllore PID in forma discreta.
 *
 * Equazione alle differenze:
 * $$u[k] = -a_1 \cdot u[k-1] + b_0 \cdot e[k] + b_1 \cdot e[k-1]$$
 */
typedef struct {
    float a1;      /**< Coefficiente feedback dell'uscita precedente. */
    float b0;      /**< Coefficiente feedforward dell'errore attuale. */
    float b1;      /**< Coefficiente feedforward dell'errore precedente. */

    float u_prev;  /**< Memoria dell'azione di controllo al passo precedente. */
    float e_prev;  /**< Memoria dell'errore al passo precedente. */
} PID_Law_t;

/**
 * @brief Inizializza i coefficienti e resetta la memoria del PID.
 *
 * @param[out] pid Puntatore alla struttura PID.
 * @param[in] a1 Coefficiente del termine autoregressivo.
 * @param[in] b0 Guadagno sull'errore istantaneo.
 * @param[in] b1 Guadagno sull'errore ritardato.
 * @return pid_law_status PID_LAW_OK se riuscito, altrimenti PID_LAW_ERR.
 */
pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1);

/**
 * @brief Calcola il nuovo valore dell'azione di controllo.
 *
 * Implementa la legge di controllo e applica la saturazione anti-windup
 * hardware definita da U_MIN e U_MAX.
 *
 * @param[in,out] pid Puntatore alla struttura PID
 * @param[in] setpoint Valore di riferimento desiderato.
 * @param[in] feedback Misura attuale dal sistema.
 * @param[out] u_out Puntatore per il valore di uscita calcolato.
 * @return pid_law_status PID_LAW_OK se il calcolo è valido.
 */
pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out);

/**
 * @brief Azzera gli stati interni (u_prev ed e_prev) del controllore.
 *
 * Da utilizzare per evitare "salti" all'attivazione del controllo.
 *
 * @param[in,out] pid Puntatore alla struttura PID da resettare.
 * @return pid_law_status PID_LAW_OK in caso di successo.
 */
pid_law_status PID_Law_reset(PID_Law_t* pid);

#endif /* PID_LAW_H_ */
