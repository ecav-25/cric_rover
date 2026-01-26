#include <utils.h>

real32_T ramp(real32_T current, real32_T target, real32_T step)
{
    if (current < target - step){
        return current + step;
    }
    else if (current > target + step){
        return current - step;
    }
    else{
        return target;
    }
}
