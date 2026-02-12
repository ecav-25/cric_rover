#include "pid_law.h"
#include <stddef.h>

pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1){
    if (pid == NULL){
    	return PID_LAW_ERR;
    }

    pid->a1 = a1;
    pid->b0 = b0;
    pid->b1 = b1;

    pid->u_prev = 0.0f;
    pid->e_prev = 0.0f;

    return PID_LAW_OK;
}

pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out){
    if (pid == NULL || u_out == NULL){
    	return PID_LAW_ERR;
    }

    float e = setpoint - feedback;

    float u = pid->a1 * pid->u_prev
            + pid->b0 * e
            + pid->b1 * pid->e_prev;

    if (u > U_MAX){
    	u = U_MAX;
    }
    else if (u < U_MIN){
    	u = U_MIN;
    }

    pid->u_prev = u;
    pid->e_prev = e;

    *u_out = u;

    return PID_LAW_OK;
}

pid_law_status PID_Law_reset(PID_Law_t* pid){
    if (pid == NULL){
    	return PID_LAW_ERR;
    }

    pid->u_prev = 0.0f;
    pid->e_prev = 0.0f;

    return PID_LAW_OK;
}
