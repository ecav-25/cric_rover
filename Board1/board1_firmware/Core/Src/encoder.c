#include "encoder.h"
#include "gpio.h"

void encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, uint8_t pulse_per_revolution, float gear_motor_ratio, float reading_period){
	enc->htim = htim;
	enc->prev_count = __HAL_TIM_GET_COUNTER(enc->htim);
	HAL_TIM_Encoder_Start(enc->htim, TIM_CHANNEL_ALL);

	if(gear_motor_ratio == 0){
		enc->gear_motor_ratio = GEAR_RATIO;
	}else{
		enc->gear_motor_ratio = gear_motor_ratio;
	}

	if(pulse_per_revolution == 0){
		enc->pulse_per_revolution = PULSES_PER_REVOLUTION;
	}else{
		enc->pulse_per_revolution = pulse_per_revolution;
	}

	enc->arr = __HAL_TIM_GET_AUTORELOAD(htim);
	enc->reading_period = reading_period/1000;
	enc->velocity = 0;
}

void encoder_readRPM(EncoderHandle* enc){
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

	  enc->velocity = (60.0 * diff) /
	          (enc->pulse_per_revolution * enc->reading_period * enc->gear_motor_ratio);
	}

	else {

	  if (cnt > enc->prev_count)
		  diff = cnt - enc->prev_count;
	  else
		  diff = (enc->arr - enc->prev_count) + cnt;

	  enc->velocity = -(60.0 * diff) /
			  (enc->pulse_per_revolution * enc->reading_period * enc->gear_motor_ratio);
	}

	if ((enc->htim->Instance->SMCR & 0x3) == 0x3)
		enc->velocity /= 2;

	enc->prev_count = __HAL_TIM_GET_COUNTER(enc->htim);
}

