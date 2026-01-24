#ifndef PID_H_
#define PID_H_

#define U_MAX (12.0f)
#define U_MIN (-12.0f)

typedef enum {
    PID_OK  = 0,
    PID_ERR = -1
} pid_status;

typedef struct {
    float kp;
    float ki;
    float kd;

    float period;          // [s]
    float tau;             // costante filtro derivativo

    float prev_feedback;
    float I_term;
    float d_filt;
} PID_t;

pid_status PID_init(PID_t* pid, float kp, float ki, float kd, float period_ms, float tau);

pid_status PID_compute(PID_t* pid, float setpoint, float feedback, float* u_out);

pid_status PID_reset(PID_t* pid);

#endif
