#include "batt.h"

/* ========== STATIC MODULE VARIABLES ========== */

/* Local pointer to the ADC instance */
static ADC_HandleTypeDef * l_adc = (ADC_HandleTypeDef *)0;

/* Long sampling time suitable for high-impedance dividers */
#define BATT_ADC_SMPL_TIME      (ADC_SAMPLETIME_247CYCLES_5)

/* ADC polling timeout in milliseconds */
#define BATT_ADC_TMO_MS         (10U)

/* ========== STATIC SUPPORT FUNCTIONS ========== */

/**
 * \brief Configures the ADC channel for battery measurement.
 * \param channel ADC channel to configure.
 * \return HAL_OK on success, HAL_ERROR otherwise.
 */
static HAL_StatusTypeDef BATT_cfgCh_(uint32_t channel)
{
    HAL_StatusTypeDef st;
    ADC_ChannelConfTypeDef cfg;

    cfg.Channel      = channel;
    cfg.Rank         = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = BATT_ADC_SMPL_TIME;
    cfg.SingleDiff   = ADC_SINGLE_ENDED;
    cfg.OffsetNumber = ADC_OFFSET_NONE;
    cfg.Offset       = 0U;

    st = HAL_ADC_ConfigChannel(l_adc, &cfg);
    return st;
}

/**
 * \brief Performs a single ADC conversion.
 * \return Raw ADC value (0..4095) or 0 on failure.
 */
static uint32_t BATT_readConv_(void)
{
    uint32_t adcVal;
    HAL_StatusTypeDef stStart;
    HAL_StatusTypeDef stPoll;

    adcVal = 0U;

    stStart = HAL_ADC_Start(l_adc);
    if (stStart == HAL_OK)
    {
        stPoll = HAL_ADC_PollForConversion(l_adc, BATT_ADC_TMO_MS);
        if (stPoll == HAL_OK)
        {
            adcVal = HAL_ADC_GetValue(l_adc);
        }
        (void)HAL_ADC_Stop(l_adc);
    }

    return adcVal;
}

/* ========== PUBLIC API ========== */

void BATT_init(ADC_HandleTypeDef * adc)
{
    l_adc = adc;
    HAL_ADCEx_Calibration_Start(l_adc, ADC_SINGLE_ENDED);
}

uint32_t BATT_readRaw(void)
{
    uint32_t raw;

    raw = 0U;

    if (l_adc != (ADC_HandleTypeDef *)0)
    {
        if (BATT_cfgCh_(BATT_ADC_CH) == HAL_OK)
        {
            raw = BATT_readConv_();
        }
    }

    return raw;
}

float BATT_getVolt(void)
{
    uint32_t raw;
    float vAdc;
    float ratio;
    float vBat;

    raw = BATT_readRaw();

    /* ADC conversion to volts */
    vAdc = ((float)raw * BATT_ADC_VREF_MV) /
           (BATT_ADC_MAX_CNT * 1000.0F);

    /* Divider ratio */
    ratio = (BATT_R1_OHM + BATT_R2_OHM) / BATT_R2_OHM;

    vBat = vAdc * ratio;
    return vBat;
}

float BATT_getSoc(void)
{
    float v;
    float soc;

    v = BATT_getVolt();

    if (v <= BATT_LIPO_VMIN)
    {
        soc = 0.0F;
    }
    else if (v >= BATT_LIPO_VMAX)
    {
        soc = 100.0F;
    }
    else
    {
        soc = ((v - BATT_LIPO_VMIN) * 100.0F) /
              (BATT_LIPO_VMAX - BATT_LIPO_VMIN);
    }

    return soc;
}
