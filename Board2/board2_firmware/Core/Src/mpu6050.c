#include "mpu6050.h"
#include "stddef.h"
/* -------------------- Costanti interne -------------------- */

#define MPU6050_I2C_TIMEOUT_MS   100u

/* -------------------- Low-level helpers (static) -------------------- */

static MPU60X0_StatusTypeDef mpu6050_write_reg(mpu6050_t* dev, uint8_t reg, uint8_t val)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if (dev != NULL)
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write(dev->i2c, dev->address, (uint16_t)reg,
                                                         I2C_MEMADD_SIZE_8BIT, &val, 1U,
                                                         MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_read_reg(mpu6050_t* dev, uint8_t reg, uint8_t* val)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if ((dev != NULL) && (val != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(dev->i2c, dev->address, (uint16_t)reg,
                                                        I2C_MEMADD_SIZE_8BIT, val, 1U,
                                                        MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_read_bytes(mpu6050_t* dev, uint8_t reg, uint8_t* buf, uint16_t len)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR_COMM;

    if ((dev != NULL) && (buf != NULL))
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(dev->i2c, dev->address, (uint16_t)reg,
                                                        I2C_MEMADD_SIZE_8BIT, buf, len,
                                                        MPU6050_I2C_TIMEOUT_MS);

        if (hal_status == HAL_OK)
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_read_axis(mpu6050_t* dev, uint8_t reg_h, float sensitivity, float* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[2];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, reg_h, buf, 2U) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            /* MISRA: Converti in unsigned per l'operazione bitwise, poi casta a signed */
            uint16_t raw_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            int16_t raw_s = (int16_t)raw_u;

            *out = (float)raw_s / sensitivity;
            status = MPU6050_OK;
        }
    }

    return status;
}


/* -------------------- Sensibilità (g, °/s) -------------------- */

static float mpu6050_accel_sensitivity(mpu6050_accel_fs_t range)
{
    float sensitivity;

    switch (range)
    {
        case MPU6050_ACCEL_FS_2G:
            sensitivity = 16384.0f;
            break;
        case MPU6050_ACCEL_FS_4G:
            sensitivity = 8192.0f;
            break;
        case MPU6050_ACCEL_FS_8G:
            sensitivity = 4096.0f;
            break;
        case MPU6050_ACCEL_FS_16G:
            sensitivity = 2048.0f;
            break;
        default:
            sensitivity = 16384.0f;
            break;
    }

    return sensitivity;
}

static float mpu6050_gyro_sensitivity(mpu6050_gyro_fs_t range)
{
    float sensitivity;

    switch (range)
    {
        case MPU6050_GYRO_FS_250DPS:
            sensitivity = 131.0f;
            break;
        case MPU6050_GYRO_FS_500DPS:
            sensitivity = 65.5f;
            break;
        case MPU6050_GYRO_FS_1000DPS:
            sensitivity = 32.8f;
            break;
        case MPU6050_GYRO_FS_2000DPS:
            sensitivity = 16.4f;
            break;
        default:
            sensitivity = 131.0f;
            break;
    }

    return sensitivity;
}

/* -------------------- Helper configurazione (static) -------------------- */

static MPU60X0_StatusTypeDef mpu6050_reset(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, PWR_MGMT_1, DEVICE_RESET) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            HAL_Delay(100U);
            status = MPU6050_OK;
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_wake(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, PWR_MGMT_1, PWR_MGMT_WAKE) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            status = MPU6050_OK;
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_set_accel_range_lowlevel(mpu6050_t* dev, mpu6050_accel_fs_t range)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;

    if (dev != NULL)
    {
        if (mpu6050_read_reg(dev, ACCEL_CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            reg &= (uint8_t)(~0x18U);
            reg |= (uint8_t)((uint8_t)range << 3U);

            if (mpu6050_write_reg(dev, ACCEL_CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

static MPU60X0_StatusTypeDef mpu6050_set_gyro_range_lowlevel(mpu6050_t* dev, mpu6050_gyro_fs_t range)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;

    if (dev != NULL)
    {
        if (mpu6050_read_reg(dev, GYRO_CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            reg &= (uint8_t)(~0x18U);
            reg |= (uint8_t)((uint8_t)range << 3U);

            if (mpu6050_write_reg(dev, GYRO_CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                status = MPU6050_OK;
            }
        }
    }

    return status;
}


static MPU60X0_StatusTypeDef mpu6050_apply_config(mpu6050_t* dev, const mpu6050_config_t* cfg)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if ((dev != NULL) && (cfg != NULL))
    {
        st = mpu6050_set_dlpf(dev, cfg->dlpf_cfg);

        if (st == MPU6050_OK)
        {
            st = mpu6050_set_sample_div(dev, cfg->smplrt_div);
        }

        if (st == MPU6050_OK)
        {
            st = mpu6050_set_accel_fs(dev, cfg->accel_range);
        }

        if (st == MPU6050_OK)
        {
            st = mpu6050_set_gyro_fs(dev, cfg->gyro_range);
        }

        if (st == MPU6050_OK)
        {
            st = mpu6050_set_interrupt_mask(dev, cfg->int_enable);
        }
    }

    return st;
}

/* -------------------- API di base -------------------- */

MPU60X0_StatusTypeDef mpu6050_init(mpu6050_t* dev, I2C_HandleTypeDef* i2c, uint16_t address, const mpu6050_config_t* mpu_cfg)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if ((dev != NULL) && (i2c != NULL) && (mpu_cfg != NULL))
    {
        dev->i2c = i2c;

        if (address != 0U)
        {
            dev->address = address;
        }
        else
        {
            dev->address = MPU60X0_ADDRESS;
        }

        if (mpu6050_reset(dev) != MPU6050_OK)
        {
            st = MPU6050_ERR_COMM;
        }
        else if (mpu6050_wake(dev) != MPU6050_OK)
        {
            st = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg = *mpu_cfg;
            st = mpu6050_apply_config(dev, &dev->mpu6050_cfg);
        }
    }

    return st;
}

MPU60X0_StatusTypeDef mpu6050_recovery_init(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef st = MPU6050_ERR;

    if (dev != NULL)
    {
        st = mpu6050_signal_path_reset(dev);

        if (st == MPU6050_OK)
        {
            st = mpu6050_apply_config(dev, &dev->mpu6050_cfg);
        }
    }

    return st;
}


MPU60X0_StatusTypeDef mpu6050_who_am_i(mpu6050_t* dev, uint8_t* id)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (id != NULL))
    {
        status = mpu6050_read_reg(dev, WHO_AM_I, id);
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_accel_value(mpu6050_t* dev, imu_vector_t* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[6];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, ACCEL_XOUT_H, buf, (uint16_t)sizeof(buf)) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            /* MISRA: Operazioni bitwise eseguite su uint16_t prima del cast a signed */
            uint16_t rawX_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            uint16_t rawY_u = ((uint16_t)buf[2] << 8U) | (uint16_t)buf[3];
            uint16_t rawZ_u = ((uint16_t)buf[4] << 8U) | (uint16_t)buf[5];

            int16_t rawX_s = (int16_t)rawX_u;
            int16_t rawY_s = (int16_t)rawY_u;
            int16_t rawZ_s = (int16_t)rawZ_u;

            float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);

            out->x = (float)rawX_s / sens;
            out->y = (float)rawY_s / sens;
            out->z = (float)rawZ_s / sens;

            dev->data.accel = *out;
            status = MPU6050_OK;
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_value(mpu6050_t* dev, imu_vector_t* out)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t buf[6];

    if ((dev != NULL) && (out != NULL))
    {
        if (mpu6050_read_bytes(dev, GYRO_XOUT_H, buf, (uint16_t)sizeof(buf)) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
        	uint16_t rawX_u = ((uint16_t)buf[0] << 8U) | (uint16_t)buf[1];
            uint16_t rawY_u = ((uint16_t)buf[2] << 8U) | (uint16_t)buf[3];
            uint16_t rawZ_u = ((uint16_t)buf[4] << 8U) | (uint16_t)buf[5];


            int16_t rawX_s = (int16_t)rawX_u;
            int16_t rawY_s = (int16_t)rawY_u;
            int16_t rawZ_s = (int16_t)rawZ_u;

            float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);

            out->x = (float)rawX_s / sens;
            out->y = (float)rawY_s / sens;
            out->z = (float)rawZ_s / sens;

            dev->data.gyro = *out;
            status = MPU6050_OK;
        }
    }

    return status;
}

/* -------------------- API di configurazione -------------------- */

MPU60X0_StatusTypeDef mpu6050_signal_path_reset(mpu6050_t* dev)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        status = mpu6050_write_reg(dev, SIGNAL_PATH_RESET, (uint8_t)(GYRO_RESET | ACCEL_RESET | TEMP_RESET));
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_set_dlpf(mpu6050_t* dev, uint8_t dlpf_cfg)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;
    uint8_t reg = 0U;
    uint8_t cfg_masked;

    if (dev != NULL)
    {
        cfg_masked = (dlpf_cfg & 0x07U);

        if (mpu6050_read_reg(dev, CONFIG, &reg) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            reg &= (uint8_t)(~0x07U);
            reg |= cfg_masked;

            if (mpu6050_write_reg(dev, CONFIG, reg) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.dlpf_cfg = cfg_masked;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_dlpf(mpu6050_t* dev, uint8_t* dlpf_cfg)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (dlpf_cfg != NULL))
    {
        *dlpf_cfg = dev->mpu6050_cfg.dlpf_cfg;
        status = MPU6050_OK;
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_set_sample_div(mpu6050_t* dev, uint8_t smplrt_div)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, SMPRT_DIV, smplrt_div) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg.smplrt_div = smplrt_div;
            status = MPU6050_OK;
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_sample_div(mpu6050_t* dev, uint8_t* smplrt_div)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (smplrt_div != NULL))
    {
        *smplrt_div = dev->mpu6050_cfg.smplrt_div;
        status = MPU6050_OK;
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_set_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (fs <= MPU6050_ACCEL_FS_16G)
        {
            if (mpu6050_set_accel_range_lowlevel(dev, fs) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.accel_range = fs;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t* fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (fs != NULL))
    {
        *fs = dev->mpu6050_cfg.accel_range;
        status = MPU6050_OK;
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_set_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (fs <= MPU6050_GYRO_FS_2000DPS)
        {
            if (mpu6050_set_gyro_range_lowlevel(dev, fs) != MPU6050_OK)
            {
                status = MPU6050_ERR_COMM;
            }
            else
            {
                dev->mpu6050_cfg.gyro_range = fs;
                status = MPU6050_OK;
            }
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t* fs)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (fs != NULL))
    {
        *fs = dev->mpu6050_cfg.gyro_range;
        status = MPU6050_OK;
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_set_interrupt_mask(mpu6050_t* dev, uint8_t int_mask)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if (dev != NULL)
    {
        if (mpu6050_write_reg(dev, INT_ENABLE, int_mask) != MPU6050_OK)
        {
            status = MPU6050_ERR_COMM;
        }
        else
        {
            dev->mpu6050_cfg.int_enable = int_mask;
            status = MPU6050_OK;
        }
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_interrupt_mask(mpu6050_t* dev, uint8_t* int_mask)
{
    MPU60X0_StatusTypeDef status = MPU6050_ERR;

    if ((dev != NULL) && (int_mask != NULL))
    {
        *int_mask = dev->mpu6050_cfg.int_enable;
        status = MPU6050_OK;
    }

    return status;
}

MPU60X0_StatusTypeDef mpu6050_get_accel_x(mpu6050_t* dev, float* ax){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_XOUT_H, sens, ax);
}

MPU60X0_StatusTypeDef mpu6050_get_accel_y(mpu6050_t* dev, float* ay){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_YOUT_H, sens, ay);
}

MPU60X0_StatusTypeDef mpu6050_get_accel_z(mpu6050_t* dev, float* az){
    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);
    return mpu6050_read_axis(dev, ACCEL_ZOUT_H, sens, az);
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_x(mpu6050_t* dev, float* gx){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_XOUT_H, sens, gx);
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_y(mpu6050_t* dev, float* gy){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_YOUT_H, sens, gy);
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_z(mpu6050_t* dev, float* gz){
    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);
    return mpu6050_read_axis(dev, GYRO_ZOUT_H, sens, gz);
}


