#ifndef INC_BATT_H_
#define INC_BATT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* ================== RETURN CODES ================== */

typedef enum {
    BATT_OK  = 0,
    BATT_ERR = -1
} Batt_Status_t;

/* ================== ADC / BATTERY CONSTANTS ================== */

/* ADC reference voltage in millivolts */
#define BATT_ADC_VREF_MV        (3300.0f)

/* ADC resolution (12-bit) */
#define BATT_ADC_MAX_CNT        (4095.0f)

/* Divider resistors (Ohm) */
#define BATT_R1_OHM             (100000.0f)   /* high side */
#define BATT_R2_OHM             (27000.0f)    /* low side  */

/* LiPo 3S limits (Volt) */
#define BATT_LIPO_VMIN          (10.0f)
#define BATT_LIPO_VMAX          (12.6f)

/* Timeout di default per operazioni ADC */
#define BATT_DEFAULT_TIMEOUT_MS (10U)

/* ================== OBJECT ================== */

typedef struct
{
    ADC_HandleTypeDef *hadc;

    ADC_ChannelConfTypeDef channel_cfg;

    uint32_t timeout_ms;

    /* ultimi valori acquisiti */
    uint32_t raw_last;
    float    volt_last;

} batt_t;

/* ================== API ================== */

/**
 * Inizializza l'oggetto batteria.
 */
Batt_Status_t batt_init(batt_t *b, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef *channel_cfg, uint32_t timeout_ms);

/**
 * Lettura ADC grezza singola.
 */
Batt_Status_t batt_read_raw_once(batt_t *b, uint32_t *raw);

/**
 * Lettura ADC grezza mediata su N campioni.
 */
Batt_Status_t batt_read_raw_avg(batt_t *b, uint8_t samples, uint32_t *raw);

/**
 * Lettura tensione batteria (Volt) singola.
 */
Batt_Status_t batt_get_voltage_once(batt_t *b, float *volt);

/**
 * Lettura tensione batteria (Volt) mediata su N campioni.
 */
Batt_Status_t batt_get_voltage_avg(batt_t *b, uint8_t samples, float *volt);


/**
 * Ultimo valore ADC acquisito.
 */
Batt_Status_t batt_get_last_raw(const batt_t *b, uint32_t *raw);

/**
 * Ultima tensione calcolata.
 */
Batt_Status_t batt_get_last_voltage(const batt_t *b, float *volt);


#ifdef __cplusplus
}
#endif

#endif /* INC_BATT_H_ */
