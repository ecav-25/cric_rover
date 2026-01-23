#include "controller.h"

void telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c){
	telecontrol->i2c = i2c;

	telecontrol->controller_information.alive = 0;

	telecontrol->controller_information.controller_data.a_btn = 0;
	telecontrol->controller_information.controller_data.ax = CONTROLLERZERO;
	telecontrol->controller_information.controller_data.ay = CONTROLLERZERO;

	telecontrol->controller_information.controller_data.b_btn = 0;
	telecontrol->controller_information.controller_data.bx = CONTROLLERZERO;
	telecontrol->controller_information.controller_data.by = CONTROLLERZERO;

	telecontrol->controller_information.controller_data.btn1 = 0;
	telecontrol->controller_information.controller_data.btn2 = 0;
	telecontrol->controller_information.controller_data.btn3 = 0;
	telecontrol->controller_information.controller_data.btn4 = 0;
}

HAL_StatusTypeDef telecontrol_read(Controller_t *telecontrol){
	return HAL_I2C_Master_Receive(telecontrol->i2c, 0x55 << 1, (uint8_t *)&(telecontrol->controller_information), sizeof(controller_information_t), 20u);
}

HAL_StatusTypeDef telecontrol_send_telemetry(Controller_t *telecontrol,Telemetry_t *telemetry){
	return HAL_I2C_Master_Transmit(telecontrol->i2c, 0x55 << 1, (uint8_t *)(telemetry), sizeof(Telemetry_t), 20u);
}

uint8_t telecontrol_is_alive(Controller_t *telecontrol) {
    return telecontrol->controller_information.alive;
}

uint16_t get_telecontrol_ay(Controller_t *telecontrol) {
    return (uint16_t)(telecontrol->controller_information.controller_data.ay);
}


uint8_t get_telecontrol_a_btn(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.a_btn;
}

uint16_t get_telecontrol_bx(Controller_t *telecontrol) {
    return (uint16_t)(telecontrol->controller_information.controller_data.bx);
}

uint8_t get_telecontrol_b_btn(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.b_btn;
}

uint8_t get_telecontrol_button_btn1(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.btn1;
}

uint8_t get_telecontrol_button_btn2(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.btn2;
}

uint8_t get_telecontrol_button_btn3(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.btn3;
}

uint8_t get_telecontrol_button_btn4(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_data.btn4;
}

uint8_t get_telecontrol_percentage(Controller_t *telecontrol) {
    return telecontrol->controller_information.controller_percentage;
}

