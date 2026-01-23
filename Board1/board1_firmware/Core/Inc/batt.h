#ifndef BATT_H
#define BATT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32g4xx_hal.h"

/* ================== ADC/BATTERY CONFIGURATION ================== */

/* ADC reference voltage in millivolts */
#define BATT_ADC_VREF_MV        (3300.0F)

/* ADC resolution (12-bit: 0..4095) */
#define BATT_ADC_MAX_CNT        (4095.0F)

/* ADC channel connected to the battery divider */
#define BATT_ADC_CH             (ADC_CHANNEL_9) //questo deve essere configurabile

/* Divider resistors (Ohm) */
#define BATT_R1_OHM             (100000.0F)    /* 100 kOhm */
#define BATT_R2_OHM             (27000.0F)     /* 27 kOhm  */

/* LiPo 3S State-of-Charge limits (Volt) */
#define BATT_LIPO_VMIN          (10.0F)
#define BATT_LIPO_VMAX          (12.6F)

/* ================== BATTERY API ================== */

/**
 * \brief Initialises the battery measurement module.
 * \param adc Pointer to an initialised ADC handle (e.g., &hadc1).
 */
void BATT_init(ADC_HandleTypeDef * adc);

/**
 * \brief Reads a raw ADC value from the battery channel.
 * \return Raw ADC value (0..4095), or 0 on error.
 */
uint32_t BATT_readRaw(void);

/**
 * \brief Computes the actual battery voltage.
 * \return Battery voltage in volts.
 */
float BATT_getVolt(void);

/**
 * \brief Estimates the LiPo 3S State-of-Charge percentage.
 * \return SOC percentage (0..100), saturated at limits.
 */
float BATT_getSoc(void);

#ifdef __cplusplus
}
#endif

#endif /* BATT_H */
