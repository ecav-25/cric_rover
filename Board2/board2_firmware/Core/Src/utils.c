#include "utils.h"

/* ===================== RAMP ===================== */

real32_T ramp(real32_T current, real32_T target, real32_T step){
    if (current < target - step) {
        return current + step;
    }
    else if (current > target + step) {
        return current - step;
    }
    else {
        return target;
    }
}

/* ===================== TELEMETRY HELPERS ===================== */

uint8_T calculate_battery_percent(real32_T voltage){
    if (voltage <= 9.0f) {
        return 0;
    }
    if (voltage >= 12.6f) {
        return 100;
    }
    return (uint8_T)(((voltage - 9.0f) / 3.6f) * 100.0f + 0.5f);
}

int8_T saturate_temperature(real32_T temp){
    if (temp > 127.0f) {
        return 127;
    }
    if (temp < -128.0f) {
        return -128;
    }
    return (int8_T)(temp + (temp >= 0.0f ? 0.5f : -0.5f));
}

LightOperatingMode get_light_mode(uint32_T state){
    if (state == Board2_IN_Lights_AUTO) {
        return LIGHT_AUTO;
    }
    if (state == Board2_IN_Lights_ON) {
        return LIGHT_ON;
    }
    return LIGHT_OFF;
}

DrivingMode get_driving_mode(uint32_T state){
	if (state == Board2_IN_Mode_SPORT) {
		return DM_SPORT;
	}
	if (state == Board2_IN_Mode_ECO) {
		return DM_ECO;
	}
	return DM_DEFAULT;
}
