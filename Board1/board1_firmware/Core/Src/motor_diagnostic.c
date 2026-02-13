// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    motor_diagnostic.c
 * @author  Gruppo 2
 * @brief   Implementazione del modulo di diagnostica motori.
 * @details Questo file gestisce l'acquisizione dei dati in buffer circolari (Ping-Pong)
 * e il calcolo dell'errore IAE (Integral Absolute Error) per rilevare anomalie
 * nel comportamento dei motori DC.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "motor_diagnostic.h"
#include <math.h>
#include <string.h>
#include <stddef.h>

/* ========================================================================== */
/* STATIC HELPER FUNCTIONS                                                    */
/* ========================================================================== */

/**
 * @brief Calcola l'Area di Errore Assoluto (IAE) applicando uno shift temporale.
 * @details La funzione compensa il ritardo fisiologico del sistema (lag) confrontando
 * il riferimento al tempo T con la misura al tempo T + shift.
 *
 * La formula applicata è:
 * $$IAE = \sum_{i=0}^{N-shift} |Ref[i] - Meas[i + shift]|$$
 *
 * @param ref Puntatore al buffer dei riferimenti.
 * @param meas Puntatore al buffer delle misure.
 * @param len Lunghezza totale dei buffer.
 * @param shift Numero di campioni di ritardo da compensare.
 * @return float Valore cumulativo dell'errore assoluto.
 */
static float calculate_iae_shifted(const float* ref, const float* meas, uint16_t len, uint16_t shift)
{
	float area = 0.0f;
	float diff = 0.0f;
	uint16_t check_len;

	/* Verifica che lo shift non superi la lunghezza del buffer */
	if (shift < len)
	{
		/* Analizziamo solo la porzione sovrapponibile dei segnali */
		check_len = (uint16_t)(len - shift);

		for (uint16_t i = 0U; i < check_len; i++)
		{
			/* Calcolo differenza con shift temporale */
			diff = ref[i] - meas[i + shift];

			/* Valore assoluto (senza dipendenza da math.h se possibile) */
			if (diff < 0.0f)
			{
				diff = -diff;
			}

			area += diff;
		}
	}

	return area;
}

/* ========================================================================== */
/* API FUNCTIONS                                                              */
/* ========================================================================== */

/**
 * @brief Inizializza il modulo e azzera i buffer interni.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @param config Puntatore alla configurazione parametri.
 * @return MotorDiag_Status_t M_DIAG_OK se l'inizializzazione ha successo.
 */
MotorDiag_Status_t motor_diag_init(MotorDiag_Handle_t* hdiag, const MotorDiag_Config_t* config)
{
	MotorDiag_Status_t status = M_DIAG_ERR;

	if ((hdiag != NULL) && (config != NULL))
	{
		hdiag->config = *config;

		/* Reset completo dei buffer a 0 */
		(void)memset(hdiag->buf_ref_A, 0, sizeof(hdiag->buf_ref_A));
		(void)memset(hdiag->buf_ref_B, 0, sizeof(hdiag->buf_ref_B));
		(void)memset(hdiag->buf_meas_A, 0, sizeof(hdiag->buf_meas_A));
		(void)memset(hdiag->buf_meas_B, 0, sizeof(hdiag->buf_meas_B));

		/* Inizializzazione stato interno */
		hdiag->active_buf_idx = 0U;
		hdiag->write_idx = 0U;
		hdiag->is_buffer_ready = false;
		hdiag->last_area_error = 0.0f;
		hdiag->health_status = MOTOR_HEALTHY;

		status = M_DIAG_OK;
	}

	return status;
}

/**
 * @brief Registra un nuovo campione nei buffer interni (Ping-Pong logic).
 * @details Scrive nel buffer attivo. Quando questo è pieno, scambia l'indice
 * del buffer attivo e imposta il flag `is_buffer_ready` per segnalare che
 * l'altro buffer è pronto per essere processato.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @param ref_val Valore di riferimento (setpoint).
 * @param meas_val Valore misurato (encoder).
 * @return MotorDiag_Status_t M_DIAG_OK se registrato correttamente.
 */
MotorDiag_Status_t motor_diag_record(MotorDiag_Handle_t* hdiag, float ref_val, float meas_val)
{
	MotorDiag_Status_t status = M_DIAG_ERR;

	if (hdiag != NULL)
	{
		uint16_t pos = hdiag->write_idx;

		/* Scrittura nel buffer attivo (A o B) */
		if (hdiag->active_buf_idx == 0U)
		{
			hdiag->buf_ref_A[pos] = ref_val;
			hdiag->buf_meas_A[pos] = meas_val;
		}
		else
		{
			hdiag->buf_ref_B[pos] = ref_val;
			hdiag->buf_meas_B[pos] = meas_val;
		}

		hdiag->write_idx++;

		/* Controllo fine buffer */
		if (hdiag->write_idx >= (uint16_t)M_DIAG_BUF_LEN)
		{
			/* Reset indice scrittura */
			hdiag->write_idx = 0U;

			/* Segnala che un buffer è completo e pronto per l'analisi */
			hdiag->is_buffer_ready = true;

			/* Swap del buffer attivo (Ping-Pong) */
			hdiag->active_buf_idx = (hdiag->active_buf_idx == 0U) ? 1U : 0U;
		}

		status = M_DIAG_OK;
	}
	return status;
}

/**
 * @brief Elabora i dati raccolti per determinare lo stato di salute del motore.
 * @details Analizza il buffer "pieno" (quello appena completato, opposto a quello attivo).
 * Calcola l'errore IAE e aggiorna lo stato di salute.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @return MotorDiag_Status_t M_DIAG_OK se elaborazione completata (o nessuna pending).
 */
MotorDiag_Status_t motor_diag_process(MotorDiag_Handle_t* hdiag)
{
	MotorDiag_Status_t status = M_DIAG_ERR;

	if (hdiag != NULL)
	{
		/* Procedi solo se c'è un buffer completo da analizzare */
		if (hdiag->is_buffer_ready)
		{
			/* Determina quale buffer analizzare (l'opposto di quello in scrittura) */
			uint8_t analyze_idx = (hdiag->active_buf_idx == 0U) ? 1U : 0U;
			const float* p_ref;
			const float* p_meas;

			/* Selezione puntatori ai buffer pieni */
			if (analyze_idx == 0U)
			{
				p_ref = hdiag->buf_ref_A;
				p_meas = hdiag->buf_meas_A;
			}
			else
			{
				p_ref = hdiag->buf_ref_B;
				p_meas = hdiag->buf_meas_B;
			}

			/* Calcolo metrica di errore */
			hdiag->last_area_error = calculate_iae_shifted(
					p_ref,
					p_meas,
					(uint16_t)M_DIAG_BUF_LEN,
					hdiag->config.delay_shift
			);

			/* Valutazione dello stato di salute */
			if (hdiag->last_area_error > hdiag->config.max_area_limit)
			{
				hdiag->health_status = MOTOR_FAILURE;
			}
			else
			{
				hdiag->health_status = MOTOR_HEALTHY;
			}

			/* Reset flag: elaborazione completata per questo ciclo */
			hdiag->is_buffer_ready = false;
		}

		status = M_DIAG_OK;
	}

	return status;
}
