#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

#include "motor.h"
#include "encoder.h"
#include "led_stripes.h"
#include "led.h"


/* ===================== MOTORS ===================== */

typedef enum {
    MOTOR_FA = 0,
    MOTOR_FB,
    MOTOR_BA,
    MOTOR_BB,
    MOTOR_COUNT
} Motor_Id_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    Motor_Calibration_t calib;
} Motor_HW_Config_t;

extern const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT];

/* ===================== ENCODERS ===================== */

typedef enum {
    ENCODER_FA = 0,
    ENCODER_FB,
    ENCODER_BA,
    ENCODER_BB,
    ENCODER_COUNT
} Encoder_Id_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    Encoder_Calibration_t calib;
} Encoder_HW_Config_t;


extern const Encoder_HW_Config_t ENCODER_HW_CONFIG[ENCODER_COUNT];

/* ===================== ADC ===================== */

typedef enum {
	ADC_TEMP_SENSOR = 0,
	ADC_BATTERY_VOLTAGE,
	ADC_COUNT
} ADC_Id_t;

typedef struct {
	ADC_HandleTypeDef* hadc;
	ADC_ChannelConfTypeDef channel_cfg;
} ADC_HW_Config_t;

extern const ADC_HW_Config_t ADC_HW_CONFIG[ADC_COUNT];


/* ===================== LEDS ===================== */

typedef enum {
	LED_A = 0,
	LED_B,
	LED_COUNT_HW
} HW_Led_Id_t;


typedef struct {

	GPIO_TypeDef* port[LED_COUNT];
	uint16_t pin[LED_COUNT];
	pin_state_t init_pin_state[LED_COUNT];
	uint8_t toggle_steps;
} HW_Led_Config_t;

extern const HW_Led_Config_t HW_LED_CONFIG[LED_COUNT_HW];


typedef enum{
	LED_STRIPES_MAIN = 0,
	LED_STRIPES_COUNT
}LED_Stripes_Id_t;


extern const led_stripes_config_t led_stripes_cfg[LED_STRIPES_COUNT];

#endif /* INC_HW_CONFIG_H_ */
