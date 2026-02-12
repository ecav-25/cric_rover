/**
 * @file hw_config.h
 * @author Gruppo 2
 * @brief Definizione della mappatura hardware e delle strutture di configurazione.
 * * Questo file centralizza la configurazione di tutti i componenti hardware del rover:
 * motori, encoder, canali ADC e LED. Definisce le costanti che verranno poi implementate
 * nel file sorgente per collegare gli oggetti software ai relativi timer e pin del microcontrollore.
 */

#ifndef INC_HW_CONFIG_H_
#define INC_HW_CONFIG_H_

#include "motor.h"
#include "encoder.h"
#include "led_stripes.h"
#include "led.h"

/* ===================== MOTORS ===================== */

/**
 * @brief Identificativi univoci dei motori.
 */
typedef enum {
    MOTOR_FA = 0,   /**< Motore Front-A */
    MOTOR_FB,       /**< Motore Front-B */
    MOTOR_BA,       /**< Motore Back-A  */
    MOTOR_BB,       /**< Motore Back-B  */
    MOTOR_COUNT     /**< Numero totale di motori */
} Motor_Id_t;

/**
 * @brief Configurazione hardware per un singolo motore.
 */
typedef struct {
    TIM_HandleTypeDef* htim;        /**< Puntatore al timer PWM */
    uint32_t           channel;     /**< Canale del timer associato */
    Motor_Calibration_t calib;      /**< Parametri di calibrazione (offset e scale) */
} Motor_HW_Config_t;

/**
 * @brief Array di configurazione motori (implementato in hw_config.c).
 */
extern const Motor_HW_Config_t MOTOR_HW_CONFIG[MOTOR_COUNT];

/* ===================== ENCODERS ===================== */

/**
 * @brief Identificativi univoci degli encoder.
 */
typedef enum {
    ENCODER_FA = 0, /**< Encoder Front-A */
    ENCODER_FB,     /**< Encoder Front-B */
    ENCODER_BA,     /**< Encoder Back-A  */
    ENCODER_BB,     /**< Encoder Back-B  */
    ENCODER_COUNT   /**< Numero totale di encoder */
} Encoder_Id_t;

/**
 * @brief Configurazione hardware per un singolo encoder.
 */
typedef struct {
    TIM_HandleTypeDef* htim;        /**< Puntatore al timer in modalità Encoder */
    Encoder_Calibration_t calib;    /**< Caratteristiche meccaniche (PPR e Gear Ratio) */
} Encoder_HW_Config_t;

/**
 * @brief Array di configurazione encoder (implementato in hw_config.c).
 */
extern const Encoder_HW_Config_t ENCODER_HW_CONFIG[ENCODER_COUNT];

/* ===================== ADC ===================== */

/**
 * @brief Identificativi dei canali ADC utilizzati.
 */
typedef enum {
    ADC_TEMP_SENSOR = 0,    /**< Sensore di temperatura interna */
    ADC_BATTERY_VOLTAGE,    /**< Tensione della batteria principale */
    ADC_COUNT               /**< Numero totale di canali ADC */
} ADC_Id_t;

/**
 * @brief Configurazione hardware per le letture analogiche.
 */
typedef struct {
    ADC_HandleTypeDef* hadc;            /**< Handle dell'ADC STM32 */
    ADC_ChannelConfTypeDef channel_cfg; /**< Configurazione specifica del canale */
} ADC_HW_Config_t;

/**
 * @brief Array di configurazione ADC (implementato in hw_config.c).
 */
extern const ADC_HW_Config_t ADC_HW_CONFIG[ADC_COUNT];

/* ===================== LEDS ===================== */

/**
 * @brief Identificativi dei LED di stato onboard.
 */
typedef enum {
    LED_A = 0,      /**< LED primario */
    LED_B,          /**< LED secondario */
    LED_COUNT_HW    /**< Numero totale di LED hardware */
} HW_Led_Id_t;

/**
 * @brief Configurazione hardware per la gestione dei LED.
 */
typedef struct {
    GPIO_TypeDef* port[LED_COUNT];              /**< Porte GPIO associate */
    uint16_t pin[LED_COUNT];                   /**< Pin GPIO associati */
    pin_state_t init_pin_state[LED_COUNT];     /**< Stato iniziale (ON/OFF) */
} HW_Led_Config_t;

/**
 * @brief Array di configurazione LED (implementato in hw_config.c).
 */
extern const HW_Led_Config_t HW_LED_CONFIG[LED_COUNT_HW];

/**
 * @brief Identificativi per le strisce LED WS2812B.
 */
typedef enum {
    LED_STRIPES_MAIN = 0,   /**< Striscia LED principale */
    LED_STRIPES_COUNT       /**< Numero totale di strisce LED */
} LED_Stripes_Id_t;

/**
 * @brief Configurazione per le strisce LED (implementato in hw_config.c).
 */
extern const led_stripes_config_t led_stripes_cfg[LED_STRIPES_COUNT];

#endif /* INC_HW_CONFIG_H_ */
