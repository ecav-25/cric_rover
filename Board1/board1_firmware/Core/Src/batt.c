#include "batt.h"

/* ================== STATIC SUPPORT FUNCTIONS ================== */

static Batt_Status_t batt_cfg_channel_(batt_t *b)
{
    if (b == NULL || b->hadc == NULL)
        return BATT_ERR;

    return (HAL_ADC_ConfigChannel(b->hadc, &b->channel_cfg) == HAL_OK) ? BATT_OK : BATT_ERR;
}

static Batt_Status_t batt_read_once_(batt_t *b, uint32_t *raw)
{
    if (b == NULL || raw == NULL)
        return BATT_ERR;

    if (HAL_ADC_Start(b->hadc) != HAL_OK)
        return BATT_ERR;

    if (HAL_ADC_PollForConversion(b->hadc, b->timeout_ms) != HAL_OK) {
        HAL_ADC_Stop(b->hadc);
        return BATT_ERR;
    }

    *raw = HAL_ADC_GetValue(b->hadc);
    HAL_ADC_Stop(b->hadc);

    return BATT_OK;
}

static float batt_raw_to_voltage_(uint32_t raw)
{
    float v_adc;
    float ratio;

    /* ADC -> volts */
    v_adc = ((float)raw * BATT_ADC_VREF_MV) / (BATT_ADC_MAX_CNT * 1000.0f);

    /* divider ratio */
    ratio = (BATT_R1_OHM + BATT_R2_OHM) / BATT_R2_OHM;

    return v_adc * ratio;
}

static Batt_Status_t batt_read_raw_internal_(batt_t *b,uint8_t samples, uint32_t *raw)
{
    if (b == NULL || raw == NULL || samples == 0)
        return BATT_ERR;

    if (batt_cfg_channel_(b) != BATT_OK)
        return BATT_ERR;

    uint32_t acc = 0;
    uint32_t sample;

    for (uint8_t i = 0; i < samples; i++) {
        if (batt_read_once_(b, &sample) != BATT_OK)
            return BATT_ERR;
        acc += sample;
    }

    *raw = acc / samples;
    b->raw_last = *raw;

    return BATT_OK;
}

/* ================== PUBLIC API ================== */

Batt_Status_t batt_init(batt_t *b, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef *channel_cfg, uint32_t timeout_ms)
{
    if (b == NULL || hadc == NULL || channel_cfg == NULL)
        return BATT_ERR;

    b->hadc        = hadc;
    b->channel_cfg = *channel_cfg;
    b->timeout_ms  = timeout_ms;

    b->raw_last  = 0U;
    b->volt_last = 0.0f;

    /* calibrazione ADC (una tantum) */
    if (HAL_ADCEx_Calibration_Start(b->hadc, b->channel_cfg.SingleDiff) != HAL_OK)
        return BATT_ERR;

    return BATT_OK;
}

Batt_Status_t batt_read_raw_once(batt_t *b, uint32_t *raw)
{
    return batt_read_raw_internal_(b, 1, raw);
}

Batt_Status_t batt_read_raw_avg(batt_t *b, uint8_t samples, uint32_t *raw)
{
    return batt_read_raw_internal_(b, samples, raw);
}

Batt_Status_t batt_get_voltage_once(batt_t *b, float *volt)
{
    if (b == NULL || volt == NULL)
        return BATT_ERR;

    uint32_t raw;

    if (batt_read_raw_once(b, &raw) != BATT_OK)
        return BATT_ERR;

    b->volt_last = batt_raw_to_voltage_(raw);

    *volt = b->volt_last;
    return BATT_OK;
}

Batt_Status_t batt_get_voltage_avg(batt_t *b, uint8_t samples, float *volt)
{
    if (b == NULL || volt == NULL)
        return BATT_ERR;

    uint32_t raw;

    if (batt_read_raw_avg(b, samples, &raw) != BATT_OK)
        return BATT_ERR;

    b->volt_last = batt_raw_to_voltage_(raw);

    *volt = b->volt_last;
    return BATT_OK;
}

Batt_Status_t batt_get_last_raw(const batt_t *b, uint32_t *raw)
{
    if (b == NULL || raw == NULL)
        return BATT_ERR;

    *raw = b->raw_last;
    return BATT_OK;
}

Batt_Status_t batt_get_last_voltage(const batt_t *b, float *volt)
{
    if (b == NULL || volt == NULL)
        return BATT_ERR;

    *volt = b->volt_last;
    return BATT_OK;
}

