// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    hw_config.h
 * @author  Gruppo 2
 * @brief   Definizioni e tipi per la configurazione hardware astratta.
 * @details Questo file espone le strutture dati e gli enumeratori utilizzati per
 * mappare le risorse hardware (Timer, I2C, GPIO) ai driver logici del sistema
 * (Motori, Sensori, Controller). Le istanze reali sono definite in hw_config.c.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

#include "motor.h"
#include "tim.h"
#include "mpu6050.h"
#include "i2c.h"
#include "hcsr04.h"

/* ========================================================================== */
/* MOTORS CONFIGURATION                                                       */
/* ========================================================================== */

/**
 * @brief Identificativi logici dei motori del veicolo.
 */
typedef enum {
    MOTOR_FA = 0,   /**< Motore Anteriore A (Front-A). */
    MOTOR_FB,       /**< Motore Anteriore B (Front-B). */
    MOTOR_BA,       /**< Motore Posteriore A (Back-A). */
    MOTOR_BB,       /**< Motore Posteriore B (Back-B). */
    MOTOR_COUNT     /**< Numero totale di motori. */
} Motor_Id_t;

/**
 * @brief Struttura di collegamento hardware per un singolo motore.
 * @details Associa il driver logico alle risorse fisiche STM32.
 */
typedef struct {
    TIM_HandleTypeDef* htim;   /**< Handle del timer hardware PWM. */
    uint32_t channel;          /**< Canale del timer PWM. */
    Motor_Calibration_t calib; /**< Parametri di calibrazione (punti neutri e scaling). */
} Motor_HW_Config_t;

/**
 * @brief Tabella di configurazione esterna per i motori.
 * @note Definita in hw_config.c.
 */
extern const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT];

/* ========================================================================== */
/* MPU6050 CONFIGURATION                                                      */
/* ========================================================================== */

/**
 * @brief Identificativi per le unità IMU (Inertial Measurement Unit).
 */
typedef enum {
    MPU_MAIN = 0,   /**< IMU Principale. */
    MPU_COUNT       /**< Numero totale di IMU. */
} MPU_Id_t;

/**
 * @brief Struttura di configurazione hardware per MPU6050.
 */
typedef struct {
    I2C_HandleTypeDef* i2c; /**< Handle del bus I2C. */
    uint16_t address;       /**< Indirizzo I2C del dispositivo (es. 0xD0). */
    mpu6050_config_t cfg;   /**< Configurazione interna del sensore (FSR, DLPF). */
} MPU_HW_Config_t;

/**
 * @brief Tabella di configurazione esterna per le IMU.
 */
extern const MPU_HW_Config_t MPU_HW_CONFIG[MPU_COUNT];


/* ========================================================================== */
/* ULTRASONIC SENSORS CONFIGURATION                                           */
/* ========================================================================== */

/**
 * @brief Identificativi logici per i sensori a ultrasuoni.
 */
typedef enum {
	US_LEFT = 0,    /**< Sensore Sinistro. */
	US_CENTER,      /**< Sensore Centrale. */
	US_RIGHT,       /**< Sensore Destro. */
	US_COUNT        /**< Numero totale di sensori US. */
} UltrasonicSensor_Id_t;


/**
 * @brief Tabella di configurazione esterna per i sensori a ultrasuoni.
 * @details Utilizza la struttura hcsr04_cfg_t definita nel driver hcsr04.h.
 */
extern const hcsr04_cfg_t ULTRASONIC_HW_CONFIG[US_COUNT];


/* ========================================================================== */
/* CONTROLLER CONFIGURATION                                                   */
/* ========================================================================== */

/**
 * @brief Identificativi per i controller remoti.
 */
typedef enum {
	CONTROLLER_MAIN = 0, /**< Controller Principale. */
	CONTROLLER_COUNT     /**< Numero totale di controller supportati. */
} Controller_Id_t;

/**
 * @brief Struttura di configurazione hardware per il controller remoto.
 */
typedef struct {
	I2C_HandleTypeDef* i2c; /**< Handle del bus I2C di comunicazione. */
	uint16_t address;       /**< Indirizzo I2C del controller remoto. */
} Controller_HW_Config_t;

/**
 * @brief Tabella di configurazione esterna per i controller.
 */
extern const Controller_HW_Config_t CONTROLLER_HW_CONFIG[CONTROLLER_COUNT];

#endif /* INC_HW_CONFIG_H_ */
