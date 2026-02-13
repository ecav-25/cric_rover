// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    mpu_constants.h
 * @author  Gruppo 2
 * @brief   Definizione dei registri e delle costanti per il sensore MPU6050.
 * @details Questo file contiene la mappa completa dei registri (Register Map)
 * dell'MPU6050 e le maschere di bit per la configurazione dei registri principali
 * (Power Management, Interrupt, Sync).
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_MPU_CONSTANTS_H_
#define INC_MPU_CONSTANTS_H_

/**
 * @brief Codici di ritorno per le operazioni sul driver MPU6050.
 */
typedef enum {
    MPU6050_OK = 0,     /**< Operazione completata con successo. */
    MPU6050_ERR,        /**< Errore generico o parametri non validi. */
    MPU6050_ERR_COMM    /**< Errore di comunicazione I2C (HAL Error). */
} MPU60X0_StatusTypeDef;

/* ========================================================================== */
/* GENERAL CONFIGURATION                                                      */
/* ========================================================================== */

/** @brief Indirizzo I2C di default (AD0=0) shiftato per HAL (0x68 << 1). */
#define MPU60X0_ADDRESS 				(0x68U << 1U)

/** @brief Stato di default del registro PWR_MGMT_1 (Bit SLEEP attivo al reset). */
#define PWR_MGMT_1_DEFAULT_STATUS     	64U

/** @brief Configurazione default del Digital Low Pass Filter (DLPF). */
#define MPU6050_DEFAULT_DLPF_CFG       	3U

/** @brief Divisore default per Sample Rate (Sample Rate = GyroRate / (1 + SMPLRT_DIV)). */
#define DEFAULT_SMPLRT_DIV				19U

/** @name Signal Path Reset Bits
 * Bitmask per il registro SIGNAL_PATH_RESET (104).
 * @{ */
#define GYRO_RESET   (1U << 2)   /**< Reset percorso segnale giroscopio. */
#define ACCEL_RESET  (1U << 1)   /**< Reset percorso segnale accelerometro. */
#define TEMP_RESET   (1U << 0)   /**< Reset percorso segnale temperatura. */
/** @} */

/* ========================================================================== */
/* COMMAND CONSTANTS (BITMASKS)                                               */
/* ========================================================================== */

/** @name Power Management 1 Bits
 * Bitmask per il registro PWR_MGMT_1 (107).
 * @{ */
#define DEVICE_RESET         0b10000000  /**< (Bit 7) Resetta tutti i registri interni. */
#define SLEEP                0b01000000  /**< (Bit 6) Mette il dispositivo in sleep mode. */
#define CYCLE                0b00100000  /**< (Bit 5) Wake up ciclico per acquisizione. */
#define TEMP_DIS             0b00001000  /**< (Bit 3) Disabilita il sensore di temperatura. */
#define PWR_MGMT_WAKE		 0b00000000  /**< Valore per svegliare il dispositivo (tutti i bit a 0). */
/** @} */

/** @name External Sync Set (CONFIG Register)
 * Configurazione del pin FSYNC nel registro CONFIG (26).
 * @{ */
#define EXT_SYNC_SET_DISABLED     0b00000000  /**< Input FSYNC disabilitato. */
#define EXT_SYNC_SET_TEMP_OUT_L   0b00001000  /**< FSYNC bit location: TEMP_OUT_L[0]. */
#define EXT_SYNC_SET_GYRO_XOUT_L  0b00010000  /**< FSYNC bit location: GYRO_XOUT_L[0]. */
#define EXT_SYNC_SET_GYRO_YOUT_L  0b00011000  /**< FSYNC bit location: GYRO_YOUT_L[0]. */
#define EXT_SYNC_SET_GYRO_ZOUT_L  0b00100000  /**< FSYNC bit location: GYRO_ZOUT_L[0]. */
#define EXT_SYNC_SET_ACCEL_XOUT_L 0b00101000  /**< FSYNC bit location: ACCEL_XOUT_L[0]. */
#define EXT_SYNC_SET_ACCEL_YOUT_L 0b00110000  /**< FSYNC bit location: ACCEL_YOUT_L[0]. */
#define EXT_SYNC_SET_ACCEL_ZOUT_L 0b00111000  /**< FSYNC bit location: ACCEL_ZOUT_L[0]. */
/** @} */

/** @name Interrupt Enable Bits
 * Bitmask per il registro INT_ENABLE (56).
 * @{ */
#define MPU6050_INT_NONE          0b00000000  /**< Nessun interrupt abilitato. */
#define MPU6050_INT_DATA_RDY      0b00000001  /**< (Bit 0) Interrupt Data Ready. */
#define MPU6050_INT_I2C_MST       0b00001000  /**< (Bit 3) Interrupt I2C Master. */
#define MPU6050_INT_FIFO_OFLOW    0b00010000  /**< (Bit 4) Interrupt FIFO Overflow. */
/** @} */

/* ========================================================================== */
/* REGISTER MAP                                                               */
/* ========================================================================== */

/** @name Self Test Registers
 * @{ */
#define SELF_TEST_X                   13U
#define SELF_TEST_Y                   14U
#define SELF_TEST_Z                   15U
#define SELF_TEST_A                   16U
/** @} */

/** @name Configuration Registers
 * @{ */
#define SMPRT_DIV                     25U  /**< Sample Rate Divider. */
#define CONFIG                        26U  /**< Configuration (DLPF, Ext Sync). */
#define GYRO_CONFIG                   27U  /**< Gyroscope Configuration (Scale). */
#define ACCEL_CONFIG                  28U  /**< Accelerometer Configuration (Scale). */
/** @} */

/** @name FIFO & I2C Master Control
 * @{ */
#define FIFO_EN                       35U
#define I2C_MST_CTRL                  36U
#define I2C_MST_STATUS                54U
#define I2C_MST_DELAY_CTRL            103U
/** @} */

/** @name I2C Slave Registers (0-4)
 * @{ */
#define I2C_SLV0_ADDR                 37U
#define I2C_SLV0_REG                  38U
#define I2C_SLV0_CTRL                 39U
#define I2C_SLV0_DO                   99U

#define I2C_SLV1_ADDR                 40U
#define I2C_SLV1_REG                  41U
#define I2C_SLV1_CTRL                 42U
#define I2C_SLV1_DO                   100U

#define I2C_SLV2_ADDR                 43U
#define I2C_SLV2_REG                  44U
#define I2C_SLV2_CTRL                 45U
#define I2C_SLV2_DO                   101U

#define I2C_SLV3_ADDR                 46U
#define I2C_SLV3_REG                  47U
#define I2C_SLV3_CTRL                 48U
#define I2C_SLV3_DO                   102U

#define I2C_SLV4_ADDR                 49U
#define I2C_SLV4_REG                  50U
#define I2C_SLV4_DO                   51U
#define I2C_SLV4_CTRL                 52U
#define I2C_SLV4_DI                   53U
/** @} */

/** @name Interrupt Registers
 * @{ */
#define INT_PIN_CFG                   55U
#define INT_ENABLE                    56U
#define INT_STATUS                    58U
/** @} */

/** @name Data Registers (Read Only)
 * @{ */
#define ACCEL_XOUT_H                  59U
#define ACCEL_XOUT_L                  60U
#define ACCEL_YOUT_H                  61U
#define ACCEL_YOUT_L                  62U
#define ACCEL_ZOUT_H                  63U
#define ACCEL_ZOUT_L                  64U
#define TEMP_OUT_H                    65U
#define TEMP_OUT_L                    66U
#define GYRO_XOUT_H                   67U
#define GYRO_XOUT_L                   68U
#define GYRO_YOUT_H                   69U
#define GYRO_YOUT_L                   70U
#define GYRO_ZOUT_H                   71U
#define GYRO_ZOUT_L                   72U
/** @} */

/** @name External Sensor Data
 * @{ */
#define EXT_SENS_DATA_00              73U
#define EXT_SENS_DATA_01              74U
#define EXT_SENS_DATA_02              75U
#define EXT_SENS_DATA_03              76U
#define EXT_SENS_DATA_04              77U
/** @} */

/** @name User Control & Power Management
 * @{ */
#define SIGNAL_PATH_RESET             104U
#define USER_CTRL                     106U
#define PWR_MGMT_1                    107U
#define PWR_MGMT_2                    108U
/** @} */

/** @name FIFO Access
 * @{ */
#define FIFO_COUNTH                   114U
#define FIFO_COUNTL                   115U
#define FIFO_R_W                      116U
/** @} */

/** @brief Device ID Register (Should be 0x68). */
#define WHO_AM_I                      117U

#endif /* INC_MPU_CONSTANTS_H_ */
