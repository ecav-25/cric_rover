#include "hw_config.h"
#include "hw_calibration.h"
#include "tim.h"

/* ===================== MOTORS ===================== */

const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT] = {
    [MOTOR_FA] = { &htim1, TIM_CHANNEL_1, { PWM_STOP_FA, PWM_SCALE_FORWARD_FA, PWM_SCALE_BACKWARD_FA } },
    [MOTOR_FB] = { &htim1, TIM_CHANNEL_2, { PWM_STOP_FB, PWM_SCALE_FORWARD_FB, PWM_SCALE_BACKWARD_FB } },
    [MOTOR_BA] = { &htim1, TIM_CHANNEL_3, { PWM_STOP_BA, PWM_SCALE_FORWARD_BA, PWM_SCALE_BACKWARD_BA } },
    [MOTOR_BB] = { &htim1, TIM_CHANNEL_4, { PWM_STOP_BB, PWM_SCALE_FORWARD_BB, PWM_SCALE_BACKWARD_BB } },
};

/* ===================== MPU6050 ===================== */

const MPU_HW_Config_t MPU_HW_CONFIG[MPU_COUNT] = {
    [MPU_MAIN] = {
        .i2c     = &hi2c1,
        .address = MPU60X0_ADDRESS,
        .cfg = {
            .dlpf_cfg    = MPU6050_DEFAULT_DLPF_CFG,
            .smplrt_div  = 4,                     // Fs = 200 Hz
            .accel_range = MPU6050_ACCEL_FS_2G,
            .gyro_range  = MPU6050_GYRO_FS_250DPS,
            .int_enable  = MPU6050_INT_NONE
        }
    }
};
