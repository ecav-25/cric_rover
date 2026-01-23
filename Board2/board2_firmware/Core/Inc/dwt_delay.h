#ifndef DWT_DELAY_H
#define DWT_DELAY_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DWT_OK = 0,
    DWT_ERR_UNSUPPORTED,
    DWT_ERR_NOT_READY
} dwt_status_t;

/* Abilita DWT->CYCCNT. Chiamare una volta all'avvio. */
dwt_status_t DWT_Delay_Init(void);

/* Delay in microsecondi (busy-wait preciso). Richiede Init. */
dwt_status_t DWT_Delay_us(uint32_t us);

/* Delay in millisecondi (usa DWT, non HAL_Delay). Richiede Init. */
dwt_status_t DWT_Delay_ms(uint32_t ms);

/* True se CYCCNT è abilitato e funzionante. */
uint8_t DWT_Delay_IsReady(void);

#ifdef __cplusplus
}
#endif

#endif
