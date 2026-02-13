// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hw_config.c
 * @author  Gruppo 2
 * @brief   Hardware configuration definition for Board 2 (Master).
 * @details This file implements the hardware abstraction layer configuration,
 * mapping physical resources (Timers, I2C, GPIOs) to logical driver instances
 * (Motors, IMU, Ultrasonic Sensors, Controller).
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "hw_config.h"
#include "hw_calibration.h"
#include "tim.h"
#include "dwt_delay.h"

/* ========================================================================== */
/* MOTORS CONFIGURATION                                                       */
/* ========================================================================== */

/**
 * @brief Hardware configuration table for Motors (Board 2).
 * @details Maps logical motor IDs to TIM1 channels.
 * Note: Board 2 controls motors directly via PWM on TIM1.
 */
const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT] = {
    [MOTOR_FA] = { &htim1, TIM_CHANNEL_1, { PWM_STOP_FA, PWM_SCALE_FORWARD_FA, PWM_SCALE_BACKWARD_FA } },
    [MOTOR_FB] = { &htim1, TIM_CHANNEL_2, { PWM_STOP_FB, PWM_SCALE_FORWARD_FB, PWM_SCALE_BACKWARD_FB } },
    [MOTOR_BA] = { &htim1, TIM_CHANNEL_3, { PWM_STOP_BA, PWM_SCALE_FORWARD_BA, PWM_SCALE_BACKWARD_BA } },
    [MOTOR_BB] = { &htim1, TIM_CHANNEL_4, { PWM_STOP_BB, PWM_SCALE_FORWARD_BB, PWM_SCALE_BACKWARD_BB } },
};

/* ========================================================================== */
/* MPU6050 CONFIGURATION                                                      */
/* ========================================================================== */

/**
 * @brief Hardware configuration table for IMU (MPU6050).

 */
const MPU_HW_Config_t MPU_HW_CONFIG[MPU_COUNT] = {
    [MPU_MAIN] = {
        .i2c     = &hi2c1,
        .address = MPU60X0_ADDRESS,
        .cfg = {
            .dlpf_cfg    = MPU6050_DEFAULT_DLPF_CFG, /**< Digital Low Pass Filter setting. */
            .smplrt_div  = 4,                        /**< Sample Rate Divider (Fs = 200 Hz). */
            .accel_range = MPU6050_ACCEL_FS_2G,      /**< Accelerometer Full Scale Range. */
            .gyro_range  = MPU6050_GYRO_FS_250DPS,   /**< Gyroscope Full Scale Range. */
            .int_enable  = MPU6050_INT_NONE          /**< Interrupts disabled (Polling mode). */
        }
    }
};

/* ========================================================================== */
/* ULTRASONIC SENSORS CONFIGURATION                                           */
/* ========================================================================== */

/**
 * @brief Hardware configuration table for Ultrasonic Sensors (HC-SR04).
 * @details Maps 3 sensors to TIM2 Input Capture channels.
 * Requires DWT_Delay_us for precise trigger generation.
 */
const hcsr04_cfg_t ULTRASONIC_HW_CONFIG[US_COUNT] = {
	[US_LEFT] = {
		.htim        = &htim2,
		.channel     = TIM_CHANNEL_1,
		.trig_port   = TRIG_LEFT_GPIO_Port,
		.trig_pin    = TRIG_LEFT_Pin,
		.timer_hz    = 1000000,      /**< Timer frequency (1MHz = 1us tick). */
		.delay_us    = DWT_Delay_us  /**< Pointer to microsecond delay function. */
	},
	[US_CENTER] = {
		.htim        = &htim2,
		.channel     = TIM_CHANNEL_2,
		.trig_port   = TRIG_CENTER_GPIO_Port,
		.trig_pin    = TRIG_CENTER_Pin,
		.timer_hz    = 1000000,
		.delay_us    = DWT_Delay_us
	},
	[US_RIGHT] = {
		.htim        = &htim2,
		.channel     = TIM_CHANNEL_3,
		.trig_port   = TRIG_RIGHT_GPIO_Port,
		.trig_pin    = TRIG_RIGHT_Pin,
		.timer_hz    = 1000000,
		.delay_us    = DWT_Delay_us
	}
};

/* ========================================================================== */
/* CONTROLLER CONFIGURATION                                                   */
/* ========================================================================== */

/**
 * @brief Hardware configuration table for Remote Controller.
 * @details Communication via I2C1 with slave address 0x55.
 */
const Controller_HW_Config_t CONTROLLER_HW_CONFIG[CONTROLLER_COUNT] = {
	[CONTROLLER_MAIN] = {
		.i2c     = &hi2c1,
		.address = 0x55
	}
};
