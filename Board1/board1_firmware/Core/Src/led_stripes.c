#include "led_stripes.h"
#include <stdio.h>
#include <string.h>


#define NUM_BPP (3)
#define NUM_BYTES (NUM_BPP * NUM_PIXELS)
#define WR_BUF_LEN (NUM_BPP * 8 * 2)

#define LED_REAR_SIGN_START 0
#define LED_REAR_SIGN_END 7

#define LED_REAR_LED_START 8
#define LED_REAR_LED_END 20

#define ARROW_OFF_STEPS  5
#define ARROW_ON_STEPS   5

typedef struct {
  led_stripes_config_t cfg;

  uint8_t rgb_arr[NUM_BYTES];
  uint8_t wr_buf[WR_BUF_LEN];

  volatile uint_fast8_t wr_buf_p;
  volatile uint8_t dirty;
} led_bus_t;

typedef struct {
  led_bus_t *bus;

  uint16_t start;
  uint16_t end;

  uint8_t last_animation;
  uint16_t step;

} led_part_t;

static uint16_t lfsr = 0xACE1u;  /* seed ≠ 0 */


/* unico contesto HW condiviso */
static led_bus_t g_led_bus;

/* due “dispositivi logici” */
static led_part_t rear_led;
static led_part_t rear_sign;


static led_status_t rear_led_off();
static led_status_t rear_led_anim_stop();
static led_status_t rear_led_anim_backlights();
static led_status_t rear_led_anim_arrow_left();
static led_status_t rear_led_anim_arrow_right();
static led_status_t rear_led_anim_backward();
static led_status_t rear_led_special_lights();
static led_status_t rear_sign_off();
static led_status_t rear_sign_white();
static led_status_t rear_sign_orange();
static led_status_t rear_sign_green();
static led_status_t rear_sign_red();


static inline uint16_t lfsr_next(void)
{
    uint16_t lsb = lfsr & 1u;
    lfsr >>= 1;
    if (lsb)
        lfsr ^= 0xB400u;  /* polinomio x^16 + x^14 + x^13 + x^11 + 1 */
    return lfsr;
}


static inline void random_rainbow(uint8_t *r, uint8_t *g, uint8_t *b)
{
    switch (lfsr_next() % 6) {
        case 0: *r = 255; *g = 0;   *b = 0;   break; // rosso
        case 1: *r = 255; *g = 80;  *b = 0;   break; // arancio
        case 2: *r = 255; *g = 255; *b = 0;   break; // giallo
        case 3: *r = 0;   *g = 255; *b = 0;   break; // verde
        case 4: *r = 0;   *g = 0;   *b = 255; break; // blu
        default:*r = 160; *g = 0;   *b = 255; break; // viola
    }
}




static inline uint8_t scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;
}


led_status_t led_stripe_init(const led_stripes_config_t *cfg){

  if (!cfg) return LED_STRIPE_ERR;

  g_led_bus.cfg = *cfg;
  g_led_bus.wr_buf_p = 0;
  g_led_bus.dirty = 0;

  memset(g_led_bus.rgb_arr, 0, sizeof(g_led_bus.rgb_arr));
  memset(g_led_bus.wr_buf,  0, sizeof(g_led_bus.wr_buf));

  rear_led.bus  = &g_led_bus;
  rear_led.start = LED_REAR_LED_START;
  rear_led.end   = LED_REAR_LED_END;
  rear_led.last_animation = IDLE;
  rear_led.step = 0;

  rear_sign.bus  = &g_led_bus;
  rear_sign.start = LED_REAR_SIGN_START;
  rear_sign.end   = LED_REAR_SIGN_END;
  rear_sign.last_animation = IDLE;
  rear_sign.step = 0;

  return LED_STRIPE_OK;
}

led_status_t rear_led_step(REAR_LED_TYPE animation){

    if (g_led_bus.wr_buf_p != 0) {
        return LED_STRIPE_OK;   // DMA attivo → non tocco lo stato
    }

	led_status_t res = LED_STRIPE_ERR;


	if (animation != rear_led.last_animation){
		rear_led.step = 0;
		rear_led.last_animation = animation;
	}

	switch(animation){
		case IDLE:
			if(rear_led_off() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case BACKLIGHTS:
			if(rear_led_anim_backlights() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case  BRAKING_LIGHT:
			if(rear_led_anim_stop() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case  BACKWARD_LIGHTS:
			if(rear_led_anim_backward() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case  ARROW_LEFT:
			if(rear_led_anim_arrow_left() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case  ARROW_RIGHT:
			if(rear_led_anim_arrow_right() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;

		case SPECIAL_LIGHTS:
			if(rear_led_special_lights() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
			
		default:
			rear_led_off();
			res = LED_STRIPE_ERR;
			break;

	}
	return res;
}



led_status_t rear_sign_step(REAR_SIGN_TYPE animation){


    if (g_led_bus.wr_buf_p != 0) {
        return LED_STRIPE_OK;   // DMA attivo → non tocco lo stato
    }

	led_status_t res = LED_STRIPE_ERR;


	if (animation != rear_sign.last_animation){
		rear_sign.step = 0;
		rear_sign.last_animation = animation;
	}

	switch(animation){
		case SIGN_OFF:
			if(rear_sign_off() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case SIGN_WHITE:
			if(rear_sign_white() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case SIGN_GREEN:
			if(rear_sign_green() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case SIGN_ORANGE:
			if(rear_sign_orange() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		case SIGN_RED:
			if(rear_sign_red() == LED_STRIPE_OK){
				res = LED_STRIPE_OK;
			}
			break;
		default:
			rear_sign_off();
			res = LED_STRIPE_ERR;
			break;

	}
	return res;
}

static led_status_t rear_led_off(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}



static led_status_t rear_sign_off(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_led_special_lights(void)
{

    for (uint16_t i = rear_led.start; i <= rear_led.end; i++) {

        uint8_t r, g, b;
        random_rainbow(&r, &g, &b);

        led_set_RGB((uint8_t)i, r, g, b);

    }

    g_led_bus.dirty = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_led_anim_stop(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 255, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_led_anim_backlights(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    // spegni solo il range rear_led
    if (led_set_RGB_range(rear_led.start, rear_led.end, 0, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    uint16_t len = rear_led.end - rear_led.start + 1;
    uint16_t k = (len < 4) ? len : 4;

    for (uint16_t i = 0; i < k; i++) {
        led_set_RGB((uint8_t)(rear_led.start + i), 100, 0, 0);
        led_set_RGB((uint8_t)(rear_led.end - i),   100, 0, 0);
    }

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}



static led_status_t rear_led_anim_arrow_right(void)
{
    const uint8_t r = 255, g = 80, b = 0;

    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)(full_end - full_start + 1);

    /* metà sinistra = [full_start .. mid], metà destra = [mid+1 .. full_end] */
    const uint16_t mid        = (uint16_t)(full_start + (full_len - 1) / 2);

    const uint16_t left_len   = (uint16_t)(mid - full_start + 1);     /* numero LED metà sinistra */
    const uint16_t fill_steps = left_len;
    const uint16_t cycle      = (uint16_t)(ARROW_OFF_STEPS + fill_steps + ARROW_ON_STEPS);

    /* 1) OFF */
    if (rear_led.step < ARROW_OFF_STEPS) {
        if (rear_led.step == 0) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);          /* spegni tutto il segmento */
            g_led_bus.dirty = 1;
        }
    }
    /* 2) FILL: centro -> sinistra (parte da mid) */
    else if (rear_led.step < (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {

        uint16_t s = (uint16_t)(rear_led.step - ARROW_OFF_STEPS + 1);  /* 1..left_len */

        led_set_RGB_range(full_start, full_end, 0, 0, 0);
        for (uint16_t i = 0; i < s && i < left_len; i++) {
            led_set_RGB((uint8_t)(mid - i), r, g, b);
        }
        g_led_bus.dirty = 1;
    }
    /* 3) ON (FULL): metà sinistra accesa */
    else {
        if (rear_led.step == (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {

            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            for (uint16_t i = 0; i < left_len; i++) {
                led_set_RGB((uint8_t)(mid - i), r, g, b);
            }
            g_led_bus.dirty = 1;
        }
    }

    rear_led.step = (uint16_t)((rear_led.step + 1) % cycle);
    return LED_STRIPE_OK;
}

static led_status_t rear_led_anim_arrow_left(void)
{
    const uint8_t r = 255, g = 80, b = 0;

    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)(full_end - full_start + 1);

    /* metà sinistra = [full_start .. mid], metà destra = [mid+1 .. full_end] */
    const uint16_t mid        = (uint16_t)(full_start + (full_len - 1) / 2);

    const uint16_t right_len  = (uint16_t)(full_end - mid + 1);
    const uint16_t fill_steps = right_len;
    const uint16_t cycle      = (uint16_t)(ARROW_OFF_STEPS + fill_steps + ARROW_ON_STEPS);

    /* 1) OFF */
    if (rear_led.step < ARROW_OFF_STEPS) {
        if (rear_led.step == 0) {
            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            g_led_bus.dirty = 1;
        }
    }
    /* 2) FILL: centro -> destra (parte da right_start = mid+1) */
    else if (rear_led.step < (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {

        uint16_t s = (uint16_t)(rear_led.step - ARROW_OFF_STEPS + 1);  /* 1..right_len */

        led_set_RGB_range(full_start, full_end, 0, 0, 0);
        for (uint16_t i = 0; i < s && i < right_len; i++) {
            led_set_RGB((uint8_t)(mid + i), r, g, b);
        }
        g_led_bus.dirty = 1;
    }
    /* 3) ON (FULL): metà destra accesa */
    else {
        if (rear_led.step == (uint16_t)(ARROW_OFF_STEPS + fill_steps)) {

            led_set_RGB_range(full_start, full_end, 0, 0, 0);
            for (uint16_t i = 0; i < right_len; i++) {
                led_set_RGB((uint8_t)(mid + i), r, g, b);
            }
            g_led_bus.dirty = 1;
        }
    }

    rear_led.step = (uint16_t)((rear_led.step + 1) % cycle);
    return LED_STRIPE_OK;
}


static led_status_t rear_led_anim_backward(void)
{
    if (rear_led.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_led.start, rear_led.end, 255, 180, 180) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_led.step = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_sign_white(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 255, 255) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}

static led_status_t rear_sign_red(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 0, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_sign_green(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 0, 255, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}


static led_status_t rear_sign_orange(void)
{
    if (rear_sign.step != 0) return LED_STRIPE_OK;

    if (led_set_RGB_range(rear_sign.start, rear_sign.end, 255, 40, 0) != LED_STRIPE_OK)
        return LED_STRIPE_ERR;

    g_led_bus.dirty = 1;
    rear_sign.step = 1;
    return LED_STRIPE_OK;
}


led_status_t led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {



  g_led_bus.rgb_arr[NUM_BPP * index] = scale8(g, g_led_bus.cfg.scale_g); // g;
  g_led_bus.rgb_arr[NUM_BPP * index + 1] = r;
  g_led_bus.rgb_arr[NUM_BPP * index + 2] = scale8(b, g_led_bus.cfg.scale_b); // b;

  return LED_STRIPE_OK;
}




// Set all colors to RGB
led_status_t led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {



  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) {
	  if(led_set_RGB(i, r, g, b) != LED_STRIPE_OK)
		  return LED_STRIPE_ERR;
  }

  return LED_STRIPE_OK;
}


led_status_t led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b) {
    if (start > end) return LED_STRIPE_ERR;
    if (end >= NUM_PIXELS) return LED_STRIPE_ERR;

    for (uint16_t i = start; i <= end; i++) {
        if (led_set_RGB((uint8_t)i, r, g, b) != LED_STRIPE_OK) {
            return LED_STRIPE_ERR;
        }
    }
    return LED_STRIPE_OK;
}

// Shuttle the data to the LEDs!
led_status_t led_render() {

  if(!g_led_bus.dirty)
	  return LED_STRIPE_OK;

  if(g_led_bus.wr_buf_p != 0 || g_led_bus.cfg.hdma->State != HAL_DMA_STATE_READY) {
    // Ongoing transfer, cancel!

	  /*
    for(uint8_t i = 0; i < WR_BUF_LEN; ++i) g_led_bus.wr_buf[i] = 0;
    g_led_bus.wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel);
    */
    return LED_STRIPE_OK;

  }

  for(uint_fast8_t i = 0; i < 8; ++i) {
	  g_led_bus.wr_buf[i     ] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[0] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i +  8] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[1] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 16] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[2] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 24] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 32] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[4] << i) & 0x80) > 0);
	  g_led_bus.wr_buf[i + 40] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[5] << i) & 0x80) > 0);
  }

  g_led_bus.dirty = 0;
  HAL_TIM_PWM_Start_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel, (uint32_t *)g_led_bus.wr_buf, WR_BUF_LEN);
  g_led_bus.wr_buf_p = 2; // Since we're ready for the next buffer

  return LED_STRIPE_OK;
}






void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {


	// DMA buffer set from LED(wr_buf_p) to LED(wr_buf_p + 1)
  if(g_led_bus.wr_buf_p < NUM_PIXELS) {
    // We're in. Fill the even buffer

    for(uint_fast8_t i = 0; i < 8; ++i) {
    	g_led_bus.wr_buf[i     ] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p    ] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i +  8] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 1] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 16] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 2] << i) & 0x80) > 0);
    }
    g_led_bus.wr_buf_p++;
  } else if (g_led_bus.wr_buf_p < NUM_PIXELS + 2) {
    // Last two transfers are resets. SK6812: 64 * 1.25 us = 80 us == good enough reset
  	//                               WS2812B: 48 * 1.25 us = 60 us == good enough reset
    // First half reset zero fill
    for(uint8_t i = 0; i < WR_BUF_LEN / 2; ++i) g_led_bus.wr_buf[i] = 0;
    g_led_bus.wr_buf_p++;
  }

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {

  // DMA buffer set from LED(wr_buf_p) to LED(wr_buf_p + 1)
  if(g_led_bus.wr_buf_p < NUM_PIXELS) {
    // We're in. Fill the odd buffer

    for(uint_fast8_t i = 0; i < 8; ++i) {
    	g_led_bus.wr_buf[i + 24] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p    ] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 32] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 1] << i) & 0x80) > 0);
    	g_led_bus.wr_buf[i + 40] = g_led_bus.cfg.pwm_lo << (((g_led_bus.rgb_arr[3 * g_led_bus.wr_buf_p + 2] << i) & 0x80) > 0);
    }
    g_led_bus.wr_buf_p++;
  } else if (g_led_bus.wr_buf_p < NUM_PIXELS + 2) {
    // Second half reset zero fill
    for(uint8_t i = WR_BUF_LEN / 2; i < WR_BUF_LEN; ++i) g_led_bus.wr_buf[i] = 0;
    ++g_led_bus.wr_buf_p;
  } else {
    // We're done. Lean back and until next time!
	g_led_bus.wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel);

  }
}
