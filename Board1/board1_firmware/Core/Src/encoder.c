#include "encoder.h"
#include "gpio.h"

Encoder_Status_t encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, const Encoder_Calibration_t* calib, float reading_period_ms)
{
    Encoder_Status_t status = ENCODER_ERR;

    if ((enc != NULL) && (htim != NULL) && (calib != NULL))
    {
        enc->htim = htim;
        enc->prev_count = __HAL_TIM_GET_COUNTER(htim);
        enc->arr = __HAL_TIM_GET_AUTORELOAD(htim);
        enc->reading_period = reading_period_ms / 1000.0f;
        enc->calib = *calib;
        enc->velocity = 0.0f;

        (void)HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);

        status = ENCODER_OK;
    }

    return status;
}



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

        if ((enc->htim->Instance->SMCR & 0x3U) == 0x3U)
        {
            enc->velocity /= 2.0f;
        }

        enc->prev_count = __HAL_TIM_GET_COUNTER(enc->htim);

        status = ENCODER_OK;
    }

    return status;
}

