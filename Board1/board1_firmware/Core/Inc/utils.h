#ifndef RAMP_H_
#define RAMP_H_

#include "rtwtypes.h"
#include "Board1_types.h"

#define RAMP_SLOPE_DEFAULT_RPM_S   (600.0f)
#define RAMP_SLOPE_SPORT_RPM_S     (1000.0f)
#define RAMP_SLOPE_ECO_RPM_S       (300.0f)

#define NORMAL_BRK_COEFF           (0.5f)

static inline real32_T ramp(real32_T current, real32_T target, real32_T step, BRAKING_TYPE braking_mode){
    real32_T effective_step = step;

    /* Gestione braking */
    if ((braking_mode == EMERGENCY) && (target == 0.0f)) {
        return target;
    }

    if ((braking_mode == NORMAL) && (target == 0.0f)) {
        effective_step = step * NORMAL_BRK_COEFF;
    }

    /* Ramping vero e proprio */
    if (current < (target - effective_step)) {
        return current + effective_step;
    }

    if (current > (target + effective_step)) {
        return current - effective_step;
    }

    return target;
}


#endif /* RAMP_H_ */
