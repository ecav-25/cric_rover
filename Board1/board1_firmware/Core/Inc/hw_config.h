#ifndef INC_MOTOR_CONFIG_H_
#define INC_MOTOR_CONFIG_H_

#include "motor.h"

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

#endif
