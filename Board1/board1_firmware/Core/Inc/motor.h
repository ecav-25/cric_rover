#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include <stdint.h>

/* -------------------- Limiti duty -------------------- */
#define MOTOR_MAX_DUTY 100u
#define MOTOR_MIN_DUTY 0u

/* -------------------- Return status -------------------- */
typedef enum {
    MOTOR_OK = 0,
    MOTOR_ERR = -1
} Motor_Status_t;

/* -------------------- Direzione -------------------- */
typedef enum {
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
} Motor_Direction_t;

/* -------------------- Calibrazione PWM -------------------- */
typedef struct {
    uint16_t pwm_stop;
    uint16_t pwm_scale_forward;
    uint16_t pwm_scale_backward;
} Motor_Calibration_t;

/* -------------------- Handle motore -------------------- */
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    Motor_Calibration_t calib;
} Motor_t;

/* -------------------- API -------------------- */
Motor_Status_t motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, const Motor_Calibration_t* calib);

Motor_Status_t motor_set(Motor_t* motor, uint8_t duty, Motor_Direction_t dir);

#endif /* INC_MOTOR_H_ */
