#include "pid.h"
#include <stddef.h>

pid_status PID_init(PID_t* pid, float kp, float ki, float kd, float period_ms, float tau){
    if (pid == NULL){
    	return PID_ERR;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->period = period_ms / 1000.0f;
    pid->tau = tau;

    pid->prev_feedback = 0.0f;
    pid->I_term = 0.0f;
    pid->d_filt = 0.0f;

    return PID_OK;
}

pid_status PID_compute(PID_t* pid, float setpoint, float feedback, float* u_out){
    if (pid == NULL || u_out == NULL){
    	return PID_ERR;
    }

    float error = setpoint - feedback;

    float P = pid->kp * error;

    float u_unsat = P + pid->I_term;
    float u_sat = u_unsat;

    if (u_unsat > U_MAX){
    	u_sat = U_MAX;
    }
    else if (u_unsat < U_MIN){
    	u_sat = U_MIN;
    }

    pid->I_term += pid->period * (pid->ki * error + (u_sat - u_unsat));

    float d_raw = (feedback - pid->prev_feedback) / pid->period;
    float alpha = pid->tau / (pid->tau + pid->period);
    pid->d_filt = alpha * pid->d_filt + (1.0f - alpha) * d_raw;

    float D = pid->kd * pid->d_filt;

    *u_out = u_sat - D;

    pid->prev_feedback = feedback;

    return PID_OK;
}

pid_status PID_reset(PID_t* pid)
{
    if (pid == NULL) return PID_ERR;

    pid->I_term = 0.0f;
    pid->d_filt = 0.0f;
    pid->prev_feedback = 0.0f;

    return PID_OK;
}
