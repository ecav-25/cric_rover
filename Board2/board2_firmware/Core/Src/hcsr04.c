// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hcsr04.c
 * @author  Gruppo 2
 * @brief   Driver per il sensore a ultrasuoni HC-SR04.
 * @details Implementa la logica di misurazione della distanza tramite Input Capture.
 * Gestisce la generazione del trigger, la cattura dei fronti di salita/discesa del
 * segnale di eco e la conversione temporale considerando la frequenza del timer.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "hcsr04.h"

/** @brief Timeout massimo in microsecondi basato sulla distanza massima supportata (58us/cm). */
#define MAX_ECHO_US  (HCSR04_MAX_DISTANCE * 58U)

/** * @brief Mappa di lookup per associare i canali attivi del Timer alle istanze del sensore.
 * @note  La dimensione 16 copre le bitmask di HAL_TIM_ACTIVE_CHANNEL_x (1, 2, 4, 8).
 */
static hcsr04_t *ic_map[16];

/* ========================================================================== */
/* STATIC HELPER FUNCTIONS                                                    */
/* ========================================================================== */

/**
 * @brief Converte la costante del canale TIM generico nella bitmask del canale attivo HAL.
 * @param ch Canale del timer
 * @return uint32_t Bitmask del canale attivo
 */
static uint32_t tim_to_active(uint32_t ch)
{
    uint32_t active;

    switch (ch)
    {
        case TIM_CHANNEL_1:
            active = (uint32_t)HAL_TIM_ACTIVE_CHANNEL_1;
            break;
        case TIM_CHANNEL_2:
            active = (uint32_t)HAL_TIM_ACTIVE_CHANNEL_2;
            break;
        case TIM_CHANNEL_3:
            active = (uint32_t)HAL_TIM_ACTIVE_CHANNEL_3;
            break;
        case TIM_CHANNEL_4:
            active = (uint32_t)HAL_TIM_ACTIVE_CHANNEL_4;
            break;
        default:
            active = 0U;
            break;
    }

    return active;
}

/**
 * @brief Calcola la durata dell'impulso in tick del timer.
 * @details Gestisce correttamente il caso di **Timer Rollover** (overflow)
 * nel caso in cui il contatore (CNT) riparta da zero tra il fronte di salita
 * e quello di discesa.
 * * Se \f$ t_{fall} \ge t_{rise} \f$:
 * \f$ \Delta t = t_{fall} - t_{rise} \f$
 * * Se \f$ t_{fall} < t_{rise} \f$ (Rollover):
 * \f$ \Delta t = (ARR + 1 - t_{rise}) + t_{fall} \f$
 *
 * @param s Puntatore all'handle del sensore.
 * @return uint32_t Durata in tick.
 */
static inline uint32_t pulse_ticks(const hcsr04_t *s)
{
    uint32_t ticks;
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(s->cfg.htim);

    if (s->t_fall >= s->t_rise)
    {
        ticks = s->t_fall - s->t_rise;
    }
    else
    {
        uint32_t diff_to_arr = (arr + 1U) - s->t_rise;
        ticks = diff_to_arr + s->t_fall;
    }

    return ticks;
}

/**
 * @brief Converte i tick del timer in microsecondi.
 * @param s Puntatore all'handle del sensore.
 * @param ticks Numero di tick misurati.
 * @return uint32_t Tempo in microsecondi.
 */
static inline uint32_t ticks_to_us(const hcsr04_t *s, uint32_t ticks)
{
    return (uint32_t)((uint64_t)ticks * 1000000ULL / (uint64_t)s->cfg.timer_hz);
}

/**
 * @brief Converte i microsecondi in centimetri.
 * @details Assume la velocità del suono nell'aria a ~20°C (343 m/s).
 * La formula usata è:
 * \f$ Distanza_{cm} = \frac{Tempo_{\mu s}}{58} \f$
 * * @param us Tempo di volo in microsecondi.
 * @return uint16_t Distanza in cm.
 */
static inline uint16_t us_to_cm(uint32_t us)
{
    return (uint16_t)((us + 29U) / 58U); /* +29 per arrotondamento */
}

/**
 * @brief Genera l'impulso di Trigger di 10us via GPIO.
 * @param s Puntatore all'handle del sensore.
 */
static inline void trigger_pulse_10us(const hcsr04_t *s)
{
    HAL_GPIO_WritePin(s->cfg.trig_port, s->cfg.trig_pin, GPIO_PIN_SET);
    s->cfg.delay_us(10);
    HAL_GPIO_WritePin(s->cfg.trig_port, s->cfg.trig_pin, GPIO_PIN_RESET);
}

/* ========================================================================== */
/* PUBLIC API                                                                 */
/* ========================================================================== */

/**
 * @brief Inizializza il driver HC-SR04 e configura l'Input Capture.
 * @details Registra l'istanza nella mappa statica `ic_map` per la gestione
 * delle callback interrupt e configura la polarità iniziale (Rising Edge).
 * @param s Puntatore all'handle del sensore.
 * @param cfg Configurazione hardware.
 * @return hcsr04_status_t HCSR04_OK se inizializzazione riuscita.
 */
hcsr04_status_t HCSR04_Init(hcsr04_t *s, const hcsr04_cfg_t *cfg)
{
    hcsr04_status_t status = HCSR04_ERR_BAD_PARAM;

    if ((s != NULL) && (cfg != NULL) && (cfg->htim != NULL) &&
        (cfg->trig_port != NULL) && (cfg->delay_us != NULL) && (cfg->timer_hz != 0U))
    {
        s->cfg = *cfg;
        s->state = HCSR04_IDLE;
        s->t_rise = 0U;
        s->t_fall = 0U;
        s->last_status = HCSR04_ERR_NOT_READY;

        uint32_t active = tim_to_active(cfg->channel);

        if (active != 0U)
        {
            ic_map[active] = s;

            (void)HAL_TIM_IC_Start_IT(cfg->htim, cfg->channel);
            __HAL_TIM_SET_CAPTUREPOLARITY(cfg->htim, cfg->channel, TIM_INPUTCHANNELPOLARITY_RISING);

            status = HCSR04_OK;
        }
    }

    return status;
}

/**
 * @brief Avvia una nuova misurazione.
 * @details Genera il segnale di Trigger e imposta la macchina a stati in attesa
 * del fronte di salita dell'Eco.
 * @param s Puntatore all'handle del sensore.
 * @return hcsr04_status_t HCSR04_OK se il trigger è stato inviato.
 */
hcsr04_status_t HCSR04_Start(hcsr04_t *s)
{
    hcsr04_status_t status = HCSR04_ERR_BAD_PARAM;

    if (s != NULL)
    {
        if (s->state != HCSR04_IDLE)
        {
            status = HCSR04_ERR_BUSY;
        }
        else
        {
            s->last_status = HCSR04_ERR_NOT_READY;
            s->state = HCSR04_WAIT_RISE;

            __HAL_TIM_SET_CAPTUREPOLARITY(s->cfg.htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);

            trigger_pulse_10us(s);
            status = HCSR04_OK;
        }
    }

    return status;
}

/**
 * @brief Calcola e restituisce la distanza misurata.
 * @details Se la misura è completata (IDLE e stato OK), converte i tempi acquisiti in cm.
 * Se chiamata mentre il sensore è in timeout o errore, restituisce codici di errore.
 * @param s Puntatore all'handle del sensore.
 * @param cm Puntatore alla variabile di destinazione.
 * @return hcsr04_status_t Esito del calcolo.
 */
hcsr04_status_t HCSR04_GetDistanceCm(hcsr04_t *s, uint16_t *cm)
{
    hcsr04_status_t status = HCSR04_ERR_BAD_PARAM;

    if ((s != NULL) && (cm != NULL))
    {
        if (s->state != HCSR04_IDLE)
        {
            s->state = HCSR04_IDLE;
            __HAL_TIM_SET_CAPTUREPOLARITY(s->cfg.htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);

            s->last_status = HCSR04_ERR_TIMEOUT;
            *cm = HCSR04_MAX_DISTANCE;
            status = HCSR04_ERR_TIMEOUT;
        }
        else if (s->last_status == HCSR04_OK)
        {
            uint32_t ticks = pulse_ticks(s);
            uint32_t us = ticks_to_us(s, ticks);

            if (us > MAX_ECHO_US)
            {
                *cm = HCSR04_MAX_DISTANCE;
            }
            else
            {
                *cm = us_to_cm(us);
            }

            status = HCSR04_OK;
        }
        else
        {
            *cm = 0U;
            status = s->last_status;
        }
    }

    return status;
}

/**
 * @brief Callback dell'interrupt Input Capture.
 * @details Implementa la macchina a stati:
 * 1. HCSR04_WAIT_RISE: Cattura t_rise, cambia polarità in Falling.
 * 2. HCSR04_WAIT_FALL: Cattura t_fall, torna a IDLE, ripristina polarità Rising.
 * @param htim Handle del timer chiamante.
 */
void HCSR04_IC_Callback(TIM_HandleTypeDef *htim)
{
    if (htim != NULL)
    {
        uint32_t channel_idx = (uint32_t)htim->Channel;
        hcsr04_t *s = ic_map[channel_idx];

        if (s != NULL)
        {
            if (s->state == HCSR04_WAIT_RISE)
            {
                s->t_rise = (uint32_t)HAL_TIM_ReadCapturedValue(htim, s->cfg.channel);
                s->state = HCSR04_WAIT_FALL;
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_FALLING);
            }
            else if (s->state == HCSR04_WAIT_FALL)
            {
                s->t_fall = (uint32_t)HAL_TIM_ReadCapturedValue(htim, s->cfg.channel);
                s->state = HCSR04_IDLE;
                s->last_status = HCSR04_OK;
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);
            }
            else
            {
                /* MISRA Default case*/
            }
        }
    }
}
