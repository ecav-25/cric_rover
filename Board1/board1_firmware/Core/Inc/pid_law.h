#ifndef PID_LAW_H_
#define PID_LAW_H_

#define U_MAX (12.0f)
#define U_MIN (-12.0f)

typedef enum {
    PID_LAW_OK  = 0,
    PID_LAW_ERR = -1
} pid_law_status;

typedef struct {
    float a1;
    float b0;
    float b1;

    float u_prev;
    float e_prev;
} PID_Law_t;

pid_law_status PID_Law_init(PID_Law_t* pid, float a1, float b0, float b1);

pid_law_status PID_Law_compute(PID_Law_t* pid, float setpoint, float feedback, float* u_out);

pid_law_status PID_Law_reset(PID_Law_t* pid);

#endif
