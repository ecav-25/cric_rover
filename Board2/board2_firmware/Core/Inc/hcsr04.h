#ifndef HCSR04_H
#define HCSR04_H

#define HCSR04_MAX_DISTANCE 400U

#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HCSR04_IDLE = 0,
    HCSR04_WAIT_RISE,
    HCSR04_WAIT_FALL
} hcsr04_state_t;

typedef enum {
    HCSR04_OK = 0,
    HCSR04_ERR_BAD_PARAM,
    HCSR04_ERR_BUSY,
    HCSR04_ERR_TIMEOUT,
    HCSR04_ERR_NOT_READY
} hcsr04_status_t;

/* Delay in microsecondi fornito dall’app (OS-agnostico). */
typedef uint8_t (*hcsr04_delay_us_f)(uint32_t us);

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;              /* TIM_CHANNEL_1..4 */

    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;

    uint32_t timer_hz;             /* freq contatore timer (Hz), es 1_000_000 */
    uint32_t timeout_ms;           /* es 30ms */

    hcsr04_delay_us_f delay_us;    /* obbligatoria: us per TRIG */
} hcsr04_cfg_t;

typedef struct {
    hcsr04_cfg_t cfg;

    volatile hcsr04_state_t state;
    volatile uint32_t t_rise;
    volatile uint32_t t_fall;
    volatile uint32_t start_ms;

    volatile uint16_t last_cm;
    volatile hcsr04_status_t last_status;
    volatile bool ready;
} hcsr04_t;

/* API */
hcsr04_status_t HCSR04_Init(hcsr04_t *s, const hcsr04_cfg_t *cfg);
hcsr04_status_t HCSR04_Start(hcsr04_t *s);

void HCSR04_IC_Callback(TIM_HandleTypeDef *htim); /* chiamare in HAL_TIM_IC_CaptureCallback */

hcsr04_status_t HCSR04_GetDistanceCm(hcsr04_t *s, uint16_t *cm);

#ifdef __cplusplus
}
#endif

#endif
