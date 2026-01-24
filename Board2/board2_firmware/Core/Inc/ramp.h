#ifndef RAMP_H
#define RAMP_H

#include "rtwtypes.h"   // per real32_T (oppure stdint.h se preferisci float)

#define OPENLOOP_RAMP_SLOPE    (600.0f)
#define NORMAL_BRK_COEFF      (0.5f)

/**
 * @brief Applica una rampa lineare verso un target.
 *
 * @param current Valore corrente
 * @param target  Valore obiettivo
 * @param step    Incremento massimo per chiamata
 * @return Nuovo valore rampato
 */
real32_T ramp(real32_T current, real32_T target, real32_T step);

#endif /* RAMP_H */
