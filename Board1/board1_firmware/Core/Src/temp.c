#include "temp.h"

/* ================== STATIC SUPPORT FUNCTIONS ================== */

static Temp_Status_t temp_cfg_channel_(temp_t *t)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (t->hadc != NULL))
    {
        if (HAL_ADC_ConfigChannel(t->hadc, &t->channel_cfg) == HAL_OK)
        {
            status = TEMP_OK;
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}

static Temp_Status_t temp_read_once_(temp_t *t, uint32_t *raw)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (raw != NULL) && (t->hadc != NULL))
    {
        if (HAL_ADC_Start(t->hadc) == HAL_OK)
        {
            if (HAL_ADC_PollForConversion(t->hadc, t->timeout_ms) == HAL_OK)
            {
                *raw = HAL_ADC_GetValue(t->hadc);
                status = TEMP_OK;
            }
            else
            {
                status = TEMP_ERR_COMM;
            }

            (void)HAL_ADC_Stop(t->hadc);
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}


static float temp_raw_to_celsius_(uint32_t raw)
{
    float v_sense =((float)raw * TEMP_ADC_VREF_MV) / TEMP_ADC_MAX_CNT;

    return ((v_sense - TEMP_V25_MV) / TEMP_SLOPE_MV_PER_C) + 25.0f;
}

static Temp_Status_t temp_read_raw_internal_(temp_t *t, uint8_t samples, uint32_t *raw)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (raw != NULL) && (samples != 0U))
    {
        status = temp_cfg_channel_(t);

        if (status == TEMP_OK)
        {
            uint32_t acc = 0U;
            uint32_t sample = 0U;

            for (uint8_t i = 0U; (i < samples) && (status == TEMP_OK); i++)
            {
                status = temp_read_once_(t, &sample);

                if (status == TEMP_OK)
                {
                    acc += sample;
                }
            }

            if (status == TEMP_OK)
            {
                *raw = acc / (uint32_t)samples;
                t->raw_last = *raw;
            }
        }
    }

    return status;
}


/* ================== PUBLIC API ================== */

Temp_Status_t temp_init(temp_t *t, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef* channel_cfg, uint32_t timeout_ms)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (hadc != NULL) && (channel_cfg != NULL))
    {
        t->hadc = hadc;
        t->channel_cfg = *channel_cfg;
        t->timeout_ms = timeout_ms;

        t->raw_last = 0U;
        t->temp_c_last = 0.0f;

        if (HAL_ADCEx_Calibration_Start(t->hadc, t->channel_cfg.SingleDiff) == HAL_OK)
        {
            status = TEMP_OK;
        }
        else
        {
            status = TEMP_ERR_COMM;
        }
    }

    return status;
}

Temp_Status_t temp_read_raw_once(temp_t *t, uint32_t *raw)
{
    return temp_read_raw_internal_(t, 1, raw);
}

Temp_Status_t temp_get_celsius_once(temp_t *t, float *temp_c)
{
    Temp_Status_t status = TEMP_ERR;

    if ((t != NULL) && (temp_c != NULL))
    {
        uint32_t raw;

        status = temp_read_raw_once(t, &raw);

        if (status == TEMP_OK)
        {
            t->temp_c_last = temp_raw_to_celsius_(raw);
            *temp_c = t->temp_c_last;
        }
    }

    return status;
}



