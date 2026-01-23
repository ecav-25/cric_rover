/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Board1_types.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCODER_FA_CH2_Pin GPIO_PIN_2
#define ENCODER_FA_CH2_GPIO_Port GPIOC
#define ENCODER_BA_CH1_Pin GPIO_PIN_0
#define ENCODER_BA_CH1_GPIO_Port GPIOA
#define ENCODER_BA_CH2_Pin GPIO_PIN_1
#define ENCODER_BA_CH2_GPIO_Port GPIOA
#define MOTOR_FB_Pin GPIO_PIN_4
#define MOTOR_FB_GPIO_Port GPIOA
#define MOTOR_FA_Pin GPIO_PIN_6
#define MOTOR_FA_GPIO_Port GPIOA
#define MOTOR_BA_Pin GPIO_PIN_0
#define MOTOR_BA_GPIO_Port GPIOB
#define MOTOR_BB_Pin GPIO_PIN_1
#define MOTOR_BB_GPIO_Port GPIOB
#define ENCODER_FA_CH1_Pin GPIO_PIN_2
#define ENCODER_FA_CH1_GPIO_Port GPIOB
#define ENCODER_FB_CH1_Pin GPIO_PIN_6
#define ENCODER_FB_CH1_GPIO_Port GPIOC
#define ENCODER_FB_CH2_Pin GPIO_PIN_7
#define ENCODER_FB_CH2_GPIO_Port GPIOC
#define MTALK_Pin GPIO_PIN_8
#define MTALK_GPIO_Port GPIOC
#define STALK_Pin GPIO_PIN_9
#define STALK_GPIO_Port GPIOC
#define ENCODER_BB_CH1_Pin GPIO_PIN_8
#define ENCODER_BB_CH1_GPIO_Port GPIOA
#define ENCODER_BB_CH2_Pin GPIO_PIN_9
#define ENCODER_BB_CH2_GPIO_Port GPIOA
#define FB_LED_WHITE_Pin GPIO_PIN_10
#define FB_LED_WHITE_GPIO_Port GPIOA
#define SESSION_Pin GPIO_PIN_11
#define SESSION_GPIO_Port GPIOC
#define SESSION_EXTI_IRQn EXTI15_10_IRQn
#define FB_LED_RED_Pin GPIO_PIN_3
#define FB_LED_RED_GPIO_Port GPIOB
#define FA_LED_WHITE_Pin GPIO_PIN_4
#define FA_LED_WHITE_GPIO_Port GPIOB
#define FA_LED_RED_Pin GPIO_PIN_5
#define FA_LED_RED_GPIO_Port GPIOB
#define REAR_LED_Pin GPIO_PIN_9
#define REAR_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
