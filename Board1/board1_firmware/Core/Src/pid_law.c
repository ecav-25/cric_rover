#include "pid_law.h"
#include <stddef.h>

pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1)
{
    if (pid == NULL)
    {
        return PID_LAW_ERR;
    }

    pid->a1 = a1;
    pid->b0 = b0;
    pid->b1 = b1;

    return PID_Law_reset(pid);
}

pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out)
{
    if (pid == NULL || u_out == NULL)
    {
        return PID_LAW_ERR;
    }

    float e_now = setpoint - feedback;

    /* Equazione alle differenze: u[k] = -a1*u[k-1] + b0*e[k] + b1*e[k-1] */
    float u_unbounded = (-pid->a1 * pid->u_prev) + (pid->b0 * e_now) + (pid->b1 * pid->e_prev);

    /* Saturazione (Anti-windup) */
    if (u_unbounded > U_MAX)
    {
        *u_out = U_MAX;
    }
    else if (u_unbounded < U_MIN)
    {
        *u_out = U_MIN;
    }
    else
    {
        *u_out = u_unbounded;
    }

    /* Aggiornamento stati per il prossimo passo */
    pid->u_prev = *u_out;
    pid->e_prev = e_now;

    return PID_LAW_OK;
}

pid_law_status PID_Law_reset(PID_Law_t* pid)
{
    if (pid == NULL)
    {
        return PID_LAW_ERR;
    }

    pid->u_prev = 0.0f;
    pid->e_prev = 0.0f;

    return PID_LAW_OK;
}
