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
#include "Board2_types.h"
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
#define MOTOR_BB_Pin GPIO_PIN_2
#define MOTOR_BB_GPIO_Port GPIOC
#define MOTOR_BA_Pin GPIO_PIN_3
#define MOTOR_BA_GPIO_Port GPIOC
#define TRIG_LEFT_Pin GPIO_PIN_0
#define TRIG_LEFT_GPIO_Port GPIOA
#define TRIG_CENTER_Pin GPIO_PIN_1
#define TRIG_CENTER_GPIO_Port GPIOA
#define TRIG_RIGHT_Pin GPIO_PIN_4
#define TRIG_RIGHT_GPIO_Port GPIOA
#define SELECT_Pin GPIO_PIN_6
#define SELECT_GPIO_Port GPIOA
#define RELAY_Pin GPIO_PIN_7
#define RELAY_GPIO_Port GPIOA
#define ECHO_RIGHT_Pin GPIO_PIN_10
#define ECHO_RIGHT_GPIO_Port GPIOB
#define STALK_Pin GPIO_PIN_8
#define STALK_GPIO_Port GPIOC
#define MTALK_Pin GPIO_PIN_9
#define MTALK_GPIO_Port GPIOC
#define MOTOR_FB_Pin GPIO_PIN_8
#define MOTOR_FB_GPIO_Port GPIOA
#define MOTOR_FA_Pin GPIO_PIN_9
#define MOTOR_FA_GPIO_Port GPIOA
#define ECHO_LEFT_Pin GPIO_PIN_15
#define ECHO_LEFT_GPIO_Port GPIOA
#define SESSION_Pin GPIO_PIN_11
#define SESSION_GPIO_Port GPIOC
#define ECHO_CENTER_Pin GPIO_PIN_3
#define ECHO_CENTER_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
