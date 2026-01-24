#include "ramp.h"

float ramp(float current, float target, float step)
{
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
