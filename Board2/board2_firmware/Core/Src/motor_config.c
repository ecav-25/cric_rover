#include "motor_config.h"
#include "motor_calibration.h"
#include "tim.h"

/* ===== Board2 motor hardware configuration ===== */

const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT] = {
    [MOTOR_FA] = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_1,
        .calib = {
            PWM_STOP_FA,
            PWM_SCALE_FORWARD_FA,
            PWM_SCALE_BACKWARD_FA
        }
    },
    [MOTOR_FB] = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_2,
        .calib = {
            PWM_STOP_FB,
            PWM_SCALE_FORWARD_FB,
            PWM_SCALE_BACKWARD_FB
        }
    },
    [MOTOR_BA] = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_3,
        .calib = {
            PWM_STOP_BA,
            PWM_SCALE_FORWARD_BA,
            PWM_SCALE_BACKWARD_BA
        }
    },
    [MOTOR_BB] = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_4,
        .calib = {
            PWM_STOP_BB,
            PWM_SCALE_FORWARD_BB,
            PWM_SCALE_BACKWARD_BB
        }
    }
};
