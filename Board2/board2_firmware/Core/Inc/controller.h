#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_
#include "i2c.h"
#include "stdio.h"

#define CONTROLLERZERO	(255)

typedef struct __attribute__((packed)){
	uint16_t ax;
	uint16_t ay;
	uint8_t a_btn;

	uint16_t bx;
	uint16_t by;
	uint8_t b_btn;

	uint8_t btn1;
	uint8_t btn2;
	uint8_t btn3;
	uint8_t btn4;
} controller_data_t;

typedef struct __attribute__((packed)) {
	uint8_t alive;
	uint8_t controller_percentage;
	controller_data_t controller_data;
}controller_information_t;

typedef struct controller_s{
	I2C_HandleTypeDef* i2c;
	controller_information_t controller_information;
}Controller_t;

typedef enum: uint8_t{
    BW_NORMAL,
    BW_SPECIAL
} BackwardMode;

typedef enum : uint8_t{
    OM_NORMAL,
    OM_SINGLE_BOARD,
    OM_DEGRADED
} OperatingMode;

typedef enum : uint8_t{
    LIGHT_OFF,
    LIGHT_ON,
    LIGHT_AUTO
} LightOperatingMode;


typedef struct __attribute__((packed)) {
    int16_t rpm_fl;
    int16_t rpm_fr;
    int16_t rpm_rl;
    int16_t rpm_rr;
    int16_t target_fl;
	int16_t target_fr;
	int16_t target_rl;
	int16_t target_rr;
    uint8_t battery_percent;
    int8_t temperature;
    uint16_t sonar_l;
    uint16_t sonar_c;
    uint16_t sonar_r;
    BackwardMode backward_mode;
    OperatingMode operating_mode;
    LightOperatingMode light_mode;
} Telemetry_t;

void telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c);

HAL_StatusTypeDef telecontrol_read(Controller_t *telecontrol);


HAL_StatusTypeDef telecontrol_send_telemetry(Controller_t *telecontrol,Telemetry_t *telemetry);

uint8_t telecontrol_is_alive(Controller_t *telecontrol);


uint16_t get_telecontrol_ay(Controller_t *telecontrol);
uint8_t get_telecontrol_a_btn(Controller_t *telecontrol);

uint16_t get_telecontrol_bx(Controller_t *telecontrol);
uint8_t  get_telecontrol_b_btn(Controller_t *telecontrol);

uint8_t  get_telecontrol_button_btn1(Controller_t *telecontrol);
uint8_t  get_telecontrol_button_btn2(Controller_t *telecontrol);
uint8_t  get_telecontrol_button_btn3(Controller_t *telecontrol);
uint8_t  get_telecontrol_button_btn4(Controller_t *telecontrol);

uint8_t get_telecontrol_percentage(Controller_t *telecontrol);

#endif /* INC_TELEMETRY_H_ */
