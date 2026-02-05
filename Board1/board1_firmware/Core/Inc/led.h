#ifndef INC_LED_H_
#define INC_LED_H_
#include "gpio.h"
#include "Board1_types.h"

#define LED_INIT_STATE_OFF	(0)
#define LED_INIT_STATE_ON	(1)
#define LED_TOGGLE_PERIOD	(0)

#define LED_ERR		(-1)
#define LED_OK		(0)

#define RED_STEPS_ON (4)
#define BLINKING_RED_STEPS (8)
#define WHITE_STEPS_ON (8)
#define BLINKING_WHITE_STEPS (16)

typedef LED_TYPE led_state_t;


typedef enum{
	LED_RED = 0,
	LED_WHITE,
	LED_COUNT
} controlled_led_t;


struct led_s
{
	GPIO_TypeDef* port[LED_COUNT];
	uint16_t pin[LED_COUNT];

	led_state_t state;

	uint8_t step;

	uint8_t pinState[LED_COUNT];
	uint8_t toggle_steps;

};

typedef struct led_s led_t;


typedef GPIO_PinState pin_state_t;


int8_t led_init(led_t* led, GPIO_TypeDef* const GPIOx[LED_COUNT], const uint16_t GPIO_Pin[LED_COUNT],const pin_state_t init_pin_state[LED_COUNT], uint8_t toggle_steps);

/*
 * Set an output status for the led.
 */
int8_t led_write(led_t* led, pin_state_t status, controlled_led_t controlled_led);

int8_t led_on(led_t* led, controlled_led_t controlled_led);

int8_t led_off(led_t* led, controlled_led_t controlled_led);

/**
 * Set toggle period in millisecond
 */
int8_t led_set_toggle_steps(led_t* led, uint8_t toggle_steps);

/**
 * Get the toggling period in milliseconds
 */
int8_t led_get_toggle_period(led_t* led, uint8_t* toggle_steps);

/*
 * Toggle the led according to the toggle period
 */
int8_t led_toggle(led_t* led, controlled_led_t controlled_led);

/*
 * Return the current write status
 */
int8_t led_get_status(const led_t* led, pin_state_t* state, controlled_led_t controlled_led);

/*
 * Return the port number of GPIO PORT used by the button
 */
int8_t led_get_port(const led_t* led, GPIO_TypeDef** port, controlled_led_t controlled_led);

/*
 * Return the port number of GPIO PIN NUMBER used by the button
 */
int8_t led_get_pin_number(const led_t* led, uint16_t* number, controlled_led_t controlled_led);


int8_t led_step( led_t* led, led_state_t state);


#endif
