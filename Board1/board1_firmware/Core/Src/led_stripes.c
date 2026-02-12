#include "led_stripes.h"
#include <stdio.h>
#include <string.h>

#define NUM_BPP (3U)
#define NUM_BYTES (NUM_BPP * NUM_PIXELS)
#define WR_BUF_LEN (NUM_BPP * 8U * 2U)

#define LED_REAR_SIGN_START 0U
#define LED_REAR_SIGN_END 7U

#define LED_REAR_LED_START 8U
#define LED_REAR_LED_END 20U

#define ARROW_OFF_STEPS  5U
#define ARROW_ON_STEPS   5U

#define EMERGENCY_PERIOD 16U

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

static uint16_t lfsr = 0xACE1U;

static led_bus_t g_led_bus;
static led_part_t rear_led;
static led_part_t rear_sign;

static void rear_led_off(void);
static void rear_led_anim_stop(void);
static void rear_led_anim_backlights(void);
static void rear_led_anim_arrow_left(void);
static void rear_led_anim_arrow_right(void);
static void rear_led_anim_backward(void);
static void rear_led_special_lights(void);
static void rear_led_emercency_lights(void);
static void rear_led_degraded_lights(void);

static void rear_sign_off(void);
static void rear_sign_white(void);
static void rear_sign_orange(void);
static void rear_sign_green(void);
static void rear_sign_red(void);
static void rear_sign_yellow(void);

static inline uint16_t lfsr_next(void)
{
    uint16_t lsb = lfsr & 1U;
    lfsr >>= 1U;
    if (lsb != 0U)
    {
        lfsr ^= 0xB400U;
    }
    return lfsr;
}

static inline void random_rainbow(uint8_t *r, uint8_t *g, uint8_t *b)
{
    switch (lfsr_next() % 6U)
    {
        case 0U:
            *r = 255U; *g = 0U;   *b = 0U;
            break;
        case 1U:
            *r = 255U; *g = 80U;  *b = 0U;
            break;
        case 2U:
            *r = 255U; *g = 255U; *b = 0U;
            break;
        case 3U:
            *r = 0U;   *g = 255U; *b = 0U;
            break;
        case 4U:
            *r = 0U;   *g = 0U;   *b = 255U;
            break;
        default:
            *r = 160U; *g = 0U;   *b = 255U;
            break;
    }
}

static inline uint8_t scale8(uint8_t x, uint8_t scale)
{
    return (uint8_t)(((uint16_t)x * (uint16_t)scale) >> 8U);
}

led_status_t led_stripe_init(const led_stripes_config_t *cfg)
{
    led_status_t status = LED_STRIPE_ERR;

    if (cfg != NULL)
    {
        g_led_bus.cfg = *cfg;
        g_led_bus.wr_buf_p = 0U;
        g_led_bus.dirty = 0U;

        (void)memset(g_led_bus.rgb_arr, 0, sizeof(g_led_bus.rgb_arr));
        (void)memset(g_led_bus.wr_buf, 0, sizeof(g_led_bus.wr_buf));

        rear_led.bus = &g_led_bus;
        rear_led.start = LED_REAR_LED_START;
        rear_led.end = LED_REAR_LED_END;
        rear_led.last_animation = IDLE;
        rear_led.step = 0U;

        rear_sign.bus = &g_led_bus;
        rear_sign.start = LED_REAR_SIGN_START;
        rear_sign.end = LED_REAR_SIGN_END;
        rear_sign.last_animation = IDLE;
        rear_sign.step = 0U;

        status = LED_STRIPE_OK;
    }

    return status;
}

void rear_led_step(REAR_LED_TYPE animation)
{
    if (g_led_bus.wr_buf_p == 0U)
    {
        if (animation != rear_led.last_animation)
        {
            rear_led.step = 0U;
            rear_led.last_animation = animation;
        }

        switch (animation)
        {
            case IDLE:
                rear_led_off();
                break;
            case BACKLIGHTS:
                rear_led_anim_backlights();
                break;
            case BRAKING_LIGHTS:
                rear_led_anim_stop();
                break;
            case BACKWARD_LIGHTS:
                rear_led_anim_backward();
                break;
            case ARROW_LEFT:
                rear_led_anim_arrow_left();
                break;
            case ARROW_RIGHT:
                rear_led_anim_arrow_right();
                break;
            case SPECIAL_LIGHTS:
                rear_led_special_lights();
                break;
            case EMERGENCY_LIGHTS:
                rear_led_emercency_lights();
                break;
            case DEGRADED_LIGHTS:
                rear_led_degraded_lights();
                break;
            default:
                rear_led_off();
                break;
        }
    }
}

void rear_sign_step(REAR_SIGN_TYPE animation)
{
    if (g_led_bus.wr_buf_p == 0U)
    {
        if (animation != rear_sign.last_animation)
        {
            rear_sign.step = 0U;
            rear_sign.last_animation = animation;
        }

        switch (animation)
        {
            case SIGN_OFF:
                rear_sign_off();
                break;
            case SIGN_WHITE:
                rear_sign_white();
                break;
            case SIGN_GREEN:
                rear_sign_green();
                break;
            case SIGN_ORANGE:
                rear_sign_orange();
                break;
            case SIGN_RED:
                rear_sign_red();
                break;
            case SIGN_YELLOW:
                rear_sign_yellow();
                break;
            default:
                rear_sign_off();
                break;
        }
    }
}

static void rear_led_off(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 0U, 0U, 0U);
        g_led_bus.dirty = 1U;
        rear_led.step = 1U;
    }
}

static void rear_sign_off(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 0U, 0U, 0U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

static void rear_led_special_lights(void)
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    for (uint16_t i = rear_led.start; i <= rear_led.end; i++)
    {
        random_rainbow(&r, &g, &b);
        led_set_RGB((uint8_t)i, r, g, b);
    }

    g_led_bus.dirty = 1U;
}

static void rear_led_emercency_lights(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 255U, 0U, 0U);
        g_led_bus.dirty = 1U;
    }
    else if (rear_led.step == (EMERGENCY_PERIOD / 2U))
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 0U, 0U, 0U);
        g_led_bus.dirty = 1U;
    }
    else
    {
    }

    rear_led.step = (uint16_t)((rear_led.step + 1U) % EMERGENCY_PERIOD);
}

static void rear_led_degraded_lights(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 255U, 255U, 0U);
        g_led_bus.dirty = 1U;
        rear_led.step = 1U;
    }
}

static void rear_led_anim_stop(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 255U, 0U, 0U);
        g_led_bus.dirty = 1U;
        rear_led.step = 1U;
    }
}

static void rear_led_anim_backlights(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 0U, 0U, 0U);

        uint16_t len = (uint16_t)((rear_led.end - rear_led.start) + 1U);
        uint16_t k = (len < 4U) ? len : 4U;

        for (uint16_t i = 0U; i < k; i++)
        {
            led_set_RGB((uint8_t)(rear_led.start + i), 100U, 0U, 0U);
            led_set_RGB((uint8_t)(rear_led.end - i), 100U, 0U, 0U);
        }

        g_led_bus.dirty = 1U;
        rear_led.step = 1U;
    }
}

static void rear_led_anim_arrow_right(void)
{
    const uint8_t r = 255U;
    const uint8_t g = 80U;
    const uint8_t b = 0U;

    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)((full_end - full_start) + 1U);

    const uint16_t mid        = (uint16_t)(full_start + ((full_len - 1U) / 2U));
    const uint16_t left_len   = (uint16_t)((mid - full_start) + 1U);
    const uint16_t fill_steps = left_len;

    const uint16_t cycle      = (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps + (uint16_t)ARROW_ON_STEPS);

    if (rear_led.step < (uint16_t)ARROW_OFF_STEPS)
    {
        if (rear_led.step == 0U)
        {
            led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
        }
    }
    else if (rear_led.step < (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps))
    {
        led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
        uint16_t s = (uint16_t)((rear_led.step - (uint16_t)ARROW_OFF_STEPS) + 1U);

        for (uint16_t i = 0U; (i < s) && (i < left_len); i++)
        {
            led_set_RGB((uint8_t)(mid - i), r, g, b);
        }
    }
    else
    {
        if (rear_led.step == (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps))
        {
            led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
            for (uint16_t i = 0U; i < left_len; i++)
            {
                led_set_RGB((uint8_t)(mid - i), r, g, b);
            }
        }
    }

    g_led_bus.dirty = 1U;
    rear_led.step = (uint16_t)((rear_led.step + 1U) % cycle);
}

static void rear_led_anim_arrow_left(void)
{
    const uint8_t r = 255U;
    const uint8_t g = 80U;
    const uint8_t b = 0U;

    const uint16_t full_start = rear_led.start;
    const uint16_t full_end   = rear_led.end;
    const uint16_t full_len   = (uint16_t)((full_end - full_start) + 1U);

    const uint16_t mid        = (uint16_t)(full_start + ((full_len - 1U) / 2U));
    const uint16_t right_len  = (uint16_t)((full_end - mid) + 1U);
    const uint16_t fill_steps = right_len;

    const uint16_t cycle      = (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps + (uint16_t)ARROW_ON_STEPS);

    if (rear_led.step < (uint16_t)ARROW_OFF_STEPS)
    {
        if (rear_led.step == 0U)
        {
            led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
        }
    }
    else if (rear_led.step < (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps))
    {
        led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
        uint16_t s = (uint16_t)((rear_led.step - (uint16_t)ARROW_OFF_STEPS) + 1U);

        for (uint16_t i = 0U; (i < s) && (i < right_len); i++)
        {
            led_set_RGB((uint8_t)(mid + i), r, g, b);
        }
    }
    else
    {
        if (rear_led.step == (uint16_t)((uint16_t)ARROW_OFF_STEPS + fill_steps))
        {
            led_set_RGB_range(full_start, full_end, 0U, 0U, 0U);
            for (uint16_t i = 0U; i < right_len; i++)
            {
                led_set_RGB((uint8_t)(mid + i), r, g, b);
            }
        }
    }

    g_led_bus.dirty = 1U;
    rear_led.step = (uint16_t)((rear_led.step + 1U) % cycle);
}

static void rear_led_anim_backward(void)
{
    if (rear_led.step == 0U)
    {
        led_set_RGB_range(rear_led.start, rear_led.end, 255U, 180U, 180U);
        g_led_bus.dirty = 1U;
        rear_led.step = 1U;
    }
}

static void rear_sign_white(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 255U, 255U, 255U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

static void rear_sign_red(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 255U, 0U, 0U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

static void rear_sign_green(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 0U, 255U, 0U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

static void rear_sign_orange(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 255U, 40U, 0U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

static void rear_sign_yellow(void)
{
    if (rear_sign.step == 0U)
    {
        led_set_RGB_range(rear_sign.start, rear_sign.end, 255U, 255U, 0U);
        g_led_bus.dirty = 1U;
        rear_sign.step = 1U;
    }
}

void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    g_led_bus.rgb_arr[NUM_BPP * index] = scale8(g, g_led_bus.cfg.scale_g);
    g_led_bus.rgb_arr[(NUM_BPP * index) + 1U] = r;
    g_led_bus.rgb_arr[(NUM_BPP * index) + 2U] = scale8(b, g_led_bus.cfg.scale_b);
}

void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint_fast8_t i = 0U; i < NUM_PIXELS; ++i)
    {
        led_set_RGB((uint8_t)i, r, g, b);
    }
}

void led_set_RGB_range(uint16_t start, uint16_t end, uint8_t r, uint8_t g, uint8_t b)
{
    if ((start <= end) && (end < (uint16_t)NUM_PIXELS))
    {
        for (uint16_t i = start; i <= end; i++)
        {
            led_set_RGB((uint8_t)i, r, g, b);
        }
    }
}

void led_render(void)
{
    if (g_led_bus.dirty != 0U)
    {
        if ((g_led_bus.wr_buf_p == 0U) && (g_led_bus.cfg.hdma->State == HAL_DMA_STATE_READY))
        {
            for (uint_fast8_t i = 0U; i < 8U; ++i)
            {
                uint8_t bit_0 = (((g_led_bus.rgb_arr[0] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_1 = (((g_led_bus.rgb_arr[1] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_2 = (((g_led_bus.rgb_arr[2] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_3 = (((g_led_bus.rgb_arr[3] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_4 = (((g_led_bus.rgb_arr[4] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_5 = (((g_led_bus.rgb_arr[5] << i) & 0x80U) != 0U) ? 1U : 0U;

                g_led_bus.wr_buf[i]       = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_0);
                g_led_bus.wr_buf[i + 8U]  = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_1);
                g_led_bus.wr_buf[i + 16U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_2);
                g_led_bus.wr_buf[i + 24U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_3);
                g_led_bus.wr_buf[i + 32U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_4);
                g_led_bus.wr_buf[i + 40U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_5);
            }

            g_led_bus.dirty = 0U;
            (void)HAL_TIM_PWM_Start_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel, (uint32_t *)g_led_bus.wr_buf, WR_BUF_LEN);
            g_led_bus.wr_buf_p = 2U;
        }
    }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if (htim != NULL)
    {
        if (g_led_bus.wr_buf_p < (uint_fast8_t)NUM_PIXELS)
        {
            for (uint_fast8_t i = 0U; i < 8U; ++i)
            {
                uint8_t bit_0 = (((g_led_bus.rgb_arr[3U * g_led_bus.wr_buf_p] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_1 = (((g_led_bus.rgb_arr[(3U * g_led_bus.wr_buf_p) + 1U] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_2 = (((g_led_bus.rgb_arr[(3U * g_led_bus.wr_buf_p) + 2U] << i) & 0x80U) != 0U) ? 1U : 0U;

                g_led_bus.wr_buf[i]       = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_0);
                g_led_bus.wr_buf[i + 8U]  = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_1);
                g_led_bus.wr_buf[i + 16U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_2);
            }
            g_led_bus.wr_buf_p++;
        }
        else if (g_led_bus.wr_buf_p < (uint_fast8_t)(NUM_PIXELS + 2U))
        {
            for (uint8_t i = 0U; i < (WR_BUF_LEN / 2U); ++i)
            {
                g_led_bus.wr_buf[i] = 0U;
            }
            g_led_bus.wr_buf_p++;
        }
        else
        {
        }
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim != NULL)
    {
        if (g_led_bus.wr_buf_p < (uint_fast8_t)NUM_PIXELS)
        {
            for (uint_fast8_t i = 0U; i < 8U; ++i)
            {
                uint8_t bit_0 = (((g_led_bus.rgb_arr[3U * g_led_bus.wr_buf_p] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_1 = (((g_led_bus.rgb_arr[(3U * g_led_bus.wr_buf_p) + 1U] << i) & 0x80U) != 0U) ? 1U : 0U;
                uint8_t bit_2 = (((g_led_bus.rgb_arr[(3U * g_led_bus.wr_buf_p) + 2U] << i) & 0x80U) != 0U) ? 1U : 0U;

                g_led_bus.wr_buf[i + 24U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_0);
                g_led_bus.wr_buf[i + 32U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_1);
                g_led_bus.wr_buf[i + 40U] = (uint8_t)(g_led_bus.cfg.pwm_lo << bit_2);
            }
            g_led_bus.wr_buf_p++;
        }
        else if (g_led_bus.wr_buf_p < (uint_fast8_t)(NUM_PIXELS + 2U))
        {
            for (uint8_t i = (WR_BUF_LEN / 2U); i < WR_BUF_LEN; ++i)
            {
                g_led_bus.wr_buf[i] = 0U;
            }
            g_led_bus.wr_buf_p++;
        }
        else
        {
            g_led_bus.wr_buf_p = 0U;
            (void)HAL_TIM_PWM_Stop_DMA(g_led_bus.cfg.htim, g_led_bus.cfg.tim_channel);
        }
    }
}
