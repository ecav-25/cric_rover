#include "temp.h"

/* ================== STATIC SUPPORT FUNCTIONS ================== */

static Temp_Status_t temp_cfg_channel_(temp_t *t){
    if (t == NULL || t->hadc == NULL)
        return TEMP_ERR;

    return (HAL_ADC_ConfigChannel(t->hadc, &t->channel_cfg) == HAL_OK) ? TEMP_OK : TEMP_ERR_COMM;
}


static Temp_Status_t temp_read_once_(temp_t *t, uint32_t *raw)
{
    if (t == NULL || raw == NULL || t->hadc == NULL)
        return TEMP_ERR;

    if (HAL_ADC_Start(t->hadc) != HAL_OK)
        return TEMP_ERR_COMM;

    if (HAL_ADC_PollForConversion(t->hadc, t->timeout_ms) != HAL_OK) {
        HAL_ADC_Stop(t->hadc);
        return TEMP_ERR_COMM;
    }

    *raw = HAL_ADC_GetValue(t->hadc);
    HAL_ADC_Stop(t->hadc);

    return TEMP_OK;
}


static float temp_raw_to_celsius_(uint32_t raw)
{
    float v_sense =((float)raw * TEMP_ADC_VREF_MV) / TEMP_ADC_MAX_CNT;

    return ((v_sense - TEMP_V25_MV) / TEMP_SLOPE_MV_PER_C) + 25.0f;
}

static Temp_Status_t temp_read_raw_internal_(temp_t *t, uint8_t samples, uint32_t *raw)
{
    if (t == NULL || raw == NULL || samples == 0)
        return TEMP_ERR;

    Temp_Status_t st = temp_cfg_channel_(t);
    if (st != TEMP_OK)
        return st;

    uint32_t acc = 0U;
    uint32_t sample;

    for (uint8_t i = 0; i < samples; i++) {
        st = temp_read_once_(t, &sample);
        if (st != TEMP_OK)
            return st;
        acc += sample;
    }

    *raw = acc / samples;
    t->raw_last = *raw;

    return TEMP_OK;
}


/* ================== PUBLIC API ================== */

Temp_Status_t temp_init(temp_t *t, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef* channel_cfg, uint32_t timeout_ms)
{
	if (t == NULL || hadc == NULL || channel_cfg == NULL) return TEMP_ERR;

    t->hadc          = hadc;
    t->channel_cfg   = *channel_cfg;
    t->timeout_ms    = timeout_ms;

    t->raw_last      = 0U;
    t->temp_c_last   = 0.0f;

    /* calibrazione ADC (una tantum) */
    if (HAL_ADCEx_Calibration_Start(t->hadc, t->channel_cfg.SingleDiff) != HAL_OK)
        return TEMP_ERR_COMM;

    return TEMP_OK;
}

Temp_Status_t temp_read_raw_once(temp_t *t, uint32_t *raw)
{
    return temp_read_raw_internal_(t, 1, raw);
}

Temp_Status_t temp_read_raw_avg(temp_t *t, uint8_t samples, uint32_t *raw)
{
    return temp_read_raw_internal_(t, samples, raw);
}

Temp_Status_t temp_get_celsius_once(temp_t *t, float *temp_c)
{
    if (t == NULL || temp_c == NULL) return TEMP_ERR;

    uint32_t raw;

    Temp_Status_t st = temp_read_raw_once(t, &raw);
	if (st != TEMP_OK)
		return st;

    t->temp_c_last = temp_raw_to_celsius_(raw);
    *temp_c = t->temp_c_last;

    return TEMP_OK;
}

Temp_Status_t temp_get_celsius_avg(temp_t *t, uint8_t samples, float *temp_c)
{
    if (t == NULL || temp_c == NULL) return TEMP_ERR;

    uint32_t raw;

    Temp_Status_t st = temp_read_raw_avg(t, samples, &raw);
	if (st != TEMP_OK)
		return st;

    t->temp_c_last = temp_raw_to_celsius_(raw);
    *temp_c = t->temp_c_last;

    return TEMP_OK;
}

Temp_Status_t temp_get_last_raw(const temp_t *t, uint32_t *raw)
{
    if (t == NULL || raw == NULL) return TEMP_ERR;

    *raw = t->raw_last;
    return TEMP_OK;
}

Temp_Status_t temp_get_last_celsius(const temp_t *t, float *temp_c)
{
    if (t == NULL || temp_c == NULL) return TEMP_ERR;

    *temp_c = t->temp_c_last;
    return TEMP_OK;
}
