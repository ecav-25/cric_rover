#ifndef UTILS_H
#define UTILS_H

#include "rtwtypes.h"
#include "controller.h"  /* LightOperatingMode */
#include "Board2.h"      /* Board2_IN_Lights_* */

#define OPENLOOP_RAMP_SLOPE    (600.0f)
#define NORMAL_BRK_COEFF      (0.5f)

/* ===================== RAMP FUNCTION ===================== */

static inline real32_T ramp(real32_T current, real32_T target, real32_T step)
{
    real32_T next_value;

    if (current < (target - step))
    {
        next_value = current + step;
    }
    else if (current > (target + step))
    {
        next_value = current - step;
    }
    else
    {
        next_value = target;
    }

    return next_value;
}

/* ===================== TELEMETRY HELPERS ===================== */

static inline uint8_T calculate_battery_percent(real32_T voltage)
{
    uint8_T percent;

    if (voltage <= 9.0f)
    {
        percent = 0U;
    }
    else if (voltage >= 12.6f)
    {
        percent = 100U;
    }
    else
    {
        real32_T result = (((voltage - 9.0f) / 3.6f) * 100.0f) + 0.5f;
        percent = (uint8_T)result;
    }

    return percent;
}

static inline int8_T saturate_temperature(real32_T temp)
{
    int8_T result;

    if (temp > 127.0f)
    {
        result = 127;
    }
    else if (temp < -128.0f)
    {
        result = -128;
    }
    else
    {
    	real32_T rounding_offset = (temp >= 0.0f) ? 0.5f : -0.5f;
    	real32_T rounded_val = temp + rounding_offset;
    	result = (int8_T)rounded_val;
    }

    return result;
}

static inline LightOperatingMode get_light_mode(uint32_T state)
{
    LightOperatingMode mode;

    if (state == Board2_IN_Lights_AUTO)
    {
        mode = LIGHT_AUTO;
    }
    else if (state == Board2_IN_Lights_ON)
    {
        mode = LIGHT_ON;
    }
    else
    {
        mode = LIGHT_OFF;
    }

    return mode;
}

static inline DrivingMode get_driving_mode(uint32_T state)
{
    DrivingMode mode;

    if (state == Board2_IN_Mode_SPORT)
    {
        mode = DM_SPORT;
    }
    else if (state == Board2_IN_Mode_ECO)
    {
        mode = DM_ECO;
    }
    else
    {
        mode = DM_DEFAULT;
    }

    return mode;
}


#endif /* UTILS_H */
