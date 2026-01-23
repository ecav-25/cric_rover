#ifndef INC_LED_STRIPES_H_
#define INC_LED_STRIPES_H_


#define NUM_PIXELS (21)
#define ARROW_OFF_STEPS  5
#define ARROW_ON_STEPS   5

#include "stm32g474xx.h"
#include "tim.h"
#include "dma.h"
#include "Board1_types.h"

typedef enum {
  LED_STRIPE_OK = 0,
  LED_STRIPE_ERR,
  LED_BUSY,
  LED_INVALID_PARAM
} led_status_t;

typedef REAR_LED_TYPE led_anim_t;

typedef struct {

  TIM_HandleTypeDef *htim;
  DMA_HandleTypeDef *hdma;
  uint32_t tim_channel;

  uint16_t pwm_hi;
  uint16_t pwm_lo;

  /* scaling (0xFF = no scaling) */
  uint8_t scale_g;
  uint8_t scale_r;
  uint8_t scale_b;

  /* numero di mezze-trasmissioni di reset (come nel tuo codice: 2) */
  uint8_t reset_halves;

} led_config_t;


led_status_t led_stripe_init(led_config_t *cfg);
led_status_t rear_led_step(uint8_t animation);
led_status_t rear_sign_step(uint8_t animation);
led_status_t led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
led_status_t led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
led_status_t led_render();
/**
 * @brief Set RGB color for all pixels in [start, end] (inclusive).
 *
 * Utility function to color a contiguous range of LEDs.
 *
 * @param start   First LED index (inclusive)
 * @param end     Last LED index (inclusive)
 * @param r       Red   (0..255)
 * @param g       Green (0..255)
 * @param b       Blue  (0..255)
 * @return LED_STRIPE_OK on success, LED_STRIPE_ERR on invalid range / errors
 */
led_status_t led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b);

#endif /* INC_LED_STRIPES_H_ */
