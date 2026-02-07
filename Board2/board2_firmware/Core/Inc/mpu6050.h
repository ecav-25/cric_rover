#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stdint.h"
#include "i2c.h"
#include "mpu_constants.h"

/* -------------------- Tipi di supporto -------------------- */

typedef struct {
    float x;
    float y;
    float z;
} imu_vector_t;

typedef struct {
    imu_vector_t accel;
    imu_vector_t gyro;
} mpu6050_data_t;

/* Full-scale ranges accelerometro (AFS_SEL) */
typedef enum {
    MPU6050_ACCEL_FS_2G  = 0,
    MPU6050_ACCEL_FS_4G  = 1,
    MPU6050_ACCEL_FS_8G  = 2,
    MPU6050_ACCEL_FS_16G = 3
} mpu6050_accel_fs_t;

/* Full-scale ranges giroscopio (FS_SEL) */
typedef enum {
    MPU6050_GYRO_FS_250DPS  = 0,
    MPU6050_GYRO_FS_500DPS  = 1,
    MPU6050_GYRO_FS_1000DPS = 2,
    MPU6050_GYRO_FS_2000DPS = 3
} mpu6050_gyro_fs_t;

/* -------------------- Configurazione interna -------------------- */
typedef struct {
    uint8_t            dlpf_cfg;     /* CONFIG.DLPF_CFG[2:0]             */
    uint8_t            smplrt_div;   /* SMPRT_DIV                        */
    mpu6050_accel_fs_t accel_range;  /* ACCEL_CONFIG.AFS_SEL[4:3]        */
    mpu6050_gyro_fs_t  gyro_range;   /* GYRO_CONFIG.FS_SEL[4:3]          */
    uint8_t            int_enable;   /* INT_ENABLE                       */
} mpu6050_config_t;

/* Descrittore dispositivo */
typedef struct {
    I2C_HandleTypeDef* i2c;
    uint16_t           address;
    mpu6050_config_t   mpu6050_cfg;
    mpu6050_data_t     data;
} mpu6050_t;

/* -------------------- API di base -------------------- */

/* Inizializza il dispositivo (reset, wake-up, applica config corrente) */
MPU60X0_StatusTypeDef mpu6050_init(mpu6050_t* dev, I2C_HandleTypeDef* i2c, uint16_t address, const mpu6050_config_t* mpu_cfg);

MPU60X0_StatusTypeDef mpu6050_recovery_init(mpu6050_t* dev);

/* Legge WHO_AM_I (dovrebbe valere 0x68) */
MPU60X0_StatusTypeDef mpu6050_who_am_i(mpu6050_t* dev, uint8_t* id);

/* Legge accelerazione (valori in g) */
MPU60X0_StatusTypeDef mpu6050_get_accel_value(mpu6050_t* dev, imu_vector_t* out);

/* Legge velocità angolare (valori in °/s) */
MPU60X0_StatusTypeDef mpu6050_get_gyro_value(mpu6050_t* dev, imu_vector_t* out);

/* -------------------- API di configurazione -------------------- */

MPU60X0_StatusTypeDef mpu6050_signal_path_reset(mpu6050_t* dev);
/* DLPF_CFG (CONFIG[2:0]) */
MPU60X0_StatusTypeDef mpu6050_set_dlpf(mpu6050_t* dev, uint8_t dlpf_cfg);
MPU60X0_StatusTypeDef mpu6050_get_dlpf(mpu6050_t* dev, uint8_t* dlpf_cfg);

/* Sample Rate Divider (SMPRT_DIV) */
MPU60X0_StatusTypeDef mpu6050_set_sample_div(mpu6050_t* dev, uint8_t smplrt_div);
MPU60X0_StatusTypeDef mpu6050_get_sample_div(mpu6050_t* dev, uint8_t* smplrt_div);

/* Full-scale accelerometro (AFS_SEL, enum pubblico) */
MPU60X0_StatusTypeDef mpu6050_set_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t fs);
MPU60X0_StatusTypeDef mpu6050_get_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t* fs);

/* Full-scale giroscopio (FS_SEL, enum pubblico) */
MPU60X0_StatusTypeDef mpu6050_set_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t fs);
MPU60X0_StatusTypeDef mpu6050_get_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t* fs);

/* Maschera interrupt (INT_ENABLE) */
MPU60X0_StatusTypeDef mpu6050_set_interrupt_mask(mpu6050_t* dev, uint8_t int_mask);
MPU60X0_StatusTypeDef mpu6050_get_interrupt_mask(mpu6050_t* dev, uint8_t* int_mask);

#endif /* INC_MPU6050_H_ */
