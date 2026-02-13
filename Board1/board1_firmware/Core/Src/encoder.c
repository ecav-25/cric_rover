// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    encoder.c
 * @author  Gruppo 2
 * @brief   Driver per la gestione degli encoder in quadratura tramite Timer STM32.
 * @details Questo modulo gestisce la lettura della posizione angolare e il calcolo
 * della velocità (RPM) gestendo automaticamente l'overflow/underflow del contatore
 * hardware del timer.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "encoder.h"
#include "gpio.h"

/**
 * @brief Inizializza l'handle dell'encoder e avvia il timer in modalità Encoder.
 * @param enc Puntatore alla struttura di controllo dell'encoder.
 * @param htim Handle del timer hardware STM32 configurato in modalità Encoder Interface.
 * @param calib Puntatore alla struttura contenente i parametri fisici (PPR, Gear Ratio).
 * @param reading_period_ms Periodo di campionamento della velocità in millisecondi.
 * @return Encoder_Status_t ENCODER_OK se inizializzazione riuscita.
 */
Encoder_Status_t encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, const Encoder_Calibration_t* calib, float reading_period_ms)
{
    Encoder_Status_t status = ENCODER_ERR;

    if ((enc != NULL) && (htim != NULL) && (calib != NULL))
    {
        enc->htim = htim;
        /* Salva il conteggio attuale per il primo calcolo differenziale */
        enc->prev_count = __HAL_TIM_GET_COUNTER(htim);
        enc->arr = __HAL_TIM_GET_AUTORELOAD(htim);

        /* Conversione periodo da ms a secondi per il calcolo della velocità */
        enc->reading_period = reading_period_ms / 1000.0f;

        enc->calib = *calib;
        enc->velocity = 0.0f;

        /* Avvia il timer in modalità Encoder Interface (tutti i canali) */
        (void)HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);

        status = ENCODER_OK;
    }

    return status;
}

/**
 * @brief Calcola la velocità di rotazione in RPM.
 * @details La funzione calcola la differenza di impulsi rispetto all'ultima lettura,
 * gestendo il wrap-around (overflow/underflow) del registro contatore (CNT) rispetto
 * al valore di Auto-Reload (ARR).
 *
 * La formula applicata per la velocità è:
 * $$RPM = \frac{60 \cdot \Delta_{pulses}}{PPR \cdot T_{sample} \cdot GearRatio}$$
 *
 * @note La direzione positiva/negativa dipende dalla direzione di conteggio hardware.
 * In questa implementazione, il conteggio alla rovescia (Down) è considerato velocità positiva.
 *
 * @param enc Puntatore alla struttura di controllo dell'encoder.
 * @return Encoder_Status_t ENCODER_OK se la lettura è valida.
 */
Encoder_Status_t encoder_readRPM(EncoderHandle* enc)
{
    Encoder_Status_t status = ENCODER_ERR;

    if ((enc != NULL) && (enc->htim != NULL))
    {
        float diff;
        uint32_t cnt = __HAL_TIM_GET_COUNTER(enc->htim);

        if (enc->prev_count == cnt)
        {
            diff = 0.0f;
            enc->velocity = 0.0f;
        }
        /* Gestione Timer in Count-Down (Direzione Inversa/Positiva in questo setup) */
        else if (__HAL_TIM_IS_TIM_COUNTING_DOWN(enc->htim))
        {
            if (cnt < enc->prev_count)
            {
                diff = (float)(enc->prev_count - cnt);
            }
            else
            {
                diff = (float)((enc->arr - cnt) + enc->prev_count);
            }

            enc->velocity = (60.0f * diff) /
                            ((float)enc->calib.pulses_per_rev * enc->reading_period * enc->calib.gear_ratio);
        }
        /* Gestione Timer in Count-Up (Direzione Diretta/Negativa in questo setup) */
        else
        {
            if (cnt > enc->prev_count)
            {
                diff = ((float)cnt - (float)(enc->prev_count));
            }
            else
            {
                diff = (((float)(enc->arr) - (float)(enc->prev_count)) + (float)cnt);
            }

            enc->velocity = -(60.0f * diff) /
                            ((float)enc->calib.pulses_per_rev * enc->reading_period * enc->calib.gear_ratio);
        }

        /* * Correzione per Encoder Mode 3 (Conteggio su entrambi i fronti TI1 e TI2).
         * Se SMCR (Slave Mode Control Register) è impostato su Encoder Mode 3 (0x3),
         * il timer conta x4. Se la calibrazione PPR è basata su x2, normalizza dividendo.
         */
        if ((enc->htim->Instance->SMCR & 0x3U) == 0x3U)
        {
            enc->velocity /= 2.0f;
        }

        /* Aggiorna il contatore precedente per la prossima iterazione */
        enc->prev_count = __HAL_TIM_GET_COUNTER(enc->htim);

        status = ENCODER_OK;
    }

    return status;
}
