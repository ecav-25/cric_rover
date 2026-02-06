#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* ================== RETURN CODES ================== */



typedef enum {
	TEMP_OK = 0,
	TEMP_ERR,
	TEMP_ERR_COMM
} Temp_Status_t;

/* ================== SENSOR CONSTANTS ================== */
/* Valori tipici da datasheet STM32 */

#define TEMP_ADC_VREF_MV        (3300.0f)
#define TEMP_ADC_MAX_CNT        (4095.0f)

#define TEMP_V25_MV             (760.0f)
#define TEMP_SLOPE_MV_PER_C     (2.5f)

/* Sampling time consigliato per sensore interno */
#define TEMP_DEFAULT_SMPL_TIME  (ADC_SAMPLETIME_247CYCLES_5)
#define TEMP_DEFAULT_TIMEOUT_MS (10U)

/* ================== OBJECT ================== */

typedef struct
{
    ADC_HandleTypeDef *hadc;

    ADC_ChannelConfTypeDef channel_cfg;

    uint32_t timeout_ms;

    /* ultimi valori acquisiti */
    uint32_t raw_last;
    float    temp_c_last;

} temp_t;

/* ================== API ================== */

/**
 * Inizializza l'oggetto sensore di temperatura.
 */
Temp_Status_t temp_init(temp_t *t, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef* channel_cfg, uint32_t timeout_ms);
/**
 * Lettura ADC grezza singola.
 */
Temp_Status_t temp_read_raw_once(temp_t *t, uint32_t *raw);

/**
 * Lettura ADC grezza mediata su N campioni.
 */
Temp_Status_t temp_read_raw_avg(temp_t *t, uint8_t samples, uint32_t *raw);

/**
 * Lettura temperatura (°C) singola.
 */
Temp_Status_t temp_get_celsius_once(temp_t *t, float *temp_c);

/**
 * Lettura temperatura (°C) mediata su N campioni.
 */
Temp_Status_t temp_get_celsius_avg(temp_t *t, uint8_t samples, float *temp_c);

/**
 * Ultimo valore ADC acquisito.
 */
Temp_Status_t temp_get_last_raw(const temp_t *t, uint32_t *raw);

/**
 * Ultima temperatura calcolata.
 */
Temp_Status_t temp_get_last_celsius(const temp_t *t, float *temp_c);

#ifdef __cplusplus
}
#endif

#endif /* INC_TEMP_H_ */
