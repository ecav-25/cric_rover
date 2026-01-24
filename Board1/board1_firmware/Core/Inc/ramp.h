#ifndef RAMP_H_
#define RAMP_H_

/* -------------------- Ramping parameters -------------------- */

#define RAMP_SLOPE_DEFAULT_RPM_S   (600.0f)
#define RAMP_SLOPE_SPORT_RPM_S     (1000.0f)
#define RAMP_SLOPE_ECO_RPM_S       (300.0f)

#define NORMAL_BRK_COEFF           (0.5f)

/* -------------------- API -------------------- */

/**
 * @brief Applica un ramping lineare tra valore corrente e target.
 *
 * @param current Valore attuale
 * @param target  Valore desiderato
 * @param step    Incremento massimo per chiamata
 * @return Nuovo valore rampato
 */
float ramp(float current, float target, float step);

#endif /* RAMP_H_ */
