#include "led.h"


#define RED_STEPS_ON (4)
#define BLINKING_RED_STEPS (8)

#define RED_STEPS_SLOW_ON (8)
#define BLINKING_RED_SLOW_STEPS (16)

#define WHITE_STEPS_ON (8)
#define BLINKING_WHITE_STEPS (16)





static int8_t led_mode_off( led_t* led);
static int8_t led_mode_white( led_t* led);
static int8_t led_mode_red( led_t* led);
static int8_t led_mode_blinking_red(led_t* led);
static int8_t led_mode_blinking_red_slow(led_t* led);
static int8_t led_mode_blinking_white(led_t* led);





int8_t led_init(led_t* led, GPIO_TypeDef* const GPIOx[LED_COUNT], const uint16_t GPIO_Pin[LED_COUNT],const pin_state_t init_pin_state[LED_COUNT], uint8_t toggle_steps)
{

	int8_t res = LED_ERR;

	if(led)
	{

		led->state = OFF;
		led->step = 0;
		led->toggle_steps = toggle_steps;


		for(uint8_t i = 0; i < LED_COUNT; i++){
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

	if(led)
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

int8_t led_set_toggle_steps(led_t* led, uint8_t toggle_steps)
{
	int8_t res = LED_ERR;

	if(led)
	{
		led->toggle_steps = toggle_steps;
		res = LED_OK;
	}

	return res;
}

int8_t led_get_toggle_period(led_t* led, uint8_t* toggle_steps)
{
	int8_t res = LED_ERR;

	if(led)
	{
		*toggle_steps = led->toggle_steps;
		res = LED_OK;
	}

	return res;
}

int8_t led_toggle(led_t* led, controlled_led_t controlled_led)
{
	int8_t res = LED_ERR;

	if(led)
	{


		if(led->step >= led->toggle_steps)
		{

			led->pinState[controlled_led] = !led->pinState[controlled_led];
			HAL_GPIO_WritePin(led->port[controlled_led], led->pin[controlled_led], led->pinState[controlled_led]);
		}

		res = LED_OK;
	}

	return res;
}

inline int8_t led_get_status(const led_t* led, pin_state_t* state, controlled_led_t controlled_led)
{
	int8_t res = LED_ERR;

	if(led)
	{
		*state = led->pinState[controlled_led];
		res = LED_OK;
	}

	return res;
}

inline int8_t led_get_port(const led_t* led, GPIO_TypeDef** port, controlled_led_t controlled_led)
{
	int8_t res = LED_ERR;

	if(led)
	{
		*port = led->port[controlled_led];
		res = LED_OK;
	}

	return res;
}

inline int8_t led_get_pin_number(const led_t* led, uint16_t* number, controlled_led_t controlled_led)
{

	int8_t res = LED_ERR;

	if(led)
	{
		*number = led->pin[controlled_led];
		res = LED_OK;
	}

	return res;
}


int8_t led_step(led_t* led, led_state_t state){

	if(!led) return LED_ERR;


	int8_t res = LED_ERR;

	if (state != led->state){

			led->step = 0;
			led->state = state;
			led->first_entry = 1;
	}


	switch(state){

		case OFF:
			if(led_mode_off(led) == LED_OK){
				res = LED_OK;
			}
			break;

		case WHITE:
			if(led_mode_white(led) == LED_OK){
				res = LED_OK;
			}
			break;

		case RED:
			if(led_mode_red(led) == LED_OK){
				res = LED_OK;
			}
			break;

		case BLINKING_RED:
			if(led_mode_blinking_red(led) == LED_OK){
				res = LED_OK;
			}
			break;

		case BLINKING_WHITE:
			if(led_mode_blinking_white(led) == LED_OK){
				res = LED_OK;
			}
			break;

		case BLINKING_RED_SLOW:
			if(led_mode_blinking_red_slow(led) == LED_OK){
				res = LED_OK;
			}
			break;
		default:
			led_off(led, LED_WHITE);
			led_off(led, LED_RED);
			res = LED_ERR;
			break;

	}



	return res;
}



static int8_t led_mode_off( led_t* led){

	if(led->step != 0) return LED_OK;


	int8_t res = LED_ERR;

	if(led_off(led, LED_RED) == LED_OK && led_off(led, LED_WHITE) == LED_OK){
		led->step++;
		res = LED_OK;
	}

	return res;
}


static int8_t led_mode_white( led_t* led){
	if(led->step != 0) return LED_OK;

	int8_t res = LED_ERR;

	if(led_off(led, LED_RED) == LED_OK && led_on(led, LED_WHITE) == LED_OK){
		led->step++;
		res = LED_OK;
	}

	return res;
}


static int8_t led_mode_red( led_t* led){

	if(led->step != 0) return LED_OK;

	int8_t res = LED_ERR;

	if(led_on(led, LED_RED) == LED_OK && led_off(led, LED_WHITE) == LED_OK){
		led->step++;
		res = LED_OK;
	}

	return res;
}


static int8_t led_mode_blinking_red(led_t* led)
{
    /* garantisco esclusività */
    if (led->pinState[LED_WHITE] != GPIO_PIN_RESET)
        led_off(led, LED_WHITE);

    /* fase ON */
    if (led->step < RED_STEPS_ON) {
        if (led->pinState[LED_RED] != GPIO_PIN_SET)
            led_on(led, LED_RED);
    }
    /* fase OFF */
    else {
        if (led->pinState[LED_RED] != GPIO_PIN_RESET)
            led_off(led, LED_RED);
    }

    /* avanzamento ciclico */
    led->step = (led->step + 1) % BLINKING_RED_STEPS;

    return LED_OK;
}

static int8_t led_mode_blinking_red_slow(led_t* led)
{
    /* garantisco esclusività */
    if (led->pinState[LED_WHITE] != GPIO_PIN_RESET)
        led_off(led, LED_WHITE);

    /* fase ON */
    if (led->step < RED_STEPS_SLOW_ON) {
        if (led->pinState[LED_RED] != GPIO_PIN_SET)
            led_on(led, LED_RED);
    }
    /* fase OFF */
    else {
        if (led->pinState[LED_RED] != GPIO_PIN_RESET)
            led_off(led, LED_RED);
    }

    /* avanzamento ciclico */
    led->step = (led->step + 1) % BLINKING_RED_SLOW_STEPS;

    return LED_OK;
}



static int8_t led_mode_blinking_white(led_t* led)
{
    if (led->pinState[LED_RED] != GPIO_PIN_RESET)
        led_off(led, LED_RED);

    if (led->step < WHITE_STEPS_ON) {
        if (led->pinState[LED_WHITE] != GPIO_PIN_SET)
            led_on(led, LED_WHITE);
    } else {
        if (led->pinState[LED_WHITE] != GPIO_PIN_RESET)
            led_off(led, LED_WHITE);
    }

    led->step = (led->step + 1) % BLINKING_WHITE_STEPS;

    return LED_OK;
}




