#include "hcsr04.h"


#define MAX_ECHO_US  (HCSR04_MAX_DISTANCE * 58U)

static hcsr04_t *ic_map[16];



static uint32_t tim_to_active(uint32_t ch)
{
    switch (ch) {
        case TIM_CHANNEL_1: return HAL_TIM_ACTIVE_CHANNEL_1;
        case TIM_CHANNEL_2: return HAL_TIM_ACTIVE_CHANNEL_2;
        case TIM_CHANNEL_3: return HAL_TIM_ACTIVE_CHANNEL_3;
        case TIM_CHANNEL_4: return HAL_TIM_ACTIVE_CHANNEL_4;
        default: return 0;
    }
}

static inline uint32_t pulse_ticks(const hcsr04_t *s)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(s->cfg.htim);
    if (s->t_fall >= s->t_rise) return s->t_fall - s->t_rise;

    return (arr + 1U - s->t_rise) + s->t_fall;
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
    if (!s || !cfg || !cfg->htim || !cfg->trig_port || !cfg->delay_us || cfg->timer_hz == 0)
        return HCSR04_ERR_BAD_PARAM;

    s->cfg = *cfg;
    s->state = HCSR04_IDLE;
    s->t_rise = 0;
    s->t_fall = 0;
    s->last_status = HCSR04_ERR_NOT_READY;

    uint32_t active = tim_to_active(cfg->channel);
    if (active == 0) return HCSR04_ERR_BAD_PARAM;
    ic_map[active] = s;

    HAL_TIM_IC_Start_IT(cfg->htim, cfg->channel);
    __HAL_TIM_SET_CAPTUREPOLARITY(cfg->htim, cfg->channel, TIM_INPUTCHANNELPOLARITY_RISING);

    return HCSR04_OK;
}

hcsr04_status_t HCSR04_Start(hcsr04_t *s)
{
    if (!s) return HCSR04_ERR_BAD_PARAM;
    if (s->state != HCSR04_IDLE) return HCSR04_ERR_BUSY;

    s->last_status = HCSR04_ERR_NOT_READY;
    s->state = HCSR04_WAIT_RISE;


    __HAL_TIM_SET_CAPTUREPOLARITY(s->cfg.htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);

    trigger_pulse_10us(s);
    return HCSR04_OK;
}

hcsr04_status_t HCSR04_GetDistanceCm(hcsr04_t *s, uint16_t *cm)
{
    if (!s || !cm) return HCSR04_ERR_BAD_PARAM;


    if (s->state != HCSR04_IDLE) {

        s->state = HCSR04_IDLE;
        __HAL_TIM_SET_CAPTUREPOLARITY(s->cfg.htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);

        s->last_status = HCSR04_ERR_TIMEOUT;
        *cm = HCSR04_MAX_DISTANCE;
        return HCSR04_ERR_TIMEOUT;
    }


    if (s->last_status == HCSR04_OK) {
        uint32_t ticks = pulse_ticks(s);
        uint32_t us = ticks_to_us(s, ticks);


        if (us > MAX_ECHO_US) {
            *cm = HCSR04_MAX_DISTANCE;

            return HCSR04_OK;
        }

        *cm = us_to_cm(us);
        return HCSR04_OK;
    }


    *cm = 0;
    return s->last_status;
}



void HCSR04_IC_Callback(TIM_HandleTypeDef *htim)
{
    hcsr04_t *s = ic_map[htim->Channel];
    if (!s) return;


    if (s->state == HCSR04_WAIT_RISE) {
        s->t_rise = HAL_TIM_ReadCapturedValue(htim, s->cfg.channel);
        s->state = HCSR04_WAIT_FALL;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_FALLING);
        return;
    }


    if (s->state == HCSR04_WAIT_FALL) {
        s->t_fall = HAL_TIM_ReadCapturedValue(htim, s->cfg.channel);

        s->state = HCSR04_IDLE;
        s->last_status = HCSR04_OK;


        __HAL_TIM_SET_CAPTUREPOLARITY(htim, s->cfg.channel, TIM_INPUTCHANNELPOLARITY_RISING);
        return;
    }
}
