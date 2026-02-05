#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

#include "motor.h"
#include "tim.h"
#include "mpu6050.h"
#include "i2c.h"
#include "hcsr04.h"

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

/* ===================== MPU6050 ===================== */

typedef enum {
    MPU_MAIN = 0,
    MPU_COUNT
} MPU_Id_t;

typedef struct {
    I2C_HandleTypeDef* i2c;
    uint16_t address;
    mpu6050_config_t cfg;
} MPU_HW_Config_t;

extern const MPU_HW_Config_t MPU_HW_CONFIG[MPU_COUNT];


/* ===================== ULTRASONIC SENSORS ===================== */

typedef enum {
	US_LEFT = 0,
	US_CENTER,
	US_RIGHT,
	US_COUNT
} UltrasonicSensor_Id_t;


extern const hcsr04_cfg_t ULTRASONIC_HW_CONFIG[US_COUNT];


/* ===================== CONTROLLER ===================== */

typedef enum {
	CONTROLLER_MAIN = 0,
	CONTROLLER_COUNT
} Controller_Id_t;

typedef struct {
	I2C_HandleTypeDef* i2c;
	uint16_t address;
} Controller_HW_Config_t;

extern const Controller_HW_Config_t CONTROLLER_HW_CONFIG[CONTROLLER_COUNT];

#endif /* INC_HW_CONFIG_H_ */
