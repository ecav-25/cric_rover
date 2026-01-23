/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Board1.h"
#include "batt.h"
#include "temp.h"
#include "encoder.h"
#include "pid.h"
#include "motor.h"
#include "stdlib.h"
#include "led.h"
#include "led_stripes.h"
#include "deadline_watchdog.h"
#include "motor_config.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DWD_FLAG_SUPERVISION		(1U << 0)
#define DWD_FLAG_READ_SENSORS		(1U << 1)
#define DWD_FLAG_PID				(1U << 2)
#define DWD_FLAG_LIGHT				(1U << 3)

#define SYSTEM_ALIVE_MASK (DWD_FLAG_SUPERVISION | DWD_FLAG_READ_SENSORS | DWD_FLAG_PID | DWD_FLAG_LIGHT )

#define EVT_SESSION   			(1U << 0)

#define PID_PERIOD				(5)
#define LIGHTS_PERIOD			(50)
#define SUPERVISION_PERIOD	    (60)

#define RAMP_SLOPE_DEFAULT_RPM_S   (600.0f)
#define RAMP_SLOPE_SPORT_RPM_S     (1000.0f)
#define RAMP_SLOPE_ECO_RPM_S       (300.0f)

#define RAMP_STEP_DEFAULT (RAMP_SLOPE_DEFAULT_RPM_S * (PID_PERIOD / 1000.0f))
#define RAMP_STEP_SPORT   (RAMP_SLOPE_SPORT_RPM_S   * (PID_PERIOD / 1000.0f))
#define RAMP_STEP_ECO     (RAMP_SLOPE_ECO_RPM_S     * (PID_PERIOD / 1000.0f))

#define NORMAL_BRK_COEFF		(0.5)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static inline float ramp(float current, float target, float step){
    if (current < target - step)
        return current + step;
    else if (current > target + step)
        return current - step;
    else
        return target;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
Deadline_Watchdog_t hard_rt_deadline_wd;

volatile real32_T debug_temperature;
volatile real32_T debug_battery_voltage;

EncoderHandle encoder_FA;
EncoderHandle encoder_FB;
EncoderHandle encoder_BA;
EncoderHandle encoder_BB;
EncoderHandle encoder_FA_pid;
EncoderHandle encoder_FB_pid;
EncoderHandle encoder_BA_pid;
EncoderHandle encoder_BB_pid;

int16_T velocity_FA = 0;
int16_T velocity_FB = 0;
int16_T velocity_BA = 0;
int16_T velocity_BB = 0;

Motor_t motor_FA;
Motor_t motor_FB;
Motor_t motor_BA;
Motor_t motor_BB;

float control_BB = 0;
float control_BA = 0;
float control_FB = 0;
float control_FA = 0;

uint8_t out_BB = 0;
uint8_t out_BA = 0;
uint8_t out_FB = 0;
uint8_t out_FA = 0;

PID_t pid_FA;
PID_t pid_FB;
PID_t pid_BA;
PID_t pid_BB;

real32_T a1 = 1;
real32_T b0_FA = 0.011526, b0_FB = 0.012068, b0_BA = 0.01182, b0_BB = 0.011206;
real32_T b1_FA = -0.0062, b1_FB = -0.0066, b1_BA = -0.0062, b1_BB = -0.0059;

DecBus debug_output;
volatile uint32_t debug_count_step = 0;
volatile uint32_t debug_read_sensor = 0;
volatile uint32_t debug_pid = 0;
volatile uint8_t debug_state_degraded = 0;
volatile uint8_t debug_state = 0;
volatile uint8_t debug_state_actions = 0;
volatile uint32_t debug_rts = 0;
uint32_t debug_time = 0, debug_diff = 0;
uint32_t degraded=0;
uint8_t state_good=0;
DecBus debug_decision;
boolean_T retransmit_seen_in_cycle = false;
uint32_t count_retransmit=0;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim20;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_tim17_ch1;

boolean_T deadline = 0;

led_t ledA;
led_t ledB;

LED_TYPE led_FA = OFF, led_FB = OFF, rear_sign = OFF;
REAR_LED_TYPE rear_led = IDLE;

/* Array di porte GPIO */
GPIO_TypeDef* ledA_ports[LED_COUNT] = {
    FA_LED_RED_GPIO_Port,   	// LED_RED
	FA_LED_WHITE_GPIO_Port    	// LED_WHITE
};

/* Array di pin GPIO */
uint16_t ledA_pins[LED_COUNT] = {
    FA_LED_RED_Pin,   	// LED_RED
	FA_LED_WHITE_Pin    // LED_WHITE
};


GPIO_TypeDef* ledB_ports[LED_COUNT] = {
	FB_LED_RED_GPIO_Port,   	// LED_RED
	FB_LED_WHITE_GPIO_Port    	// LED_WHITE
};

/* Array di pin GPIO */
uint16_t ledB_pins[LED_COUNT] = {
	FB_LED_RED_Pin,   	// LED_RED
	FB_LED_WHITE_Pin    // LED_WHITE
};

/* Stato iniziale dei pin */
pin_state_t led_init_state[LED_COUNT] = {
    GPIO_PIN_RESET,  // LED_RED spento
    GPIO_PIN_RESET   // LED_WHITE spento
};

led_config_t cfg = {
		  .htim = &htim17,
		  .hdma = &hdma_tim17_ch1,
		  .pwm_hi = 135,
		  .pwm_lo = 55,
		  .reset_halves = 2,
		  .scale_b = 0xF0,
		  .scale_g = 0xB0,
		  .scale_r = 0xFF,
		  .tim_channel = TIM_CHANNEL_1
};

/* USER CODE END Variables */
/* Definitions for supervision */
osThreadId_t supervisionHandle;
uint32_t supervisionBuffer[ 512 ];
osStaticThreadDef_t supervisionControlBlock;
const osThreadAttr_t supervision_attributes = {
  .name = "supervision",
  .stack_mem = &supervisionBuffer[0],
  .stack_size = sizeof(supervisionBuffer),
  .cb_mem = &supervisionControlBlock,
  .cb_size = sizeof(supervisionControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for readSensors */
osThreadId_t readSensorsHandle;
uint32_t readSensorsBuffer[ 256 ];
osStaticThreadDef_t readSensorsControlBlock;
const osThreadAttr_t readSensors_attributes = {
  .name = "readSensors",
  .stack_mem = &readSensorsBuffer[0],
  .stack_size = sizeof(readSensorsBuffer),
  .cb_mem = &readSensorsControlBlock,
  .cb_size = sizeof(readSensorsControlBlock),
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for pid */
osThreadId_t pidHandle;
uint32_t pidBuffer[ 384 ];
osStaticThreadDef_t pidControlBlock;
const osThreadAttr_t pid_attributes = {
  .name = "pid",
  .stack_mem = &pidBuffer[0],
  .stack_size = sizeof(pidBuffer),
  .cb_mem = &pidControlBlock,
  .cb_size = sizeof(pidControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for lights */
osThreadId_t lightsHandle;
uint32_t lightsBuffer[ 128 ];
osStaticThreadDef_t lightsControlBlock;
const osThreadAttr_t lights_attributes = {
  .name = "lights",
  .stack_mem = &lightsBuffer[0],
  .stack_size = sizeof(lightsBuffer),
  .cb_mem = &lightsControlBlock,
  .cb_size = sizeof(lightsControlBlock),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for SessionEvent */
osEventFlagsId_t SessionEventHandle;
osStaticEventGroupDef_t SessionEventControlBlock;
const osEventFlagsAttr_t SessionEvent_attributes = {
  .name = "SessionEvent",
  .cb_mem = &SessionEventControlBlock,
  .cb_size = sizeof(SessionEventControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void executeSupervision();
void deadlineProcedure();
/* USER CODE END FunctionPrototypes */

void supervisionTask(void *argument);
void readSensorsTask(void *argument);
void pidTask(void *argument);
void lightsTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	DWD_Init(&hard_rt_deadline_wd, &htim4, SYSTEM_ALIVE_MASK, deadlineProcedure);
	BATT_init(&hadc1);
	TEMP_init(&hadc1);

	encoder_init(&encoder_FA, &htim5, 0, 0, SUPERVISION_PERIOD);
	encoder_init(&encoder_FB, &htim20, 0, 0, SUPERVISION_PERIOD);
	encoder_init(&encoder_BA, &htim1, 0, 0, SUPERVISION_PERIOD);
	encoder_init(&encoder_BB, &htim8, 0, 0, SUPERVISION_PERIOD);

	encoder_init(&encoder_FA_pid, &htim5, 0, 0, PID_PERIOD);
	encoder_init(&encoder_FB_pid, &htim20, 0, 0, PID_PERIOD);
	encoder_init(&encoder_BA_pid, &htim1, 0, 0, PID_PERIOD);
	encoder_init(&encoder_BB_pid, &htim8, 0, 0, PID_PERIOD);

	motor_init(&motor_FA,
	           MOTOR_HW_CONFIG[MOTOR_FA].htim,
	           MOTOR_HW_CONFIG[MOTOR_FA].channel,
	           &MOTOR_HW_CONFIG[MOTOR_FA].calib);

	motor_init(&motor_FB,
	           MOTOR_HW_CONFIG[MOTOR_FB].htim,
	           MOTOR_HW_CONFIG[MOTOR_FB].channel,
	           &MOTOR_HW_CONFIG[MOTOR_FB].calib);

	motor_init(&motor_BA,
	           MOTOR_HW_CONFIG[MOTOR_BA].htim,
	           MOTOR_HW_CONFIG[MOTOR_BA].channel,
	           &MOTOR_HW_CONFIG[MOTOR_BA].calib);

	motor_init(&motor_BB,
	           MOTOR_HW_CONFIG[MOTOR_BB].htim,
	           MOTOR_HW_CONFIG[MOTOR_BB].channel,
	           &MOTOR_HW_CONFIG[MOTOR_BB].calib);

	PID_init(&pid_FA, 0, 0, 0, 0, 0, a1, b0_FA, b1_FA);
	PID_init(&pid_FB, 0, 0, 0, 0, 0, a1, b0_FB, b1_FB);
	PID_init(&pid_BA, 0, 0, 0, 0, 0, a1, b0_BA, b1_BA);
	PID_init(&pid_BB, 0, 0, 0, 0, 0, a1, b0_BB, b1_BB);

	led_stripe_init(&cfg);
	led_init(&ledA, ledA_ports, ledA_pins, OFF, led_init_state, 20);
	led_init(&ledB, ledB_ports, ledB_pins, OFF, led_init_state, 20);

	Board1_initialize();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of supervision */
  supervisionHandle = osThreadNew(supervisionTask, NULL, &supervision_attributes);

  /* creation of readSensors */
  readSensorsHandle = osThreadNew(readSensorsTask, NULL, &readSensors_attributes);

  /* creation of pid */
  pidHandle = osThreadNew(pidTask, NULL, &pid_attributes);

  /* creation of lights */
  lightsHandle = osThreadNew(lightsTask, NULL, &lights_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of SessionEvent */
  SessionEventHandle = osEventFlagsNew(&SessionEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_supervisionTask */
/**
  * @brief  Function implementing the supervision thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_supervisionTask */
void supervisionTask(void *argument)
{
  /* USER CODE BEGIN supervisionTask */

	for(;;){
			osEventFlagsWait(SessionEventHandle, EVT_SESSION, osFlagsWaitAny, pdMS_TO_TICKS(SUPERVISION_PERIOD));
			executeSupervision();
			DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_SUPERVISION);
	}
  /* USER CODE END supervisionTask */
}

/* USER CODE BEGIN Header_readSensorsTask */
/**
* @brief Function implementing the readSensors thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_readSensorsTask */
void readSensorsTask(void *argument)
{
  /* USER CODE BEGIN readSensorsTask */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(SUPERVISION_PERIOD);
	real32_T battery_voltage, temperature;

	for(;;){
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		//debug_read_sensor++;
		encoder_readRPM(&encoder_FA);
		encoder_readRPM(&encoder_FB);
		encoder_readRPM(&encoder_BA);
		encoder_readRPM(&encoder_BB);

		temperature = TEMP_getCelsius();

		battery_voltage = BATT_getVolt();

		//debug_temperature = temperature;
		//debug_battery_voltage = battery_voltage;

		velocity_FA = (int16_T) roundf(encoder_FA.velocity);
		velocity_FB = (int16_T) roundf(encoder_FB.velocity);
		velocity_BA = (int16_T) roundf(encoder_BA.velocity);
		velocity_BB = (int16_T) roundf(encoder_BB.velocity);

		taskENTER_CRITICAL();

		Board1_U.temperature = temperature;
		Board1_U.battery_voltage = battery_voltage;
		Board1_U.velocity_FA = velocity_FA;
		Board1_U.velocity_FB = velocity_FB;
		Board1_U.velocity_BA = velocity_BA;
		Board1_U.velocity_BB = velocity_BB;

		taskEXIT_CRITICAL();

		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_READ_SENSORS);
	}
  /* USER CODE END readSensorsTask */
}

/* USER CODE BEGIN Header_pidTask */
/**
* @brief Function implementing the pid thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pidTask */
void pidTask(void *argument)
{
  /* USER CODE BEGIN pidTask */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(PID_PERIOD);
	real32_T rif_FA = 0, rif_FB = 0, rif_BA = 0, rif_BB = 0;
	BRAKING_TYPE braking_mode;
	ROVER_MODE rover_mode;

	static real32_T rif_FA_r = 0;
	static real32_T rif_FB_r = 0;
	static real32_T rif_BA_r = 0;
	static real32_T rif_BB_r = 0;

	real32_T ramp_step;

	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//debug_pid++;
		taskENTER_CRITICAL();

		rif_FA = Board1_Y.output.rif_FA;
		rif_FB = Board1_Y.output.rif_FB;
		rif_BA = Board1_Y.output.rif_BA;
		rif_BB = Board1_Y.output.rif_BB;
		braking_mode = Board1_Y.output.brk_mode;
		rover_mode = Board1_Y.output.mode;

	    taskEXIT_CRITICAL();

	    encoder_readRPM(&encoder_FA_pid);
	    encoder_readRPM(&encoder_FB_pid);
	    encoder_readRPM(&encoder_BA_pid);
	    encoder_readRPM(&encoder_BB_pid);

	    switch (rover_mode)
	    {
	        case DEFAULT:
	            ramp_step = RAMP_STEP_DEFAULT;
	            break;

	        case SPORT:
	            ramp_step = RAMP_STEP_SPORT;
	            break;

	        default:
	            ramp_step = RAMP_STEP_ECO;
	            break;
	    }


	    if (braking_mode == EMERGENCY &&
	        (rif_FA == 0 && rif_FB == 0 && rif_BA == 0 && rif_BB == 0)) {
	        rif_FA_r = rif_FA;
	        rif_FB_r = rif_FB;
	        rif_BA_r = rif_BA;
	        rif_BB_r = rif_BB;
	    }
	    else if (braking_mode == NORMAL &&
	        (rif_FA == 0 && rif_FB == 0 && rif_BA == 0 && rif_BB == 0)) {
	        rif_FA_r = ramp(rif_FA_r, rif_FA, ramp_step * NORMAL_BRK_COEFF);
	        rif_FB_r = ramp(rif_FB_r, rif_FB, ramp_step * NORMAL_BRK_COEFF);
	        rif_BA_r = ramp(rif_BA_r, rif_BA, ramp_step * NORMAL_BRK_COEFF);
	        rif_BB_r = ramp(rif_BB_r, rif_BB, ramp_step * NORMAL_BRK_COEFF);
	    }
	    else {
	        rif_FA_r = ramp(rif_FA_r, rif_FA, ramp_step);
	        rif_FB_r = ramp(rif_FB_r, rif_FB, ramp_step);
	        rif_BA_r = ramp(rif_BA_r, rif_BA, ramp_step);
	        rif_BB_r = ramp(rif_BB_r, rif_BB, ramp_step);
	    }

		control_FA = PID_compute_law(&pid_FA, rif_FA_r, encoder_FA_pid.velocity);
		out_FA = abs(round(control_FA*MOTOR_MAX_DUTY/U_MAX));
		control_FB = PID_compute_law(&pid_FB, rif_FB_r, encoder_FB_pid.velocity);
		out_FB = abs(round(control_FB*MOTOR_MAX_DUTY/U_MAX));
		control_BA = PID_compute_law(&pid_BA, rif_BA_r, encoder_BA_pid.velocity);
		out_BA = abs(round(control_BA*MOTOR_MAX_DUTY/U_MAX));
		control_BB = PID_compute_law(&pid_BB, rif_BB_r, encoder_BB_pid.velocity);
		out_BB = abs(round(control_BB*MOTOR_MAX_DUTY/U_MAX));

		motor_set(&motor_FA, out_FA, (control_FA > 0) ? CLOCKWISE : COUNTERCLOCKWISE);
		motor_set(&motor_FB, out_FB, (control_FB > 0) ? CLOCKWISE : COUNTERCLOCKWISE);
		motor_set(&motor_BA, out_BA, (control_BA > 0) ? CLOCKWISE : COUNTERCLOCKWISE);
		motor_set(&motor_BB, out_BB, (control_BB > 0) ? CLOCKWISE : COUNTERCLOCKWISE);
		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_PID);
	}

  /* USER CODE END pidTask */
}

/* USER CODE BEGIN Header_lightsTask */
/**
* @brief Function implementing the lights thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lightsTask */
void lightsTask(void *argument)
{
  /* USER CODE BEGIN lightsTask */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(LIGHTS_PERIOD);

	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		taskENTER_CRITICAL();

		led_FA = Board1_Y.output.led_A;
		led_FB = Board1_Y.output.led_B;
		rear_led = Board1_Y.output.rear_led;
		rear_sign = Board1_Y.output.rear_sign;

		taskEXIT_CRITICAL();

		led_step(&ledA, led_FA);
		led_step(&ledB, led_FB);
		rear_led_step(rear_led);
		rear_sign_step(rear_sign); //aggiustare il +1
		led_render();

		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_LIGHT);
	}
  /* USER CODE END lightsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void executeSupervision(){
	HAL_TIM_Base_Start_IT(&htim4);

	debug_time = HAL_GetTick();

	do{
		debug_state = Board1_DW.is_Supervisor;
		debug_state_degraded = Board1_DW.is_Restablish;
		if(degraded == 0)
			state_good = debug_state;
		debug_count_step++;
		Board1_step();

		if(Board1_DW.is_Supervision_task != Board1_IN_Normal)
				degraded=degraded+1;

		if(Board1_DW.is_Supervision_task == Board1_IN_Single_Board)
					debug_decision = Board1_DW.decision;

		if(Board1_DW.is_Supervision_task == Board1_IN_Normal && Board1_DW.retransmitted)
			retransmit_seen_in_cycle = true;
	}
	while(Board1_DW.is_Supervisor != Board1_IN_Same_decision &&
			Board1_DW.is_Single_Board != Board1_IN_Other_board_failure &&
				Board1_DW.is_Degraded != Board1_IN_Restarting &&
					Board1_DW.is_Restablish != Boar_IN_Connection_restablished && !deadline);

	debug_diff = HAL_GetTick() - debug_time;

	HAL_TIM_Base_Stop_IT(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4, 0);

	if (retransmit_seen_in_cycle){
		count_retransmit++;
		retransmit_seen_in_cycle = false;
	}

	if(deadline){
		deadlineProcedure();
	}

	debug_output = Board1_Y.output;
}

void deadlineProcedure(){
	__disable_irq(); // Disabilitazione di ogni ulteriore Interrupt

	// Ferma tutti i motori
	motor_set(&motor_FA, 0, CLOCKWISE);
	motor_set(&motor_FB, 0, CLOCKWISE);
	motor_set(&motor_BA, 0, CLOCKWISE);
	motor_set(&motor_BB, 0, CLOCKWISE);

	// Ferma i PWM
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);

	// Ferma gli encoder
	HAL_TIM_Base_Stop(&htim20);
	HAL_TIM_Base_Stop(&htim8);
	HAL_TIM_Base_Stop(&htim5);
	HAL_TIM_Base_Stop(&htim1);


	while(1) {
	    // Blocco del sistema in condizioni di sicurezza
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == SESSION_Pin)
	{
		debug_rts++;
		osEventFlagsSet(SessionEventHandle, EVT_SESSION);
	}
}
/* USER CODE END Application */

