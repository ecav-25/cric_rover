#include "controller.h"

/* ===================== INIT ===================== */

ControllerStatus_t telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c){
    if (telecontrol == NULL || i2c == NULL) {
        return CONTROLLER_ERR;
    }

    telecontrol->i2c = i2c;

    telecontrol->controller_information.alive = 0;
    telecontrol->controller_information.controller_percentage = 0;

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

    return CONTROLLER_OK;
}

/* ===================== READ ===================== */

ControllerStatus_t telecontrol_read(Controller_t *telecontrol){
    if (telecontrol == NULL || telecontrol->i2c == NULL) {
        return CONTROLLER_ERR;
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(telecontrol->i2c, 0x55 << 1, (uint8_t *)&telecontrol->controller_information, sizeof(controller_information_t), 20u);

    if (status == HAL_OK) {
        return CONTROLLER_OK;
    } else {
        return CONTROLLER_ERR_COMM;
    }
}

/* ===================== TELEMETRY TX ===================== */

ControllerStatus_t telecontrol_send_telemetry(Controller_t *telecontrol, Telemetry_t *telemetry){
    if (telecontrol == NULL || telecontrol->i2c == NULL || telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(telecontrol->i2c, 0x55 << 1, (uint8_t *)telemetry, sizeof(Telemetry_t), 100);

    if (status == HAL_OK) {
        return CONTROLLER_OK;
    } else {
        return CONTROLLER_ERR_COMM;
    }
}

/* ===================== GETTERS ===================== */

ControllerStatus_t telecontrol_is_alive(Controller_t *telecontrol, uint8_t *alive)
{
    if (telecontrol == NULL || alive == NULL) {
        return CONTROLLER_ERR;
    }

    *alive = telecontrol->controller_information.alive;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_percentage(Controller_t *telecontrol, uint8_t *percentage)
{
    if (telecontrol == NULL || percentage == NULL) {
        return CONTROLLER_ERR;
    }

    *percentage = telecontrol->controller_information.controller_percentage;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_ay(Controller_t *telecontrol, uint16_t *ay)
{
    if (telecontrol == NULL || ay == NULL) {
        return CONTROLLER_ERR;
    }

    *ay = telecontrol->controller_information.controller_data.ay;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_a_btn(Controller_t *telecontrol, uint8_t *a_btn)
{
    if (telecontrol == NULL || a_btn == NULL) {
        return CONTROLLER_ERR;
    }

    *a_btn = telecontrol->controller_information.controller_data.a_btn;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_bx(Controller_t *telecontrol, uint16_t *bx)
{
    if (telecontrol == NULL || bx == NULL) {
        return CONTROLLER_ERR;
    }

    *bx = telecontrol->controller_information.controller_data.bx;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_b_btn(Controller_t *telecontrol, uint8_t *b_btn)
{
    if (telecontrol == NULL || b_btn == NULL) {
        return CONTROLLER_ERR;
    }

    *b_btn = telecontrol->controller_information.controller_data.b_btn;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_button_btn1(Controller_t *telecontrol, uint8_t *btn)
{
    if (telecontrol == NULL || btn == NULL) {
        return CONTROLLER_ERR;
    }

    *btn = telecontrol->controller_information.controller_data.btn1;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_button_btn2(Controller_t *telecontrol, uint8_t *btn)
{
    if (telecontrol == NULL || btn == NULL) {
        return CONTROLLER_ERR;
    }

    *btn = telecontrol->controller_information.controller_data.btn2;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_button_btn3(Controller_t *telecontrol, uint8_t *btn)
{
    if (telecontrol == NULL || btn == NULL) {
        return CONTROLLER_ERR;
    }

    *btn = telecontrol->controller_information.controller_data.btn3;
    return CONTROLLER_OK;
}

ControllerStatus_t get_telecontrol_button_btn4(Controller_t *telecontrol, uint8_t *btn)
{
    if (telecontrol == NULL || btn == NULL) {
        return CONTROLLER_ERR;
    }

    *btn = telecontrol->controller_information.controller_data.btn4;
    return CONTROLLER_OK;
}

/* ===================== TELEMETRY ===================== */

ControllerStatus_t telemetry_init(Telemetry_t *telemetry)
{
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->rpm_fl = 0;
    telemetry->rpm_fr = 0;
    telemetry->rpm_rl = 0;
    telemetry->rpm_rr = 0;

    telemetry->target_fl = 0;
    telemetry->target_fr = 0;
    telemetry->target_rl = 0;
    telemetry->target_rr = 0;

    telemetry->battery_percent = 0;
    telemetry->temperature = 0;

    telemetry->sonar_l = 0;
    telemetry->sonar_c = 0;
    telemetry->sonar_r = 0;

    telemetry->backward_mode = BW_NORMAL;
    telemetry->operating_mode = OM_NORMAL;
    telemetry->light_mode = LIGHT_OFF;

    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_light_mode(Telemetry_t *telemetry, LightOperatingMode mode){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->light_mode = mode;
    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_operating_mode(Telemetry_t *telemetry, OperatingMode mode){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->operating_mode = mode;
    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_backward_mode(Telemetry_t *telemetry, BackwardMode mode){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->backward_mode = mode;
    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_driving_mode(Telemetry_t *telemetry, DrivingMode  mode){
	if (telemetry == NULL) {
		return CONTROLLER_ERR;
	}

	telemetry->driving_mode = mode;
	return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_obstacle_avoidance_mode(Telemetry_t *telemetry, ObstacleAvoidanceMode  mode){
	if (telemetry == NULL) {
		return CONTROLLER_ERR;
	}

	telemetry->obstacle_mode = mode;
	return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_battery(Telemetry_t *telemetry, uint8_t percent){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->battery_percent = percent;
    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_temperature(Telemetry_t *telemetry, int8_t temp){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->temperature = temp;
    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_rpm(Telemetry_t *telemetry, int16_t fl, int16_t fr, int16_t rl, int16_t rr){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->rpm_fl = fl;
    telemetry->rpm_fr = fr;
    telemetry->rpm_rl = rl;
    telemetry->rpm_rr = rr;

    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_targets(Telemetry_t *telemetry, int16_t fl, int16_t fr, int16_t rl, int16_t rr){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->target_fl = fl;
    telemetry->target_fr = fr;
    telemetry->target_rl = rl;
    telemetry->target_rr = rr;

    return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_max_velocity(Telemetry_t *telemetry, uint8_t max_vel){
	if (telemetry == NULL) {
		return CONTROLLER_ERR;
	}

	telemetry->max_velocity = max_vel;

	return CONTROLLER_OK;
}

ControllerStatus_t telemetry_set_sonars(Telemetry_t *telemetry, uint16_t left, uint16_t center, uint16_t right){
    if (telemetry == NULL) {
        return CONTROLLER_ERR;
    }

    telemetry->sonar_l = left;
    telemetry->sonar_c = center;
    telemetry->sonar_r = right;

    return CONTROLLER_OK;
}
