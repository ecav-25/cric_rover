#include "encoder.h"
#include "gpio.h"

Encoder_Status_t encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, const Encoder_Calibration_t* calib, float reading_period_ms)
{
    if ((enc == NULL) || (htim == NULL) || (calib == NULL)) {
        return ENCODER_ERR;
    }

    enc->htim = htim;
    enc->prev_count = __HAL_TIM_GET_COUNTER(htim);
    enc->arr = __HAL_TIM_GET_AUTORELOAD(htim);
    enc->reading_period = reading_period_ms / 1000.0f;
    enc->calib = *calib;
    enc->velocity = 0.0f;

    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    return ENCODER_OK;
}




Encoder_Status_t encoder_readRPM(EncoderHandle* enc){

	if ((enc == NULL) || (enc->htim == NULL)) {
		return ENCODER_ERR;
	}

	float diff;
	uint32_t cnt = __HAL_TIM_GET_COUNTER(enc->htim);

	if(enc->prev_count == cnt){
	  diff = 0;
	  enc->velocity = 0;
	}

	else if (__HAL_TIM_IS_TIM_COUNTING_DOWN(enc->htim)) {

	  if (cnt < enc->prev_count)
		  diff = enc->prev_count - cnt;

	  else
		  diff = (enc->arr - cnt) + enc->prev_count;

	  enc->velocity = (60.0f * diff) /
	                  (enc->calib.pulses_per_rev * enc->reading_period * enc->calib.gear_ratio);
	}

	else {
	  if (cnt > enc->prev_count)
		  diff = cnt - enc->prev_count;
	  else
		  diff = (enc->arr - enc->prev_count) + cnt;

	  enc->velocity = -(60.0 * diff) /
			  (enc->calib.pulses_per_rev * enc->reading_period * enc->calib.gear_ratio);
	}

	if ((enc->htim->Instance->SMCR & 0x3) == 0x3)
		enc->velocity /= 2;

	enc->prev_count = __HAL_TIM_GET_COUNTER(enc->htim);

    return ENCODER_OK;
}

