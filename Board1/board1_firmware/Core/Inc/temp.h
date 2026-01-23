#ifndef TEMP_H
#define TEMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32g4xx_hal.h"

/* ================== ADC/TEMPERATURE CONFIGURATION ================== */

/* ADC reference voltage in millivolts */
#define TEMP_ADC_VREF_MV        (3300.0F)

/* ADC resolution (12-bit) */
#define TEMP_ADC_MAX_CNT        (4095.0F)

/* Internal temperature sensor ADC channel */
#define TEMP_ADC_CH             (ADC_CHANNEL_TEMPSENSOR_ADC1)

/* Sensor parameters from STM32 datasheet */
#define TEMP_V25_MV             (760.0F)    /* mV at 25°C       */
#define TEMP_SLOPE_MV           (2.5F)      /* mV/°C typical    */

/* ================== TEMPERATURE API ================== */

/**
 * \brief Initialises the temperature module.
 * \param adc Pointer to an initialised ADC handle.
 */
void TEMP_init(ADC_HandleTypeDef * adc);

/**
 * \brief Reads a raw ADC value from the internal temperature channel.
 * \return Raw ADC result (0..4095) or 0 on failure.
 */
uint32_t TEMP_readRaw(void);

/**
 * \brief Computes temperature in degrees Celsius.
 * \return Temperature (°C).
 */
float TEMP_getCelsius(void);

#ifdef __cplusplus
}
#endif

#endif /* TEMP_H */
