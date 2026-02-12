#include "motor.h"
#include <math.h>

/* -------------------- Helpers -------------------- */

static inline uint8_t clip_duty(uint8_t duty)
{
    uint8_t res = duty;

    if (res > MOTOR_MAX_DUTY)
    {
        res = MOTOR_MAX_DUTY;
    }

    return res;
}

static uint16_t compute_pwm(const Motor_t* m, uint8_t duty, Motor_Direction_t dir){
    uint16_t pwm;

    if (duty == MOTOR_MIN_DUTY){
        pwm = m->calib.pwm_stop;
    }

    else if (dir == CLOCKWISE){
        pwm = m->calib.pwm_stop + (uint16_t)roundf((float)duty * m->calib.pwm_scale_forward / MOTOR_MAX_DUTY);
    }

    else if (dir == COUNTERCLOCKWISE){
        pwm = m->calib.pwm_stop - (uint16_t)roundf((float)duty * m->calib.pwm_scale_backward / MOTOR_MAX_DUTY);
    }

    else{
        pwm = m->calib.pwm_stop;
    }

    return pwm;
}

static inline void apply_pwm(const Motor_t* m, uint16_t value){
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, value);
}

/* -------------------- API -------------------- */

Motor_Status_t motor_init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, const Motor_Calibration_t* calib)
{
    Motor_Status_t status = MOTOR_ERR;

    if ((motor != NULL) && (htim != NULL) && (calib != NULL))
    {
        motor->htim = htim;
        motor->channel = channel;
        motor->calib = *calib;

        apply_pwm(motor, calib->pwm_stop);
        (void)HAL_TIM_PWM_Start(htim, channel);

        status = MOTOR_OK;
    }

    return status;
}

Motor_Status_t motor_set(const Motor_t* motor, uint8_t duty, Motor_Direction_t dir)
{
    Motor_Status_t status = MOTOR_ERR;

    if (motor != NULL)
    {
        uint8_t safe_duty = clip_duty(duty);

        apply_pwm(motor, compute_pwm(motor, safe_duty, dir));

        status = MOTOR_OK;
    }

    return status;
}
