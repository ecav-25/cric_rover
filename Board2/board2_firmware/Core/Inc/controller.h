#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "i2c.h"
#include <stdint.h>

#define CONTROLLERZERO (255)
#define CONTROLLER_MAX_CRC_ERRORS   3u


/* ===================== STATUS ===================== */

typedef enum {
    CONTROLLER_OK = 0,
    CONTROLLER_ERR,
	CONTROLLER_ERR_COMM,
    CONTROLLER_ERR_CRC
} ControllerStatus_t;

/* ===================== CONTROLLER DATA ===================== */

typedef struct __attribute__((packed)) {
    uint16_t ax;
    uint16_t ay;
    uint8_t  a_btn;

    uint16_t bx;
    uint16_t by;
    uint8_t  b_btn;

    uint8_t btn1;
    uint8_t btn2;
    uint8_t btn3;
    uint8_t btn4;
} controller_data_t;

typedef struct __attribute__((packed)) {
    uint8_t alive;
    uint8_t controller_percentage;
    controller_data_t controller_data;
    uint8_t crc_value;
} controller_information_t;

typedef struct {
    I2C_HandleTypeDef* i2c;
    uint16_t address;
    controller_information_t controller_information;
    controller_information_t last_valid_information;
	uint8_t crc_error_count;
} Controller_t;

/* ===================== TELEMETRY ===================== */

typedef uint8_t BackwardMode;
enum {
    BW_NORMAL = 0,
    BW_SPECIAL
};

typedef uint8_t OperatingMode;
enum {
    OM_NORMAL = 0,
    OM_SINGLE_BOARD,
    OM_DEGRADED
};

typedef uint8_t LightOperatingMode;
enum {
    LIGHT_OFF = 0,
    LIGHT_ON,
    LIGHT_AUTO
};

typedef uint8_t DrivingMode;
enum {
    DM_DEFAULT = 0,
    DM_SPORT,
    DM_ECO
};

typedef uint8_t ObstacleAvoidanceMode;
enum {
    COMPLETE = 0,
    MINIMAL
};

typedef uint8_t GyroAvailability;
enum {
	GYRO_STATUS_DEGRADED = 0,
    GYRO_STATUS_READY
};

typedef uint8_t Traction_Health;
enum {
	T_UNHEALTHY = 0,
    T_HEALTHY
};


typedef struct __attribute__((packed)) {
	uint16_t sonar_l;
	uint16_t sonar_c;
	uint16_t sonar_r;

    int16_t rpm_FL;
    int16_t rpm_FR;
    int16_t rpm_RL;
    int16_t rpm_RR;

    uint8_t max_velocity;

    uint8_t battery_percent;
    int8_t  temperature;

    BackwardMode backward_mode;
    OperatingMode operating_mode;
    LightOperatingMode light_mode;
    DrivingMode driving_mode;
    ObstacleAvoidanceMode obstacle_mode;
    GyroAvailability gyro_status;
    Traction_Health traction_status;
} Telemetry_t;

/* ===================== CONTROLLER API ===================== */

ControllerStatus_t telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c, uint16_t address);

ControllerStatus_t telecontrol_read(Controller_t *telecontrol);

ControllerStatus_t telecontrol_send_telemetry(Controller_t *telecontrol, Telemetry_t *telemetry);

/* ===================== GETTERS (INLINE) ===================== */

static inline ControllerStatus_t get_telecontrol_bx(const Controller_t *telecontrol, uint16_t *bx)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (bx != NULL))
    {
        *bx = telecontrol->controller_information.controller_data.bx;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_ay(const Controller_t *telecontrol, uint16_t *ay)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (ay != NULL))
    {
        *ay = telecontrol->controller_information.controller_data.ay;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_button_btn1(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn1;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_button_btn2(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn2;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_button_btn3(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn3;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_button_btn4(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn4;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_b_btn(const Controller_t *telecontrol, uint8_t *b_btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (b_btn != NULL))
    {
        *b_btn = telecontrol->controller_information.controller_data.b_btn;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_a_btn(const Controller_t *telecontrol, uint8_t *a_btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (a_btn != NULL))
    {
        *a_btn = telecontrol->controller_information.controller_data.a_btn;
        status = CONTROLLER_OK;
    }

    return status;
}

static inline ControllerStatus_t get_telecontrol_percentage(const Controller_t *telecontrol, uint8_t *percentage)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (percentage != NULL))
    {
        *percentage = telecontrol->controller_information.controller_percentage;
        status = CONTROLLER_OK;
    }

    return status;
}

/* ===================== TELEMETRY API ===================== */

ControllerStatus_t telemetry_init(Telemetry_t *telemetry);

ControllerStatus_t telemetry_set_light_mode(Telemetry_t *telemetry, LightOperatingMode mode);

ControllerStatus_t telemetry_set_operating_mode(Telemetry_t *telemetry, OperatingMode mode);

ControllerStatus_t telemetry_set_backward_mode(Telemetry_t *telemetry, BackwardMode mode);

ControllerStatus_t telemetry_set_driving_mode(Telemetry_t *telemetry, DrivingMode  mode);

ControllerStatus_t telemetry_set_obstacle_avoidance_mode(Telemetry_t *telemetry, ObstacleAvoidanceMode  mode);

ControllerStatus_t telemetry_set_battery(Telemetry_t *telemetry, uint8_t percent);

ControllerStatus_t telemetry_set_temperature(Telemetry_t *telemetry, int8_t temp);

ControllerStatus_t telemetry_set_rpm(Telemetry_t *telemetry, int16_t fl, int16_t fr,int16_t rl, int16_t rr);

ControllerStatus_t telemetry_set_max_velocity(Telemetry_t *telemetry, uint8_t max_vel);

ControllerStatus_t telemetry_set_sonars(Telemetry_t *telemetry, uint16_t left, uint16_t center, uint16_t right);

ControllerStatus_t telemetry_set_gyro_status(Telemetry_t *telemetry, GyroAvailability gyro_status);

ControllerStatus_t telemetry_set_traction_health(Telemetry_t *telemetry, Traction_Health traction_status);

#endif /* INC_CONTROLLER_H_ */
