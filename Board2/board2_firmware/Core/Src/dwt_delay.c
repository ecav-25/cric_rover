#include "dwt_delay.h"

static uint8_t s_ready = 0U;

dwt_status_t DWT_Delay_Init(void)
{
    dwt_status_t status = DWT_ERR_UNSUPPORTED;

#if defined(DWT) && defined(CoreDebug) && defined(CoreDebug_DEMCR_TRCENA_Msk) && defined(DWT_CTRL_CYCCNTENA_Msk)
    uint32_t before;
    uint32_t after;
    volatile int32_t i;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    before = DWT->CYCCNT;
    for (i = 0; i < 64; i++)
    {
        __NOP();
    }
    after = DWT->CYCCNT;

    if (after != before)
    {
        s_ready = 1U;
        status = DWT_OK;
    }
    else
    {
        s_ready = 0U;
        status = DWT_ERR_UNSUPPORTED;
    }
#endif

    return status;
}

uint8_t DWT_Delay_IsReady(void)
{
    uint8_t ready_flag = 0U;

#if defined(DWT) && defined(DWT_CTRL_CYCCNTENA_Msk)
    if ((s_ready != 0U) && ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) != 0U))
    {
        ready_flag = 1U;
    }
#endif

    return ready_flag;
}

static inline void dwt_delay_cycles(uint32_t cycles)
{
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < cycles)
    {

    }
}

dwt_status_t DWT_Delay_us(uint32_t us)
{
    dwt_status_t status = DWT_ERR_NOT_READY;
    uint64_t cycles;

    if (DWT_Delay_IsReady() != 0U)
    {

        cycles = ((uint64_t)SystemCoreClock * (uint64_t)us) / 1000000ULL;

        if (cycles == 0ULL)
        {
            cycles = 1ULL;
        }

        dwt_delay_cycles((uint32_t)cycles);
        status = DWT_OK;
    }

    return status;
}

dwt_status_t DWT_Delay_ms(uint32_t ms)
{
    dwt_status_t status = DWT_ERR_NOT_READY;
    uint32_t ms_count = ms;

    if (DWT_Delay_IsReady() != 0U)
    {
        while (ms_count > 0U)
        {
            (void)DWT_Delay_us(1000U);
            ms_count--;
        }
        status = DWT_OK;
    }

    return status;
}
