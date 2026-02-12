#ifndef STM32G4xx_HAL_H
#define STM32G4xx_HAL_H

#include <stdint.h>

#ifndef __IO
#define __IO volatile
#endif

/* --- 1. TIPI BASE E STATI --- */
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

/* --- 4. SEZIONE TIMER (TIM) --- */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t SMCR;
    __IO uint32_t SR;
    __IO uint32_t CNT;
    __IO uint32_t ARR;
    __IO uint32_t RCR;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
} TIM_TypeDef;

typedef enum {
    HAL_TIM_ACTIVE_CHANNEL_1 = 0x01U,
    HAL_TIM_ACTIVE_CHANNEL_2 = 0x02U,
    HAL_TIM_ACTIVE_CHANNEL_3 = 0x04U,
    HAL_TIM_ACTIVE_CHANNEL_4 = 0x08U,
    HAL_TIM_ACTIVE_CHANNEL_CLEARED = 0x00U
} HAL_TIM_ActiveChannel;

typedef struct {
    TIM_TypeDef* Instance;
    HAL_TIM_ActiveChannel Channel; /**< Usata per callback IC */
} TIM_HandleTypeDef;

/* Costanti Timer */
#define TIM_CHANNEL_1      0x00000000U
#define TIM_CHANNEL_2      0x00000004U
#define TIM_CHANNEL_3      0x00000008U
#define TIM_CHANNEL_4      0x0000000CU
#define TIM_CHANNEL_ALL    0x00000018U
#define TIM_FLAG_UPDATE    (0x0001U)

#define TIM_INPUTCHANNELPOLARITY_RISING  0x00000000U
#define TIM_INPUTCHANNELPOLARITY_FALLING 0x00000001U

/* Macro Timer */
#define __HAL_TIM_GET_COUNTER(__HANDLE__)          ((__HANDLE__)->Instance->CNT)
#define __HAL_TIM_SET_COUNTER(__HANDLE__, __CNT__) ((__HANDLE__)->Instance->CNT = (__CNT__))
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__)       ((__HANDLE__)->Instance->ARR)
#define __HAL_TIM_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->SR = ~(__FLAG__))
#define __HAL_TIM_IS_TIM_COUNTING_DOWN(__HANDLE__) (((__HANDLE__)->Instance->CR1 & (0x10U)) != 0U)

#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
  (*(__IO uint32_t *)(&((__HANDLE__)->Instance->CCR1) + ((__CHANNEL__) >> 2U)) = (__COMPARE__))

#define __HAL_TIM_SET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__, __POLARITY__) // Mocked for test

/* --- 5. SEZIONE I2C --- */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t ISR;
    __IO uint32_t RXDR;
    __IO uint32_t TXDR;
} I2C_TypeDef;

typedef struct {
    I2C_TypeDef* Instance;
} I2C_HandleTypeDef;

/* --- 6. SEZIONE ARM CORE (DWT & CoreDebug) --- */
typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t CYCCNT;
} DWT_Type;

typedef struct {
    __IO uint32_t DEMCR;
} CoreDebug_Type;

#define CoreDebug_DEMCR_TRCENA_Msk (1UL << 24)
#define DWT_CTRL_CYCCNTENA_Msk     (1UL << 0)

#ifdef TEST
    extern DWT_Type       *DWT;
    extern CoreDebug_Type *CoreDebug;
#else
    #define DWT         ((DWT_Type *) 0xE0001000)
    #define CoreDebug   ((CoreDebug_Type *) 0xE000EDFC)
#endif

/* --- 7. PROTOTIPI PER CMOCK --- */

/* GPIO */
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* ADC */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
uint32_t          HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);

/* I2C */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/* TIM Base & PWM */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

/* TIM Input Capture (HC-SR04) */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
uint32_t          HAL_TIM_ReadCapturedValue(TIM_HandleTypeDef *htim, uint32_t Channel);

/* System */
extern uint32_t SystemCoreClock;
void __NOP(void);

#define I2C_MEMADD_SIZE_8BIT 0x00000001U

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void HAL_Delay(uint32_t Delay);

#endif /* STM32G4xx_HAL_H */