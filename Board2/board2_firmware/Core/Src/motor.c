#include "gpio.h"
#include "motor.h"
#include "tim.h"
#include "math.h"


void motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, uint16_t pwm_stop, uint16_t pwm_scale_forward, uint16_t pwm_scale_backward){
	motor->htim = htim;
	motor->channel = channel;
	motor->pwm_stop = pwm_stop;
	motor->pwm_scale_forward = pwm_scale_forward;
	motor->pwm_scale_backward = pwm_scale_backward;
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, 0);
	HAL_TIM_PWM_Start(motor->htim, motor->channel);
}


void motor_set(Motor_t* motor, uint8_t duty, wise_t dir) {

	uint16_t value = 0;

	if(duty > MAX_DUTY){
		duty = MAX_DUTY;
	}

	if(duty == MIN_DUTY){
		value = motor->pwm_stop;
	}
	else if(dir == CLOCKWISE){
		value = motor->pwm_stop + round((float)duty*motor->pwm_scale_forward/MAX_DUTY);
	}
	else if(dir == COUNTERCLOCKWISE){
		value = motor->pwm_stop - round((float)duty*motor->pwm_scale_backward/MAX_DUTY);
	}else{
		value = motor->pwm_stop;
	}
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, value);
}

