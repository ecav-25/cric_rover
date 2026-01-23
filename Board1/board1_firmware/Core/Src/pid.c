#include "pid.h"

float error = 0;

void  PID_init(PID_t* pid, float kp, float ki, float kd, float period, float tau, float a1, float b0, float b1){
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->period = period/1000;
	pid->tau = tau;
	pid->prev_feedback = 0;
	pid->I_term = 0;
    pid->d_filt = 0;

    pid->u_prev = 0;
    pid->e_prev = 0;
    pid->a1 = a1;
    pid->b0 = b0;
    pid->b1 = b1;
}

float PID_compute(PID_t* pid, float setpoint, float feedback)
{
	error = setpoint - feedback;

	float P_term = pid->kp * error;

	float u_unsat = P_term + pid->I_term;
	float u_sat = u_unsat;

	if(u_unsat >= U_MAX) u_sat  = U_MAX;
	else if(u_unsat <= U_MIN) u_sat = U_MIN;

	pid->I_term += pid->period * (pid->ki * error + (u_sat - u_unsat));

	float d_raw = (feedback - pid->prev_feedback)/pid->period;
	float alpha = pid->tau / (pid->tau + pid->period);
	pid->d_filt = alpha * pid->d_filt + (1 - alpha) * d_raw;
	float D_term = pid->kd * pid->d_filt;

	float u = u_sat - D_term;

	pid->prev_feedback = feedback;

	return u;
}

float PID_compute_law(PID_t* pid, float setpoint, float feedback){
    // e[k]
    float e = setpoint - feedback;

    // Ricorrenza: u[k] = -a1*u[k-1] + b0*e[k] + b1*e[k-1]
    float u = pid->a1 * (pid->u_prev) + pid->b0 * e + pid->b1 * (pid->e_prev);

    // Saturazione
    if (u > U_MAX) u = U_MAX;
    else if (u < U_MIN) u = U_MIN;

    // Aggiorna stati
    pid->u_prev = u;
    pid->e_prev = e;

    return u;
}

void PID_reset(PID_t* pid)
{
	pid->I_term=0;
    pid->e_prev = 0.0f;
    pid->u_prev = 0.0f;
}
