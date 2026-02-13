// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    temp.c
 * @author  Gruppo 2
 * @brief   Driver per la lettura della temperatura tramite ADC.
 * @details Questo modulo gestisce la lettura di un sensore di temperatura (tipicamente
 * il sensore interno STM32 o un termistore lineare) convertendo il valore grezzo ADC
 * in gradi Celsius basandosi sui parametri di calibrazione V25 e Slope.
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "temp.h"

/* ========================================================================== */
/* STATIC SUPPORT FUNCTIONS                                                   */
/* ========================================================================== */

/**
 * @brief Configura il canale ADC specifico per la lettura della temperatura.
 * @param t Puntatore alla struttura di controllo del sensore.
 * @return Temp_Status_t TEMP_OK se la configurazione HAL ha successo.
 */
static Temp_Status_t temp_cfg_channel_(temp_t *t)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (t->hadc != NULL))
    {
        if (HAL_ADC_ConfigChannel(t->hadc, &t->channel_cfg) == HAL_OK)
        {
            status = TEMP_OK;
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}

/**
 * @brief Esegue una singola conversione ADC in modalità polling.
 * @param t Puntatore alla struttura di controllo.
 * @param raw Puntatore alla variabile dove salvare il valore grezzo.
 * @return Temp_Status_t TEMP_OK se la conversione è valida.
 */
static Temp_Status_t temp_read_once_(temp_t *t, uint32_t *raw)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (raw != NULL) && (t->hadc != NULL))
    {
        if (HAL_ADC_Start(t->hadc) == HAL_OK)
        {
            if (HAL_ADC_PollForConversion(t->hadc, t->timeout_ms) == HAL_OK)
            {
                *raw = HAL_ADC_GetValue(t->hadc);
                status = TEMP_OK;
            }
            else
            {
                status = TEMP_ERR_COMM;
            }

            (void)HAL_ADC_Stop(t->hadc);
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}

/**
 * @brief Converte il valore grezzo ADC in gradi Celsius.
 * @details La conversione utilizza i parametri di calibrazione tipici dei sensori interni STM32:
 * 1. Calcolo della tensione letta ($V_{sense}$):
 * $$V_{sense} = \frac{RAW \cdot V_{REF}}{ADC_{max}}$$
 * 2. Conversione in temperatura basata sulla tensione a 25°C ($V_{25}$) e pendenza ($Avg\_Slope$):
 * $$T [^{\circ}C] = \frac{V_{sense} - V_{25}}{Avg\_Slope} + 25$$
 *
 * @param raw Valore grezzo ADC (0 .. 4095 per 12-bit).
 * @return float Temperatura in gradi Celsius.
 */
static float temp_raw_to_celsius_(uint32_t raw)
{
    float v_sense = 0.0f;

    /* Calcolo tensione in mV */
    v_sense = ((float)raw * TEMP_ADC_VREF_MV) / TEMP_ADC_MAX_CNT;

    /* Calcolo temperatura lineare */
    return ((v_sense - TEMP_V25_MV) / TEMP_SLOPE_MV_PER_C) + 25.0f;
}

/**
 * @brief Gestisce la lettura interna con supporto per il sovracampionamento (media).
 * @param t Puntatore alla struttura di controllo.
 * @param samples Numero di campioni da acquisire e mediare.
 * @param raw Puntatore al buffer di destinazione per il valore medio grezzo.
 * @return Temp_Status_t Esito dell'operazione.
 */
static Temp_Status_t temp_read_raw_internal_(temp_t *t, uint8_t samples, uint32_t *raw)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (raw != NULL) && (samples != 0U))
    {
        /* Configura il canale prima di iniziare la sequenza di letture */
        status = temp_cfg_channel_(t);

        if (status == TEMP_OK)
        {
            uint32_t acc = 0U;
            uint32_t sample = 0U;

            /* Ciclo di acquisizione */
            for (uint8_t i = 0U; (i < samples) && (status == TEMP_OK); i++)
            {
                status = temp_read_once_(t, &sample);

                if (status == TEMP_OK)
                {
                    acc += sample;
                }
            }

            /* Calcolo media finale */
            if (status == TEMP_OK)
            {
                *raw = acc / (uint32_t)samples;
                t->raw_last = *raw;
            }
        }
    }

    return status;
}


/* ========================================================================== */
/* PUBLIC API                                                                 */
/* ========================================================================== */

/**
 * @brief Inizializza il driver di temperatura e calibra l'ADC.
 * @param t Puntatore all'handle del sensore di temperatura.
 * @param hadc Handle dell'ADC hardware.
 * @param channel_cfg Configurazione del canale ADC associato al sensore.
 * @param timeout_ms Timeout massimo per le operazioni di polling.
 * @return Temp_Status_t TEMP_OK se inizializzazione riuscita.
 */
Temp_Status_t temp_init(temp_t *t, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef* channel_cfg, uint32_t timeout_ms)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (hadc != NULL) && (channel_cfg != NULL))
    {
        t->hadc = hadc;
        t->channel_cfg = *channel_cfg;
        t->timeout_ms = timeout_ms;

        t->raw_last = 0U;
        t->temp_c_last = 0.0f;

        /* Avvio calibrazione ADC per migliorare la precisione */
        if (HAL_ADCEx_Calibration_Start(t->hadc, t->channel_cfg.SingleDiff) == HAL_OK)
        {
            status = TEMP_OK;
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}

/**
 * @brief Legge un singolo campione grezzo dal sensore.
 * @param t Puntatore all'handle.
 * @param raw Puntatore dove salvare il valore ADC.
 * @return Temp_Status_t Esito della lettura.
 */
Temp_Status_t temp_read_raw_once(temp_t *t, uint32_t *raw)
{
    return temp_read_raw_internal_(t, 1, raw);
}

/**
 * @brief Acquisisce la temperatura e la converte in gradi Celsius.
 * @note Aggiorna automaticamente il campo @p temp_c_last nella struttura.
 * @param t Puntatore all'handle.
 * @param temp_c Puntatore (float) dove salvare la temperatura calcolata.
 * @return Temp_Status_t TEMP_OK se la lettura è valida.
 */
Temp_Status_t temp_get_celsius_once(temp_t *t, float *temp_c)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (temp_c != NULL))
    {
        uint32_t raw;

        status = temp_read_raw_once(t, &raw);

        if (status == TEMP_OK)
        {
            t->temp_c_last = temp_raw_to_celsius_(raw);
            *temp_c = t->temp_c_last;
        }
    }

    return status;
}
