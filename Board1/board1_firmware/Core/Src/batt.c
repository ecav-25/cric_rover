#include "batt.h"

static Batt_Status_t batt_cfg_channel_(batt_t *b)
{
    Batt_Status_t status = BATT_ERR;

    if ((b != NULL) && (b->hadc != NULL))
    {
        if (HAL_ADC_ConfigChannel(b->hadc, &b->channel_cfg) == HAL_OK)
        {
            status = BATT_OK;
        }
        else
        {
            status = BATT_ERR_COMM;
        }
    }

    return status;
}

static Batt_Status_t batt_read_once_(batt_t *b, uint32_t *raw)
{
    Batt_Status_t status = BATT_ERR;

    if ((b != NULL) && (raw != NULL) && (b->hadc != NULL))
    {
        if (HAL_ADC_Start(b->hadc) != HAL_OK)
        {
            status = BATT_ERR_COMM;
        }
        else
        {
            if (HAL_ADC_PollForConversion(b->hadc, b->timeout_ms) == HAL_OK)
            {
                *raw = HAL_ADC_GetValue(b->hadc);
                status = BATT_OK;
            }
            else
            {
                status = BATT_ERR_COMM;
            }

            HAL_ADC_Stop(b->hadc);
        }
    }

    return status;
}


static float batt_raw_to_voltage_(uint32_t raw){
    float v_adc;
    float ratio;

    v_adc = ((float)raw * BATT_ADC_VREF_MV) / (BATT_ADC_MAX_CNT * 1000.0f);

    ratio = (BATT_R1_OHM + BATT_R2_OHM) / BATT_R2_OHM;

    return v_adc * ratio;
}

static Batt_Status_t batt_read_raw_internal_(batt_t *b, uint8_t samples, uint32_t *raw)
{
    Batt_Status_t status = BATT_ERR;
    Batt_Status_t step_status = BATT_OK;
    uint32_t acc = 0U;
    uint32_t sample = 0U;
    uint8_t i;

    if ((b != NULL) && (raw != NULL) && (samples != 0U))
    {
        if (batt_cfg_channel_(b) == BATT_OK)
        {
            for (i = 0U; (i < samples) && (step_status == BATT_OK); i++)
            {
                step_status = batt_read_once_(b, &sample);

                if (step_status == BATT_OK)
                {
                    acc += sample;
                }
            }

            if (step_status == BATT_OK)
            {
                *raw = acc / (uint32_t)samples;
                b->raw_last = *raw;
                status = BATT_OK;
            }
        }
    }

    return status;
}



Batt_Status_t batt_init(batt_t *b, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef *channel_cfg, uint32_t timeout_ms)
{
    Batt_Status_t status = BATT_ERR;

    if ((b != NULL) && (hadc != NULL) && (channel_cfg != NULL))
    {
        b->hadc        = hadc;
        b->channel_cfg = *channel_cfg;
        b->timeout_ms  = timeout_ms;

        b->raw_last  = 0U;
        b->volt_last = 0.0f;

        if (HAL_ADCEx_Calibration_Start(b->hadc, b->channel_cfg.SingleDiff) != HAL_OK)
        {
            status = BATT_ERR_COMM;
        }
        else
        {
            status = BATT_OK;
        }
    }

    return status;
}

Batt_Status_t batt_read_raw_once(batt_t *b, uint32_t *raw){
    return batt_read_raw_internal_(b, 1, raw);
}

Batt_Status_t batt_get_voltage_once(batt_t *b, float *volt)
{
    Batt_Status_t status = BATT_ERR;
    uint32_t raw;

    if ((b != NULL) && (volt != NULL))
    {
        if (batt_read_raw_once(b, &raw) == BATT_OK)
        {
            b->volt_last = batt_raw_to_voltage_(raw);
            *volt = b->volt_last;
            status = BATT_OK;
        }
    }

    return status;
}


