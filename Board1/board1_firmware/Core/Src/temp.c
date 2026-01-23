#include "temp.h"

/* ========== STATIC MODULE VARIABLES ========== */

static ADC_HandleTypeDef * l_adc = (ADC_HandleTypeDef *)0;

/* Recommended sampling time for internal temperature sensor */
#define TEMP_ADC_SMPL_TIME      (ADC_SAMPLETIME_247CYCLES_5)
#define TEMP_ADC_TMO_MS         (10U)

/* ========== STATIC SUPPORT FUNCTIONS ========== */

/**
 * \brief Configures the ADC channel for internal temperature.
 * \param channel Temperature sensor ADC channel.
 * \return HAL_OK on success, HAL_ERROR otherwise.
 */
static HAL_StatusTypeDef TEMP_cfgCh_(uint32_t channel)
{
    HAL_StatusTypeDef st;
    ADC_ChannelConfTypeDef cfg;

    cfg.Channel      = channel;
    cfg.Rank         = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = TEMP_ADC_SMPL_TIME;
    cfg.SingleDiff   = ADC_SINGLE_ENDED;
    cfg.OffsetNumber = ADC_OFFSET_NONE;
    cfg.Offset       = 0U;

    st = HAL_ADC_ConfigChannel(l_adc, &cfg);
    return st;
}

/**
 * \brief Performs a single ADC conversion.
 * \return Raw ADC value or 0 on error.
 */
static uint32_t TEMP_readConv_(void)
{
    uint32_t adcVal;
    HAL_StatusTypeDef stStart;
    HAL_StatusTypeDef stPoll;

    adcVal = 0U;

    stStart = HAL_ADC_Start(l_adc);
    if (stStart == HAL_OK)
    {
        stPoll = HAL_ADC_PollForConversion(l_adc, TEMP_ADC_TMO_MS);
        if (stPoll == HAL_OK)
        {
            adcVal = HAL_ADC_GetValue(l_adc);
        }
        (void)HAL_ADC_Stop(l_adc);
    }

    return adcVal;
}

/* ========== PUBLIC API ========== */

void TEMP_init(ADC_HandleTypeDef * adc)
{

    l_adc = adc;
    HAL_ADCEx_Calibration_Start(l_adc, ADC_SINGLE_ENDED);
}

uint32_t TEMP_readRaw(void)
{
    uint32_t raw;

    raw = 0U;

    if (l_adc != (ADC_HandleTypeDef *)0)
    {
        if (TEMP_cfgCh_(TEMP_ADC_CH) == HAL_OK)
        {
            raw = TEMP_readConv_();
        }
    }

    return raw;
}

float TEMP_getCelsius(void)
{
    uint32_t raw;
    float vSense;
    float tempC;

    raw = TEMP_readRaw();

    /* Convert ADC reading to millivolts */
    vSense = ((float)raw * TEMP_ADC_VREF_MV) / TEMP_ADC_MAX_CNT;

    /* Apply STM32 linear formula */
    tempC = ((vSense - TEMP_V25_MV) / TEMP_SLOPE_MV) + 25.0F;

    return tempC;
}
