#include "hcsr04.h"


#define MAX_ECHO_US  (HCSR04_MAX_DISTANCE * 58U)

static hcsr04_t *ic_map[16];



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

static inline uint32_t ticks_to_us(const hcsr04_t *s, uint32_t ticks)
{

    return (uint32_t)((uint64_t)ticks * 1000000ULL / (uint64_t)s->cfg.timer_hz);
}

static inline uint16_t us_to_cm(uint32_t us)
{

    return (uint16_t)((us + 29U) / 58U);
}

static inline void trigger_pulse_10us(const hcsr04_t *s)
{
    HAL_GPIO_WritePin(s->cfg.trig_port, s->cfg.trig_pin, GPIO_PIN_SET);
    s->cfg.delay_us(10);
    HAL_GPIO_WritePin(s->cfg.trig_port, s->cfg.trig_pin, GPIO_PIN_RESET);
}



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
                /*MISRA Default case*/
            }
        }
    }
}
