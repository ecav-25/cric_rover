#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

#include "motor.h"
#include "encoder.h"


/* ===================== MOTORS ===================== */

typedef enum {
    MOTOR_FA = 0,
    MOTOR_FB,
    MOTOR_BA,
    MOTOR_BB,
    MOTOR_COUNT
} Motor_Id_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    Motor_Calibration_t calib;
} Motor_HW_Config_t;

extern const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT];

/* ===================== ENCODERS ===================== */

typedef enum {
    ENCODER_FA = 0,
    ENCODER_FB,
    ENCODER_BA,
    ENCODER_BB,
    ENCODER_COUNT
} Encoder_Id_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    Encoder_Calibration_t calib;
} Encoder_HW_Config_t;


extern const Encoder_HW_Config_t ENCODER_HW_CONFIG[ENCODER_COUNT];

/* ===================== ADC ===================== */

typedef enum {
	ADC_TEMP_SENSOR = 0,
	ADC_BATTERY_VOLTAGE,
	ADC_COUNT
} ADC_Id_t;

typedef struct {
	ADC_HandleTypeDef* hadc;
	ADC_ChannelConfTypeDef channel_cfg;
} ADC_HW_Config_t;

extern const ADC_HW_Config_t ADC_HW_CONFIG[ADC_COUNT];

#endif /* INC_HW_CONFIG_H_ */
