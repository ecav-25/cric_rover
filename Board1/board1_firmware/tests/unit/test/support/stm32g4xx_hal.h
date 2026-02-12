#ifndef STM32G4xx_HAL_H
#define STM32G4xx_HAL_H

#include <stdint.h>

#ifndef __IO
#define __IO volatile
#endif

/* --- 1. Tipi Base e DMA (Dichiarati per primi per evitare errori unknown type) --- */
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum {
    HAL_DMA_STATE_RESET = 0x00U,
    HAL_DMA_STATE_READY = 0x01U,
    HAL_DMA_STATE_BUSY  = 0x02U
} HAL_DMA_StateTypeDef;

typedef struct {
    HAL_DMA_StateTypeDef State;
} DMA_HandleTypeDef;

/* --- 2. SEZIONE GPIO --- */
typedef struct {
    __IO uint32_t MODER;   
    __IO uint32_t ODR;     
} GPIO_TypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET = 1
} GPIO_PinState;

/* --- 3. SEZIONE ADC --- */
typedef struct { __IO uint32_t SR; __IO uint32_t CR; } ADC_TypeDef;
typedef struct { uint32_t SingleDiff; } ADC_ChannelConfTypeDef;
typedef struct { ADC_TypeDef* Instance; } ADC_HandleTypeDef;

/* --- 4. SEZIONE TIMER (Aggiornata con registri CCR per Motor PWM) --- */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t SMCR;
    __IO uint32_t SR;
    __IO uint32_t CNT;
    __IO uint32_t ARR;
    __IO uint32_t RCR;   /**< Aggiunto per offset corretti */
    __IO uint32_t CCR1;  /**< Necessario per motor_set() */
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
} TIM_TypeDef;

typedef struct {
    TIM_TypeDef* Instance;
} TIM_HandleTypeDef;

/* --- 5. Costanti HAL --- */
#define TIM_CHANNEL_1      0x00000000U
#define TIM_CHANNEL_2      0x00000004U
#define TIM_CHANNEL_ALL    0x00000018U
#define TIM_FLAG_UPDATE    (0x0001U)

/* --- 6. Macro HAL (Aggiunta __HAL_TIM_SET_COMPARE per motor.c) --- */
#define __HAL_TIM_GET_COUNTER(__HANDLE__)          ((__HANDLE__)->Instance->CNT)
#define __HAL_TIM_SET_COUNTER(__HANDLE__, __CNT__) ((__HANDLE__)->Instance->CNT = (__CNT__))
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__)       ((__HANDLE__)->Instance->ARR)
#define __HAL_TIM_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->SR = ~(__FLAG__))
#define __HAL_TIM_IS_TIM_COUNTING_DOWN(__HANDLE__) (((__HANDLE__)->Instance->CR1 & (0x10U)) != 0U)

/** * Macro per impostare il duty cycle del motore. 
 * Calcola l'indirizzo del registro CCR corretto in base al canale.
 */
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
  (*(__IO uint32_t *)(&((__HANDLE__)->Instance->CCR1) + ((__CHANNEL__) >> 2U)) = (__COMPARE__))

/* --- 7. Prototipi per CMock --- */

/* GPIO */
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* ADC */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
uint32_t          HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc, uint32_t SingleDiff);

/* Timer Base (Necessario per Deadline Watchdog) */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim); // Aggiunto Stop_IT

/* Timer PWM (Necessario per motor.c) */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel); // Aggiunto PWM Start
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);  // Aggiunto PWM Stop
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);

/* Timer Encoder */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel); // Aggiunto Encoder Stop

#endif /* STM32G4xx_HAL_H */