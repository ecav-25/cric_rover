#include "dwt_delay.h"

static uint8_t s_ready = 0;

dwt_status_t DWT_Delay_Init(void)
{
#if defined(DWT) && defined(CoreDebug) && defined(CoreDebug_DEMCR_TRCENA_Msk) && defined(DWT_CTRL_CYCCNTENA_Msk)
    /* abilita trace */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* reset + enable cycle counter */
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* verifica: su alcuni target potrebbe essere bloccato */
    uint32_t before = DWT->CYCCNT;
    for (volatile int i = 0; i < 64; i++) { __NOP(); }
    uint32_t after = DWT->CYCCNT;

    s_ready = (after != before) ? 1U : 0U;
    return s_ready ? DWT_OK : DWT_ERR_UNSUPPORTED;
#else
    (void)0;
    return DWT_ERR_UNSUPPORTED;
#endif
}

uint8_t DWT_Delay_IsReady(void)
{
#if defined(DWT) && defined(DWT_CTRL_CYCCNTENA_Msk)
    return (uint8_t)(s_ready && (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk));
#else
    return 0;
#endif
}

static inline void dwt_delay_cycles(uint32_t cycles)
{
    uint32_t start = DWT->CYCCNT;
    while ((uint32_t)(DWT->CYCCNT - start) < cycles) { }
}

dwt_status_t DWT_Delay_us(uint32_t us)
{
    if (!DWT_Delay_IsReady()) return DWT_ERR_NOT_READY;

    /* evita overflow in moltiplicazione: usa 64-bit */
    uint64_t cycles = ((uint64_t)SystemCoreClock * (uint64_t)us) / 1000000ULL;
    if (cycles == 0) cycles = 1;

    dwt_delay_cycles((uint32_t)cycles);
    return DWT_OK;
}

dwt_status_t DWT_Delay_ms(uint32_t ms)
{
    if (!DWT_Delay_IsReady()) return DWT_ERR_NOT_READY;

    /* spezza in chunk per evitare overflow */
    while (ms--) {
        (void)DWT_Delay_us(1000);
    }
    return DWT_OK;
}
