#include "led.h"


#define RED_STEPS_ON (4U)
#define BLINKING_RED_STEPS (8U)

#define RED_STEPS_SLOW_ON (8U)
#define BLINKING_RED_SLOW_STEPS (16U)

#define WHITE_STEPS_ON (8U)
#define BLINKING_WHITE_STEPS (16U)





static int8_t led_mode_off( led_t* led);
static int8_t led_mode_white( led_t* led);
static int8_t led_mode_red( led_t* led);
static int8_t led_mode_blinking_red(led_t* led);
static int8_t led_mode_blinking_red_slow(led_t* led);
static int8_t led_mode_blinking_white(led_t* led);





int8_t led_init(led_t* led, GPIO_TypeDef* const GPIOx[LED_COUNT], const uint16_t GPIO_Pin[LED_COUNT],const pin_state_t init_pin_state[LED_COUNT])
{

	int8_t res = LED_ERR;

	if(led != NULL)
	{

		led->state = OFF;
		led->step = 0;


		for(uint8_t i = 0; i < (uint8_t)LED_COUNT; i++){
			led->pin[i] = GPIO_Pin[i];
			led->port[i] = GPIOx[i];
			led->pinState[i] = init_pin_state[i];
			HAL_GPIO_WritePin(led->port[i], led->pin[i], led->pinState[i]);
		}

		res = LED_OK;
	}

	return res;
}


int8_t led_write(led_t* led, pin_state_t status, controlled_led_t controlled_led)
{
	int8_t res = LED_ERR;

	if(led != NULL)
	{
		led->pinState[controlled_led] = status;
		HAL_GPIO_WritePin(led->port[controlled_led], led->pin[controlled_led], led->pinState[controlled_led]);
		res = LED_OK;
	}

	return res;
}



int8_t led_on(led_t* led, controlled_led_t controlled_led)
{
	return led_write(led, 1, controlled_led);
}

int8_t led_off(led_t* led, controlled_led_t controlled_led)
{
	return led_write(led, 0, controlled_led);
}

int8_t led_step(led_t* led, led_state_t state)
{
    int8_t res = LED_ERR;

    if (led != NULL)
    {
        if (state != led->state)
        {
            led->step = 0U;
            led->state = state;
            led->first_entry = 1U;
        }

        switch (state)
        {
            case OFF:
                if (led_mode_off(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            case WHITE:
                if (led_mode_white(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            case RED:
                if (led_mode_red(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            case BLINKING_RED:
                if (led_mode_blinking_red(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            case BLINKING_WHITE:
                if (led_mode_blinking_white(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            case BLINKING_RED_SLOW:
                if (led_mode_blinking_red_slow(led) == LED_OK)
                {
                    res = LED_OK;
                }
                break;

            default:
                (void)led_off(led, LED_WHITE);
                (void)led_off(led, LED_RED);
                res = LED_ERR;
                break;
        }
    }

    return res;
}



static int8_t led_mode_off(led_t* led)
{
    int8_t res = LED_OK;

    if (led->step == 0U)
    {
        if ((led_off(led, LED_RED) == LED_OK) &&
            (led_off(led, LED_WHITE) == LED_OK))
        {
            led->step++;
        }
        else
        {
            res = LED_ERR;
        }
    }

    return res;
}


static int8_t led_mode_white(led_t* led)
{
    int8_t res = LED_ERR;

    if (led->step != 0U)
    {
        res = LED_OK;
    }
    else
    {
        if ((led_off(led, LED_RED) == LED_OK) &&
            (led_on(led, LED_WHITE) == LED_OK))
        {
            led->step++;
            res = LED_OK;
        }
    }

    return res;
}

static int8_t led_mode_red(led_t* led)
{
    int8_t res = LED_ERR;

    if (led->step != 0U)
    {
        res = LED_OK;
    }
    else
    {
        if ((led_on(led, LED_RED) == LED_OK) &&
            (led_off(led, LED_WHITE) == LED_OK))
        {
            led->step++;
            res = LED_OK;
        }
    }

    return res;
}

static int8_t led_mode_blinking_red(led_t* led)
{
    if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
    	(void)led_off(led, LED_WHITE);
    }
    if (led->step < RED_STEPS_ON) {
        if (led->pinState[LED_RED] != GPIO_PIN_SET){
        	(void)led_on(led, LED_RED);
        }
    }
    else {
        if (led->pinState[LED_RED] != GPIO_PIN_RESET){
        	(void)led_off(led, LED_RED);
        }
    }

    led->step = (led->step + 1U) % BLINKING_RED_STEPS;

    return LED_OK;
}

static int8_t led_mode_blinking_red_slow(led_t* led)
{
    if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
    	(void)led_off(led, LED_WHITE);
    }

    if (led->step < RED_STEPS_SLOW_ON) {
        if (led->pinState[LED_RED] != GPIO_PIN_SET){
        	(void)led_on(led, LED_RED);
        }

    }
    else {
        if (led->pinState[LED_RED] != GPIO_PIN_RESET){
        	(void)led_off(led, LED_RED);
        }

    }

    led->step = (led->step + 1U) % BLINKING_RED_SLOW_STEPS;

    return LED_OK;
}



static int8_t led_mode_blinking_white(led_t* led)
{
    if (led->pinState[LED_RED] != GPIO_PIN_RESET){
    	(void)led_off(led, LED_RED);
    }


    if (led->step < WHITE_STEPS_ON) {
        if (led->pinState[LED_WHITE] != GPIO_PIN_SET){
        	(void)led_on(led, LED_WHITE);
        }
    } else {
        if (led->pinState[LED_WHITE] != GPIO_PIN_RESET){
        	(void)led_off(led, LED_WHITE);
        }

    }

    led->step = (led->step + 1U) % BLINKING_WHITE_STEPS;

    return LED_OK;
}




