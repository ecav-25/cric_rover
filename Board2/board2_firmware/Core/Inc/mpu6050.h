// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    mpu6050.h
 * @author  Gruppo 2
 * @brief   Driver per il sensore inerziale (IMU) MPU6050 (6-DOF).
 * @details Questo driver gestisce la configurazione e la lettura dei dati dal
 * sensore MPU6050 tramite bus I2C
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stdint.h"
#include "i2c.h"
#include "mpu_constants.h"

/* ========================================================================== */
/* DATA TYPES                                                                 */
/* ========================================================================== */

/**
 * @brief Struttura generica per un vettore a 3 assi (float).
 */
typedef struct {
    float x; /**< Componente X. */
    float y; /**< Componente Y. */
    float z; /**< Componente Z. */
} imu_vector_t;

/**
 * @brief Contenitore dati completi (Accelerometro + Giroscopio).
 */
typedef struct {
    imu_vector_t accel; /**< Dati accelerometro convertiti [g]. */
    imu_vector_t gyro;  /**< Dati giroscopio convertiti [°/s]. */
} mpu6050_data_t;

/**
 * @brief Fondo scala (Full Scale Range) dell'accelerometro.
 * @details Imposta la sensibilità del sensore (LSB/g).
 */
typedef enum {
    MPU6050_ACCEL_FS_2G  = 0, /**< Range ±2g (16384 LSB/g). */
    MPU6050_ACCEL_FS_4G  = 1, /**< Range ±4g (8192 LSB/g). */
    MPU6050_ACCEL_FS_8G  = 2, /**< Range ±8g (4096 LSB/g). */
    MPU6050_ACCEL_FS_16G = 3  /**< Range ±16g (2048 LSB/g). */
} mpu6050_accel_fs_t;

/**
 * @brief Fondo scala (Full Scale Range) del giroscopio.
 * @details Imposta la sensibilità del sensore (LSB/°/s).
 */
typedef enum {
    MPU6050_GYRO_FS_250DPS  = 0, /**< Range ±250 °/s (131 LSB/°/s). */
    MPU6050_GYRO_FS_500DPS  = 1, /**< Range ±500 °/s (65.5 LSB/°/s). */
    MPU6050_GYRO_FS_1000DPS = 2, /**< Range ±1000 °/s (32.8 LSB/°/s). */
    MPU6050_GYRO_FS_2000DPS = 3  /**< Range ±2000 °/s (16.4 LSB/°/s). */
} mpu6050_gyro_fs_t;

/* ========================================================================== */
/* DEVICE CONFIGURATION                                                       */
/* ========================================================================== */

/**
 * @brief Struttura di configurazione iniziale del dispositivo.
 */
typedef struct {
    uint8_t            dlpf_cfg;     /**< Configurazione Digital Low Pass Filter (0-6). */
    uint8_t            smplrt_div;   /**< Sample Rate Divider: SampleRate = GyroRate / (1 + SMPLRT_DIV). */
    mpu6050_accel_fs_t accel_range;  /**< Fondo scala accelerometro desiderato. */
    mpu6050_gyro_fs_t  gyro_range;   /**< Fondo scala giroscopio desiderato. */
    uint8_t            int_enable;   /**< Maschera abilitazione interrupt (es. Data Ready). */
} mpu6050_config_t;

/**
 * @brief Handle principale dell'istanza MPU6050.
 */
typedef struct {
    I2C_HandleTypeDef* i2c;          /**< Handle I2C hardware utilizzato. */
    uint16_t           address;      /**< Indirizzo I2C del dispositivo (es. 0x68 << 1). */
    mpu6050_config_t   mpu6050_cfg;  /**< Copia locale della configurazione attiva. */
    mpu6050_data_t     data;         /**< Ultimi dati acquisiti. */
} mpu6050_t;

/* ========================================================================== */
/* CORE API                                                                   */
/* ========================================================================== */

/**
 * @brief Inizializza il dispositivo MPU6050.
 * @details Esegue la sequenza di startup: reset del dispositivo, wake-up dallo sleep mode
 * e applicazione della configurazione passata come parametro.
 * @param dev Puntatore all'handle del dispositivo.
 * @param i2c Handle I2C hardware.
 * @param address Indirizzo I2C del sensore.
 * @param mpu_cfg Puntatore alla configurazione iniziale.
 * @return MPU60X0_StatusTypeDef Esito dell'inizializzazione.
 */
MPU60X0_StatusTypeDef mpu6050_init(mpu6050_t* dev, I2C_HandleTypeDef* i2c, uint16_t address, const mpu6050_config_t* mpu_cfg);

/**
 * @brief Esegue una procedura di recovery/reset del sensore.
 * @details Utile in caso di perdita di comunicazione o stato inconsistente.
 * @param dev Puntatore all'handle del dispositivo.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_recovery_init(mpu6050_t* dev);

/**
 * @brief Legge il registro WHO_AM_I per verificare l'identità del sensore.
 * @param dev Puntatore all'handle del dispositivo.
 * @param id Puntatore dove salvare l'ID letto (valore atteso: 0x68).
 * @return MPU60X0_StatusTypeDef MPU6050_OK se la lettura ha successo.
 */
MPU60X0_StatusTypeDef mpu6050_who_am_i(mpu6050_t* dev, uint8_t* id);

/**
 * @brief Legge i dati dell'accelerometro dai registri.
 * @details I valori grezzi vengono convertiti in unità **g** (gravità) in base al Full Scale configurato.
 * @param dev Puntatore all'handle del dispositivo.
 * @param out Puntatore alla struttura vettore dove salvare i dati X, Y, Z.
 * @return MPU60X0_StatusTypeDef Esito della lettura.
 */
MPU60X0_StatusTypeDef mpu6050_get_accel_value(mpu6050_t* dev, imu_vector_t* out);

/**
 * @brief Legge i dati del giroscopio dai registri.
 * @details I valori grezzi vengono convertiti in **gradi al secondo (°/s)** in base al Full Scale configurato.
 * @param dev Puntatore all'handle del dispositivo.
 * @param out Puntatore alla struttura vettore dove salvare i dati X, Y, Z.
 * @return MPU60X0_StatusTypeDef Esito della lettura.
 */
MPU60X0_StatusTypeDef mpu6050_get_gyro_value(mpu6050_t* dev, imu_vector_t* out);

/* ========================================================================== */
/* CONFIGURATION API                                                          */
/* ========================================================================== */

/**
 * @brief Resetta i percorsi di segnale interni (Signal Path Reset).
 * @details Resetta i filtri digitali e i registri dati di Accel, Gyro e Temp.
 * @param dev Puntatore all'handle del dispositivo.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_signal_path_reset(mpu6050_t* dev);

/**
 * @brief Imposta la configurazione del filtro passa-basso digitale (DLPF).
 * @param dev Puntatore all'handle del dispositivo.
 * @param dlpf_cfg Valore da 0 (260Hz) a 6 (5Hz).
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_set_dlpf(mpu6050_t* dev, uint8_t dlpf_cfg);

/**
 * @brief Legge la configurazione attuale del DLPF.
 * @param dev Puntatore all'handle del dispositivo.
 * @param dlpf_cfg Puntatore dove salvare il valore letto.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_get_dlpf(mpu6050_t* dev, uint8_t* dlpf_cfg);

/**
 * @brief Imposta il divisore della frequenza di campionamento (SMPRT_DIV).
 * @note Sample Rate = Gyroscope Output Rate / (1 + smplrt_div).
 * @param dev Puntatore all'handle del dispositivo.
 * @param smplrt_div Valore del divisore.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_set_sample_div(mpu6050_t* dev, uint8_t smplrt_div);

/**
 * @brief Legge il divisore della frequenza di campionamento attuale.
 * @param dev Puntatore all'handle del dispositivo.
 * @param smplrt_div Puntatore dove salvare il valore letto.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_get_sample_div(mpu6050_t* dev, uint8_t* smplrt_div);

/**
 * @brief Imposta il fondo scala dell'accelerometro.
 * @param dev Puntatore all'handle del dispositivo.
 * @param fs Valore enum del range desiderato (2G, 4G, 8G, 16G).
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_set_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t fs);

/**
 * @brief Legge il fondo scala attuale dell'accelerometro.
 * @param dev Puntatore all'handle del dispositivo.
 * @param fs Puntatore dove salvare il valore letto.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_get_accel_fs(mpu6050_t* dev, mpu6050_accel_fs_t* fs);

/**
 * @brief Imposta il fondo scala del giroscopio.
 * @param dev Puntatore all'handle del dispositivo.
 * @param fs Valore enum del range desiderato (250, 500, 1000, 2000 DPS).
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_set_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t fs);

/**
 * @brief Legge il fondo scala attuale del giroscopio.
 * @param dev Puntatore all'handle del dispositivo.
 * @param fs Puntatore dove salvare il valore letto.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_get_gyro_fs(mpu6050_t* dev, mpu6050_gyro_fs_t* fs);

/**
 * @brief Imposta la maschera degli interrupt (registro INT_ENABLE).
 * @param dev Puntatore all'handle del dispositivo.
 * @param int_mask Bitmask delle sorgenti interrupt da abilitare.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_set_interrupt_mask(mpu6050_t* dev, uint8_t int_mask);

/**
 * @brief Legge la maschera degli interrupt configurata.
 * @param dev Puntatore all'handle del dispositivo.
 * @param int_mask Puntatore dove salvare la maschera letta.
 * @return MPU60X0_StatusTypeDef Esito dell'operazione.
 */
MPU60X0_StatusTypeDef mpu6050_get_interrupt_mask(mpu6050_t* dev, uint8_t* int_mask);

/* ========================================================================== */
/* SINGLE AXIS GETTERS                                                        */
/* ========================================================================== */

/** @brief Legge accelerazione asse X [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_x(mpu6050_t* dev, float* ax);
/** @brief Legge accelerazione asse Y [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_y(mpu6050_t* dev, float* ay);
/** @brief Legge accelerazione asse Z [g]. */
MPU60X0_StatusTypeDef mpu6050_get_accel_z(mpu6050_t* dev, float* az);

/** @brief Legge velocità angolare asse X [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_x(mpu6050_t* dev, float* gx);
/** @brief Legge velocità angolare asse Y [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_y(mpu6050_t* dev, float* gy);
/** @brief Legge velocità angolare asse Z [°/s]. */
MPU60X0_StatusTypeDef mpu6050_get_gyro_z(mpu6050_t* dev, float* gz);

#endif /* INC_MPU6050_H_ */
