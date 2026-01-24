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
