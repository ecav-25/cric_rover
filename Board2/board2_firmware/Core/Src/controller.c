#include "controller.h"

static inline uint8_t crc8_le(uint8_t crc, const uint8_t *data, uint16_t len){
        crc = ~crc;

        while (len > 0U) {
            crc ^= *data++;
            for (uint8_t i = 0; i < 8U; i++) {
                if ((crc & 0x01U) != 0U) {
                    crc = (crc >> 1) ^ 0xE0;
                } else {
                    crc >>= 1;
                }
            }
            len--;
        }

        return ~crc;
}

/* ===================== INIT ===================== */

ControllerStatus_t telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c, uint16_t address)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (i2c != NULL))
    {
        telecontrol->i2c = i2c;
        telecontrol->address = address;

        telecontrol->controller_information.alive = 0U;
        telecontrol->controller_information.controller_percentage = 0U;
        telecontrol->controller_information.crc_value = 0U;

        telecontrol->controller_information.controller_data.a_btn = 0U;
        telecontrol->controller_information.controller_data.ax = CONTROLLERZERO;
        telecontrol->controller_information.controller_data.ay = CONTROLLERZERO;

        telecontrol->controller_information.controller_data.b_btn = 0U;
        telecontrol->controller_information.controller_data.bx = CONTROLLERZERO;
        telecontrol->controller_information.controller_data.by = CONTROLLERZERO;

        telecontrol->controller_information.controller_data.btn1 = 0U;
        telecontrol->controller_information.controller_data.btn2 = 0U;
        telecontrol->controller_information.controller_data.btn3 = 0U;
        telecontrol->controller_information.controller_data.btn4 = 0U;

        telecontrol->crc_error_count = 0U;
        telecontrol->last_valid_information = telecontrol->controller_information;

        status = CONTROLLER_OK;
    }

    return status;
}
/* ===================== READ ===================== */

ControllerStatus_t telecontrol_read(Controller_t *telecontrol)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (telecontrol->i2c != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Master_Receive(
            telecontrol->i2c,
            (uint16_t)(telecontrol->address << 1U),
            (uint8_t *)&telecontrol->controller_information,
            (uint16_t)sizeof(controller_information_t),
            20U
        );

        if (hal_status == HAL_OK)
        {
            uint8_t crc = 0U;

            crc = crc8_le(
                crc,
                (uint8_t *)&telecontrol->controller_information.controller_data,
                (size_t)sizeof(controller_data_t)
            );

            crc = crc8_le(
                crc,
                &telecontrol->controller_information.controller_percentage,
                (size_t)sizeof(uint8_t)
            );

            crc = crc8_le(
                crc,
                &telecontrol->controller_information.alive,
                (size_t)sizeof(uint8_t)
            );

            if (crc == telecontrol->controller_information.crc_value)
            {
                status = CONTROLLER_OK;
            }
            else
            {
                status = CONTROLLER_ERR_CRC;
            }
        }
        else
        {
            status = CONTROLLER_ERR_COMM;
        }
    }

    return status;
}

/* ===================== TELEMETRY TX ===================== */

ControllerStatus_t telecontrol_send_telemetry(Controller_t *telecontrol, Telemetry_t *telemetry)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (telecontrol->i2c != NULL) && (telemetry != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Master_Transmit(
            telecontrol->i2c,
            (uint16_t)(0x55U << 1U),
            (uint8_t *)telemetry,
            (uint16_t)sizeof(Telemetry_t),
            100U
        );

        if (hal_status == HAL_OK)
        {
            status = CONTROLLER_OK;
        }
        else
        {
            status = CONTROLLER_ERR_COMM;
        }
    }

    return status;
}

/* ===================== TELEMETRY ===================== */

ControllerStatus_t telemetry_init(Telemetry_t *telemetry)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->rpm_FL = 0U;
        telemetry->rpm_FR = 0U;
        telemetry->rpm_RL = 0U;
        telemetry->rpm_RR = 0U;

        telemetry->battery_percent = 0U;
        telemetry->temperature = 0;

        telemetry->sonar_l = 0U;
        telemetry->sonar_c = 0U;
        telemetry->sonar_r = 0U;

        telemetry->backward_mode = BW_NORMAL;
        telemetry->operating_mode = OM_NORMAL;
        telemetry->light_mode = LIGHT_OFF;
        telemetry->driving_mode = DEFAULT;
        telemetry->obstacle_mode = COMPLETE;
        telemetry->gyro_status = GYRO_STATUS_READY;
        telemetry->traction_status = T_HEALTHY;

        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_light_mode(Telemetry_t *telemetry, LightOperatingMode mode)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->light_mode = mode;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_operating_mode(Telemetry_t *telemetry, OperatingMode mode)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->operating_mode = mode;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_backward_mode(Telemetry_t *telemetry, BackwardMode mode)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->backward_mode = mode;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_driving_mode(Telemetry_t *telemetry, DrivingMode mode)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->driving_mode = mode;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_obstacle_avoidance_mode(Telemetry_t *telemetry, ObstacleAvoidanceMode mode)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->obstacle_mode = mode;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_battery(Telemetry_t *telemetry, uint8_t percent)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->battery_percent = percent;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_temperature(Telemetry_t *telemetry, int8_t temp)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->temperature = temp;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_rpm(Telemetry_t *telemetry, int16_t fL, int16_t fR, int16_t rL, int16_t rR)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->rpm_FL = fL;
        telemetry->rpm_FR = fR;
        telemetry->rpm_RL = rL;
        telemetry->rpm_RR = rR;

        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_max_velocity(Telemetry_t *telemetry, uint8_t max_vel)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->max_velocity = max_vel;
        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_sonars(Telemetry_t *telemetry, uint16_t left, uint16_t center, uint16_t right)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->sonar_l = left;
        telemetry->sonar_c = center;
        telemetry->sonar_r = right;

        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_gyro_status(Telemetry_t *telemetry, GyroAvailability gyro_status)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->gyro_status = gyro_status;

        status = CONTROLLER_OK;
    }

    return status;
}

ControllerStatus_t telemetry_set_traction_health(Telemetry_t *telemetry, Traction_Health traction_status)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if (telemetry != NULL)
    {
        telemetry->traction_status = traction_status;

        status = CONTROLLER_OK;
    }

    return status;
}
