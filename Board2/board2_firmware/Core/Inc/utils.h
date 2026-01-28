#ifndef UTILS_H
#define UTILS_H

#include "rtwtypes.h"
#include "controller.h"  /* LightOperatingMode */
#include "Board2.h"      /* Board2_IN_Lights_* */

#define OPENLOOP_RAMP_SLOPE    (600.0f)
#define NORMAL_BRK_COEFF      (0.5f)

/* ===================== RAMP FUNCTION ===================== */

real32_T ramp(real32_T current, real32_T target, real32_T step);

/* ===================== TELEMETRY HELPERS ===================== */

uint8_T calculate_battery_percent(real32_T voltage);

int8_T saturate_temperature(real32_T temp);

LightOperatingMode get_light_mode(uint32_T state);

DrivingMode get_driving_mode(uint32_T state);

#endif /* UTILS_H */
