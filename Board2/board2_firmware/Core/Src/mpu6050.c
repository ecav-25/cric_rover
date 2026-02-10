#include "mpu6050.h"
#include "stddef.h"
/* -------------------- Costanti interne -------------------- */

#define MPU6050_I2C_TIMEOUT_MS   100u

/* -------------------- Low-level helpers (static) -------------------- */

static MPU60X0_StatusTypeDef mpu6050_write_reg(mpu6050_t* dev, uint8_t reg, uint8_t val){
    if (HAL_I2C_Mem_Write(dev->i2c, dev->address, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, MPU6050_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return MPU6050_ERR_COMM;
    }
    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_read_reg(mpu6050_t* dev, uint8_t reg, uint8_t* val){
    if (HAL_I2C_Mem_Read(dev->i2c, dev->address, reg, I2C_MEMADD_SIZE_8BIT, val, 1, MPU6050_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return MPU6050_ERR_COMM;
    }
    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_read_bytes(mpu6050_t* dev, uint8_t reg, uint8_t* buf, uint16_t len){
    if (HAL_I2C_Mem_Read(dev->i2c, dev->address, reg, I2C_MEMADD_SIZE_8BIT, buf, len, MPU6050_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return MPU6050_ERR_COMM;
    }
    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_read_axis(mpu6050_t* dev, uint8_t reg_h, float sensitivity, float* out){
    if (dev == NULL || out == NULL)
        return MPU6050_ERR;

    uint8_t buf[2];

    if (mpu6050_read_bytes(dev, reg_h, buf, 2) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    int16_t raw = (int16_t)((buf[0] << 8) | buf[1]);
    *out = (float)raw / sensitivity;

    return MPU6050_OK;
}


/* -------------------- Sensibilità (g, °/s) -------------------- */

static float mpu6050_accel_sensitivity(mpu6050_accel_fs_t range){
    switch (range) {
        case MPU6050_ACCEL_FS_2G:  return 16384.0f;
        case MPU6050_ACCEL_FS_4G:  return 8192.0f;
        case MPU6050_ACCEL_FS_8G:  return 4096.0f;
        case MPU6050_ACCEL_FS_16G: return 2048.0f;
        default:                   return 16384.0f;
    }
}

static float mpu6050_gyro_sensitivity(mpu6050_gyro_fs_t range){
    switch (range) {
        case MPU6050_GYRO_FS_250DPS:  return 131.0f;
        case MPU6050_GYRO_FS_500DPS:  return 65.5f;
        case MPU6050_GYRO_FS_1000DPS: return 32.8f;
        case MPU6050_GYRO_FS_2000DPS: return 16.4f;
        default:                      return 131.0f;
    }
}

/* -------------------- Helper configurazione (static) -------------------- */

static MPU60X0_StatusTypeDef mpu6050_reset(mpu6050_t* dev){
    if (mpu6050_write_reg(dev, PWR_MGMT_1, DEVICE_RESET) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    HAL_Delay(100);
    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_wake(mpu6050_t* dev){
    if (mpu6050_write_reg(dev, PWR_MGMT_1, PWR_MGMT_WAKE) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    //HAL_Delay(10);
    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_set_accel_range_lowlevel(mpu6050_t* dev, mpu6050_accel_fs_t range){
    uint8_t reg = 0u;

    if (mpu6050_read_reg(dev, ACCEL_CONFIG, &reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    reg &= ~(0x18u);                 /* clear AFS_SEL[4:3] */
    reg |= ((uint8_t)range << 3);

    if (mpu6050_write_reg(dev, ACCEL_CONFIG, reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    return MPU6050_OK;
}

static MPU60X0_StatusTypeDef mpu6050_set_gyro_range_lowlevel(mpu6050_t* dev, mpu6050_gyro_fs_t range){
    uint8_t reg = 0u;

    if (mpu6050_read_reg(dev, GYRO_CONFIG, &reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    reg &= ~(0x18u);                 /* clear FS_SEL[4:3] */
    reg |= ((uint8_t)range << 3);

    if (mpu6050_write_reg(dev, GYRO_CONFIG, reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    return MPU6050_OK;
}

/* Applica tutta la config corrente (usata in init o se vuoi un reset completo) */
static MPU60X0_StatusTypeDef mpu6050_apply_config(mpu6050_t* dev, const mpu6050_config_t* cfg){
    MPU60X0_StatusTypeDef st;

    st = mpu6050_set_dlpf(dev, cfg->dlpf_cfg);
    if (st != MPU6050_OK) return st;

    st = mpu6050_set_sample_div(dev, cfg->smplrt_div);
    if (st != MPU6050_OK) return st;

    st = mpu6050_set_accel_fs(dev, cfg->accel_range);
    if (st != MPU6050_OK) return st;

    st = mpu6050_set_gyro_fs(dev, cfg->gyro_range);
    if (st != MPU6050_OK) return st;

    st = mpu6050_set_interrupt_mask(dev, cfg->int_enable);
    if (st != MPU6050_OK) return st;

    return MPU6050_OK;
}


/* -------------------- API di base -------------------- */

MPU60X0_StatusTypeDef mpu6050_init(mpu6050_t* dev, I2C_HandleTypeDef* i2c, uint16_t address, const mpu6050_config_t* mpu_cfg){
	MPU60X0_StatusTypeDef st;

	if (dev == NULL || i2c == NULL || mpu_cfg == NULL){
		return MPU6050_ERR;
    }

    dev->i2c     = i2c;
    dev->address = (address != 0u) ? address : MPU60X0_ADDRESS;

    if (mpu6050_reset(dev) != MPU6050_OK){
        return MPU6050_ERR_COMM;
    }

    if (mpu6050_wake(dev) != MPU6050_OK){
        return MPU6050_ERR_COMM;
    }

    dev->mpu6050_cfg = *mpu_cfg;

    st = mpu6050_apply_config(dev, &dev->mpu6050_cfg);
    if (st != MPU6050_OK)
        return st;

    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_recovery_init(mpu6050_t* dev){
    if (dev == NULL)
        return MPU6050_ERR;

    MPU60X0_StatusTypeDef st;

    st = mpu6050_signal_path_reset(dev);
    if (st != MPU6050_OK)
        return st;

    return mpu6050_apply_config(dev, &dev->mpu6050_cfg);
}



MPU60X0_StatusTypeDef mpu6050_who_am_i(mpu6050_t* dev, uint8_t* id)
{
    if (dev == NULL || id == NULL)
        return MPU6050_ERR;

    return mpu6050_read_reg(dev, WHO_AM_I, id);
}

MPU60X0_StatusTypeDef mpu6050_get_accel_value(mpu6050_t* dev, imu_vector_t* out)
{
    if (dev == NULL || out == NULL)
        return MPU6050_ERR;

    uint8_t buf[6];

    if (mpu6050_read_bytes(dev, ACCEL_XOUT_H, buf, sizeof(buf)) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    int16_t rawX = (int16_t)((buf[0] << 8) | buf[1]);
    int16_t rawY = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t rawZ = (int16_t)((buf[4] << 8) | buf[5]);

    float sens = mpu6050_accel_sensitivity(dev->mpu6050_cfg.accel_range);

    out->x = (float)rawX / sens;
    out->y = (float)rawY / sens;
    out->z = (float)rawZ / sens;

    dev->data.accel = *out;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_value(mpu6050_t* dev, imu_vector_t* out)
{
    if (dev == NULL || out == NULL)
        return MPU6050_ERR;

    uint8_t buf[6];

    if (mpu6050_read_bytes(dev, GYRO_XOUT_H, buf, sizeof(buf)) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    int16_t rawX = (int16_t)((buf[0] << 8) | buf[1]);
    int16_t rawY = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t rawZ = (int16_t)((buf[4] << 8) | buf[5]);

    float sens = mpu6050_gyro_sensitivity(dev->mpu6050_cfg.gyro_range);

    out->x = (float)rawX / sens;
    out->y = (float)rawY / sens;
    out->z = (float)rawZ / sens;

    dev->data.gyro = *out;
    return MPU6050_OK;
}

/* -------------------- API di configurazione -------------------- */

MPU60X0_StatusTypeDef mpu6050_signal_path_reset(mpu6050_t* dev){
    if (dev == NULL)
        return MPU6050_ERR;

    return mpu6050_write_reg(dev, SIGNAL_PATH_RESET, GYRO_RESET | ACCEL_RESET | TEMP_RESET);
}

MPU60X0_StatusTypeDef mpu6050_set_dlpf(mpu6050_t* dev, uint8_t dlpf_cfg){
    if (dev == NULL)
        return MPU6050_ERR;

    dlpf_cfg &= 0b00000111;   /* solo 3 bit validi */

    uint8_t reg = 0u;
    if (mpu6050_read_reg(dev, CONFIG, &reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    reg &= ~0b00000111;       /* clear DLPF_CFG[2:0] */
    reg |= dlpf_cfg;

    if (mpu6050_write_reg(dev, CONFIG, reg) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    dev->mpu6050_cfg.dlpf_cfg = dlpf_cfg;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_dlpf(mpu6050_t* dev, uint8_t* dlpf_cfg){
    if (dev == NULL || dlpf_cfg == NULL)
        return MPU6050_ERR;

    *dlpf_cfg = dev->mpu6050_cfg.dlpf_cfg;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_set_sample_div(mpu6050_t* dev, uint8_t smplrt_div){
    if (dev == NULL)
        return MPU6050_ERR;

    if (mpu6050_write_reg(dev, SMPRT_DIV, smplrt_div) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    dev->mpu6050_cfg.smplrt_div = smplrt_div;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_sample_div(mpu6050_t* dev, uint8_t* smplrt_div){
    if (dev == NULL || smplrt_div == NULL)
        return MPU6050_ERR;

    *smplrt_div = dev->mpu6050_cfg.smplrt_div;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_set_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t fs){
    if (dev == NULL)
        return MPU6050_ERR;

    if (fs > MPU6050_ACCEL_FS_16G)
        return MPU6050_ERR;

    if (mpu6050_set_accel_range_lowlevel(dev, fs) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    dev->mpu6050_cfg.accel_range = fs;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t* fs){
    if (dev == NULL || fs == NULL)
        return MPU6050_ERR;

    *fs = dev->mpu6050_cfg.accel_range;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_set_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t fs){
    if (dev == NULL)
        return MPU6050_ERR;
    if (fs > MPU6050_GYRO_FS_2000DPS)
        return MPU6050_ERR;

    if (mpu6050_set_gyro_range_lowlevel(dev, fs) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    dev->mpu6050_cfg.gyro_range = fs;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t* fs){
    if (dev == NULL || fs == NULL)
        return MPU6050_ERR;

    *fs = dev->mpu6050_cfg.gyro_range;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_set_interrupt_mask(mpu6050_t* dev, uint8_t int_mask){
    if (dev == NULL)
        return MPU6050_ERR;

    if (mpu6050_write_reg(dev, INT_ENABLE, int_mask) != MPU6050_OK)
        return MPU6050_ERR_COMM;

    dev->mpu6050_cfg.int_enable = int_mask;
    return MPU6050_OK;
}

MPU60X0_StatusTypeDef mpu6050_get_interrupt_mask(mpu6050_t* dev, uint8_t* int_mask){
    if (dev == NULL || int_mask == NULL)
        return MPU6050_ERR;

    *int_mask = dev->mpu6050_cfg.int_enable;
    return MPU6050_OK;
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


