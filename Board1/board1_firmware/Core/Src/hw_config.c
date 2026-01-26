#include "hw_config.h"
#include "hw_calibration.h"
#include "tim.h"
#include "adc.h"

/* ===================== MOTORS ===================== */

const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT] = {
    [MOTOR_FA] = { &htim3, TIM_CHANNEL_3, { PWM_STOP_FA, PWM_SCALE_FORWARD_FA, PWM_SCALE_BACKWARD_FA } },
    [MOTOR_FB] = { &htim3, TIM_CHANNEL_2, { PWM_STOP_FB, PWM_SCALE_FORWARD_FB, PWM_SCALE_BACKWARD_FB } },
    [MOTOR_BA] = { &htim3, TIM_CHANNEL_1, { PWM_STOP_BA, PWM_SCALE_FORWARD_BA, PWM_SCALE_BACKWARD_BA } },
    [MOTOR_BB] = { &htim3, TIM_CHANNEL_4, { PWM_STOP_BB, PWM_SCALE_FORWARD_BB, PWM_SCALE_BACKWARD_BB } },
};

/* ===================== ENCODERS ===================== */

const Encoder_HW_Config_t ENCODER_HW_CONFIG[ENCODER_COUNT] = {
    [ENCODER_FA] = { &htim5,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_FB] = { &htim20, { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_BA] = { &htim1,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
    [ENCODER_BB] = { &htim8,  { PULSES_PER_REVOLUTION, GEAR_RATIO } },
};

/* ===================== ADC ===================== */

const ADC_HW_Config_t ADC_HW_CONFIG[ADC_COUNT] = {
	[ADC_TEMP_SENSOR] = { &hadc1, { ADC_CHANNEL_TEMPSENSOR_ADC1, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_247CYCLES_5, ADC_SINGLE_ENDED, ADC_OFFSET_NONE, 0U } },
	[ADC_BATTERY_VOLTAGE] = { &hadc1, { ADC_CHANNEL_9, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_247CYCLES_5, ADC_SINGLE_ENDED, ADC_OFFSET_NONE, 0U } },
};





