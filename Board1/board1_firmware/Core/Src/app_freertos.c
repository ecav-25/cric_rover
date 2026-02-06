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
#include "pid_law.h"
#include "motor.h"
#include "stdlib.h"
#include "led.h"
#include "led_stripes.h"
#include "deadline_watchdog.h"
#include "utils.h"
#include "hw_config.h"
#include "motor_diagnostic.h"
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

#define RAMP_STEP_DEFAULT (RAMP_SLOPE_DEFAULT_RPM_S * (PID_PERIOD / 1000.0f))
#define RAMP_STEP_SPORT   (RAMP_SLOPE_SPORT_RPM_S   * (PID_PERIOD / 1000.0f))
#define RAMP_STEP_ECO     (RAMP_SLOPE_ECO_RPM_S     * (PID_PERIOD / 1000.0f))

#define DIAG_DELAY_SHIFT    0
#define DIAG_MAX_AREA_ERR   15000.0f


#define TOGGLE_EMERGENCY 8
#define TOGGLE_NORMAL 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


MotorDiag_Handle_t h_diag_FA;
MotorDiag_Handle_t h_diag_FB;
MotorDiag_Handle_t h_diag_BA;
MotorDiag_Handle_t h_diag_BB;


volatile float    debug_area_FA   = 0.0f;
volatile uint8_t  motor_status_FA = 1;

volatile float    debug_area_FB   = 0.0f;
volatile uint8_t  motor_status_FB = 1;

volatile float    debug_area_BA   = 0.0f;
volatile uint8_t  motor_status_BA = 1;

volatile float    debug_area_BB   = 0.0f;
volatile uint8_t  motor_status_BB = 1;

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

temp_t temp_sensor;
batt_t battery_sensor;

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
/*
PID_t pid_FA;
PID_t pid_FB;
PID_t pid_BA;
PID_t pid_BB;
*/
PID_Law_t pid_FA;
PID_Law_t pid_FB;
PID_Law_t pid_BA;
PID_Law_t pid_BB;

real32_T a1 = 1;
real32_T b0_FA = 0.011526, b0_FB = 0.012068, b0_BA = 0.01182, b0_BB = 0.011206;
real32_T b1_FA = -0.0062, b1_FB = -0.0066, b1_BA = -0.0062, b1_BB = -0.0059;


/*Variabili per il pid classico*/
/*
float kpFA = 0.009319;
float kiFA = 1.1;

float kpFB = 0.008858;
float kiFB = 1.067;

float kpBA = 0.008989;
float kiBA = 1.133;

float kpBB = 0.008543;
float kiBB = 1.065;
*/

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

boolean_T deadline = 0;

real32_T ramp_step;

led_t ledA;
led_t ledB;

LED_TYPE led_FA = OFF, led_FB = OFF;
REAR_SIGN_TYPE rear_sign = OFF;
REAR_LED_TYPE rear_led = IDLE;



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
static Encoder_Status_t init_encoders_supervision(void)
{
    if (encoder_init(&encoder_FA, ENCODER_HW_CONFIG[ENCODER_FA].htim, &ENCODER_HW_CONFIG[ENCODER_FA].calib, SUPERVISION_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_FB, ENCODER_HW_CONFIG[ENCODER_FB].htim, &ENCODER_HW_CONFIG[ENCODER_FB].calib, SUPERVISION_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_BA, ENCODER_HW_CONFIG[ENCODER_BA].htim, &ENCODER_HW_CONFIG[ENCODER_BA].calib, SUPERVISION_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_BB, ENCODER_HW_CONFIG[ENCODER_BB].htim, &ENCODER_HW_CONFIG[ENCODER_BB].calib, SUPERVISION_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    return ENCODER_OK;
}

static Encoder_Status_t init_encoders_pid(void){
    if (encoder_init(&encoder_FA_pid, ENCODER_HW_CONFIG[ENCODER_FA].htim, &ENCODER_HW_CONFIG[ENCODER_FA].calib, PID_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_FB_pid, ENCODER_HW_CONFIG[ENCODER_FB].htim, &ENCODER_HW_CONFIG[ENCODER_FB].calib, PID_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_BA_pid, ENCODER_HW_CONFIG[ENCODER_BA].htim, &ENCODER_HW_CONFIG[ENCODER_BA].calib, PID_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    if (encoder_init(&encoder_BB_pid, ENCODER_HW_CONFIG[ENCODER_BB].htim, &ENCODER_HW_CONFIG[ENCODER_BB].calib, PID_PERIOD) != ENCODER_OK){
        return ENCODER_ERR;
    }

    return ENCODER_OK;
}

static Motor_Status_t init_motors(void)
{
    if (motor_init(&motor_FA, MOTOR_HW_CONFIG[MOTOR_FA].htim, MOTOR_HW_CONFIG[MOTOR_FA].channel, &MOTOR_HW_CONFIG[MOTOR_FA].calib) != MOTOR_OK){
        return MOTOR_ERR;
    }

    if (motor_init(&motor_FB, MOTOR_HW_CONFIG[MOTOR_FB].htim, MOTOR_HW_CONFIG[MOTOR_FB].channel, &MOTOR_HW_CONFIG[MOTOR_FB].calib) != MOTOR_OK){
        return MOTOR_ERR;
    }

    if (motor_init(&motor_BA, MOTOR_HW_CONFIG[MOTOR_BA].htim, MOTOR_HW_CONFIG[MOTOR_BA].channel, &MOTOR_HW_CONFIG[MOTOR_BA].calib) != MOTOR_OK){
        return MOTOR_ERR;
    }

    if (motor_init(&motor_BB, MOTOR_HW_CONFIG[MOTOR_BB].htim, MOTOR_HW_CONFIG[MOTOR_BB].channel, &MOTOR_HW_CONFIG[MOTOR_BB].calib) != MOTOR_OK){
        return MOTOR_ERR;
    }

    return MOTOR_OK;
}
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

	if(batt_init(&battery_sensor, ADC_HW_CONFIG[ADC_BATTERY_VOLTAGE].hadc, &ADC_HW_CONFIG[ADC_BATTERY_VOLTAGE].channel_cfg, BATT_DEFAULT_TIMEOUT_MS) != BATT_OK){
		Error_Handler();
		return;
	}

	if(temp_init(&temp_sensor,ADC_HW_CONFIG[ADC_TEMP_SENSOR].hadc, &ADC_HW_CONFIG[ADC_TEMP_SENSOR].channel_cfg, TEMP_DEFAULT_TIMEOUT_MS) != TEMP_OK){
		Error_Handler();
		return;
	}

	if (init_encoders_supervision() != ENCODER_OK){
		Error_Handler();
		return;
	}
	if (init_encoders_pid() != ENCODER_OK){
		Error_Handler();
		return;
	}
	if (init_motors() != MOTOR_OK){
		Error_Handler();
		return;
	}

	MotorDiag_Config_t diag_cfg = {
		.delay_shift = DIAG_DELAY_SHIFT,
		.max_area_limit = DIAG_MAX_AREA_ERR
	};


	if (motor_diag_init(&h_diag_FA, &diag_cfg) != M_DIAG_OK) {
		Error_Handler();
		return;
	}

	if (motor_diag_init(&h_diag_FB, &diag_cfg) != M_DIAG_OK) {
		Error_Handler();
		return;
	}

	if (motor_diag_init(&h_diag_BA, &diag_cfg) != M_DIAG_OK) {
		Error_Handler();
		return;
	}

	if (motor_diag_init(&h_diag_BB, &diag_cfg) != M_DIAG_OK) {
		Error_Handler();
		return;
	}

	/*
	if (PID_init(&pid_FA, kpFA, kiFA, 0, 5, 0) != PID_OK) {
		Error_Handler();
		return;
	}

	if (PID_init(&pid_FB, kpFB, kiFB, 0, 5, 0) != PID_OK) {
		Error_Handler();
		return;
	}

	if (PID_init(&pid_BA, kpBA, kiBA, 0, 5, 0) != PID_OK) {
		Error_Handler();
		return;
	}

	if (PID_init(&pid_BB, kpBB, kiBB, 0, 5, 0) != PID_OK) {
		Error_Handler();
		return;
	}
	*/

	if (PID_Law_init(&pid_FA, a1, b0_FA, b1_FA) != PID_LAW_OK) {
	    Error_Handler();
		return;
	}

	if (PID_Law_init(&pid_FB, a1, b0_FB, b1_FB) != PID_LAW_OK) {
	    Error_Handler();
		return;
	}

	if (PID_Law_init(&pid_BA, a1, b0_BA, b1_BA) != PID_LAW_OK) {
	    Error_Handler();
	    return;
	}

	if (PID_Law_init(&pid_BB, a1, b0_BB, b1_BB) != PID_LAW_OK) {
	    Error_Handler();
	    return;
	}

	if(led_stripe_init(&led_stripes_cfg[LED_STRIPES_MAIN]) != LED_STRIPE_OK){
		Error_Handler();
		return;
	}

	if(led_init(&ledA, HW_LED_CONFIG[LED_A].port, HW_LED_CONFIG[LED_A].pin, HW_LED_CONFIG[LED_A].init_pin_state, HW_LED_CONFIG[LED_A].toggle_steps) != LED_OK){
		Error_Handler();
		return;
	}

	if(led_init(&ledB, HW_LED_CONFIG[LED_B].port, HW_LED_CONFIG[LED_B].pin, HW_LED_CONFIG[LED_B].init_pin_state, HW_LED_CONFIG[LED_B].toggle_steps) != LED_OK){
		Error_Handler();
		return;
	}

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

	real32_T battery_voltage_raw = 0.0f; // Variabile per la lettura istantanea
	real32_T temperature;

	// --- VARIABILI PER IL FILTRO BATTERIA ---
	// Alpha determina quanto "pesa" la nuova lettura.
	// 0.1 = 10% nuova lettura, 90% storico (Molto filtrato, lento)
	// 0.5 = 50% nuova lettura, 50% storico (Poco filtrato, veloce)
	// Valore simile al tuo Arduino: prova tra 0.05f e 0.1f
	const real32_T batt_alpha = 0.05f;

	// Inizializziamo a un valore negativo per capire se è la prima lettura
	static real32_T battery_voltage_filtered = -1.0f;
	// ----------------------------------------

	for(;;){
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		// ... (codice encoder esistente omesso per brevità) ...
		if (encoder_readRPM(&encoder_FA) != ENCODER_OK) { Error_Handler(); }
		if (encoder_readRPM(&encoder_FB) != ENCODER_OK) { Error_Handler(); }
		if (encoder_readRPM(&encoder_BA) != ENCODER_OK) { Error_Handler(); }
		if (encoder_readRPM(&encoder_BB) != ENCODER_OK) { Error_Handler(); }


		if(temp_get_celsius_once(&temp_sensor, &temperature) != TEMP_OK){
			//Error_Handler();
			__NOP();
		}

		// Leggi il valore ISTANTANEO (RAW)
		if(batt_get_voltage_once(&battery_sensor, &battery_voltage_raw) != BATT_OK){
			//Error_Handler();
			__NOP();
		} else {
			// --- LOGICA DI FILTRAGGIO ---
			if (battery_voltage_filtered < 0.0f) {
				// Se è la prima accensione, prendiamo il valore così com'è
				// per evitare che il filtro parta da 0V e ci metta tempo a salire.
				battery_voltage_filtered = battery_voltage_raw;
			} else {
				// Formula EMA: (Nuovo * alpha) + (Vecchio * (1 - alpha))
				battery_voltage_filtered = (battery_voltage_raw * batt_alpha) +
										   (battery_voltage_filtered * (1.0f - batt_alpha));
			}
			// ----------------------------------------------------
		}

		debug_temperature = temperature;
		debug_battery_voltage = battery_voltage_filtered;

		velocity_FA = (int16_T) roundf(encoder_FA.velocity);
		velocity_FB = (int16_T) roundf(encoder_FB.velocity);
		velocity_BA = (int16_T) roundf(encoder_BA.velocity);
		velocity_BB = (int16_T) roundf(encoder_BB.velocity);

		if (motor_diag_process(&h_diag_FA) != M_DIAG_OK) {
			Error_Handler();
		}

		if (motor_diag_process(&h_diag_FB) != M_DIAG_OK) {
		Error_Handler();
		}

		if (motor_diag_process(&h_diag_BA) != M_DIAG_OK) {
		Error_Handler();
		}

		if (motor_diag_process(&h_diag_BB) != M_DIAG_OK) {
		Error_Handler();
		}

		taskENTER_CRITICAL();

		Board1_U.temperature = temperature;
		Board1_U.battery_voltage = battery_voltage_filtered;
		Board1_U.velocity_FA = velocity_FA;
		Board1_U.velocity_FB = velocity_FB;
		Board1_U.velocity_BA = velocity_BA;
		Board1_U.velocity_BB = velocity_BB;

		Board1_U.motorError_FA = (h_diag_FA.health_status == MOTOR_FAILURE);
		Board1_U.motorError_FB = (h_diag_FB.health_status == MOTOR_FAILURE);
		Board1_U.motorError_BA = (h_diag_BA.health_status == MOTOR_FAILURE);
		Board1_U.motorError_BB = (h_diag_BB.health_status == MOTOR_FAILURE);

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

	    if (encoder_readRPM(&encoder_FA_pid) != ENCODER_OK) {
	        Error_Handler();
	    }

	    if (encoder_readRPM(&encoder_FB_pid) != ENCODER_OK) {
	        Error_Handler();
	    }

	    if (encoder_readRPM(&encoder_BA_pid) != ENCODER_OK) {
	        Error_Handler();
	    }

	    if (encoder_readRPM(&encoder_BB_pid) != ENCODER_OK) {
	        Error_Handler();
	    }

	    switch (rover_mode)
	    {
	        case SPORT:
	            ramp_step = RAMP_STEP_SPORT;
	            break;

	        case ECO:
	        	ramp_step = RAMP_STEP_ECO;
	            break;

	        default:
	            ramp_step = RAMP_STEP_DEFAULT;
	            break;
	    }

	    rif_FA_r = ramp(rif_FA_r, rif_FA, ramp_step, braking_mode);
	    rif_FB_r = ramp(rif_FB_r, rif_FB, ramp_step, braking_mode);
	    rif_BA_r = ramp(rif_BA_r, rif_BA, ramp_step, braking_mode);
	    rif_BB_r = ramp(rif_BB_r, rif_BB, ramp_step, braking_mode);

	    if (motor_diag_record(&h_diag_FA, rif_FA_r, encoder_FA_pid.velocity) != M_DIAG_OK) {
	        Error_Handler();
	    }

		if (motor_diag_record(&h_diag_FB, rif_FB_r, encoder_FB_pid.velocity) != M_DIAG_OK) {
			Error_Handler();
		}

		if (motor_diag_record(&h_diag_BA, rif_BA_r, encoder_BA_pid.velocity) != M_DIAG_OK) {
			Error_Handler();
		}

		if (motor_diag_record(&h_diag_BB, rif_BB_r, encoder_BB_pid.velocity) != M_DIAG_OK) {
			Error_Handler();
		}

/*
	    if (PID_compute(&pid_FA, rif_FA_r, encoder_FA_pid.velocity, &control_FA) != PID_OK) {
			Error_Handler();
		}
		out_FA = abs(round(control_FA * MOTOR_MAX_DUTY / U_MAX));

		if (PID_compute(&pid_FB, rif_FB_r, encoder_FB_pid.velocity, &control_FB) != PID_OK) {
			Error_Handler();
		}
		out_FB = abs(round(control_FB * MOTOR_MAX_DUTY / U_MAX));

		if (PID_compute(&pid_BA, rif_BA_r, encoder_BA_pid.velocity, &control_BA) != PID_OK) {
			Error_Handler();
		}
		out_BA = abs(round(control_BA * MOTOR_MAX_DUTY / U_MAX));

		if (PID_compute(&pid_BB, rif_BB_r, encoder_BB_pid.velocity, &control_BB) != PID_OK) {
			Error_Handler();
		}
		out_BB = abs(round(control_BB * MOTOR_MAX_DUTY / U_MAX));
*/

	    if (PID_Law_compute(&pid_FA, rif_FA_r, encoder_FA_pid.velocity, &control_FA) != PID_LAW_OK) {
	        Error_Handler();
	    }
	    out_FA = abs(round(control_FA * MOTOR_MAX_DUTY / U_MAX));

	    if (PID_Law_compute(&pid_FB, rif_FB_r, encoder_FB_pid.velocity, &control_FB) != PID_LAW_OK) {
	        Error_Handler();
	    }
	    out_FB = abs(round(control_FB * MOTOR_MAX_DUTY / U_MAX));

	    if (PID_Law_compute(&pid_BA, rif_BA_r, encoder_BA_pid.velocity, &control_BA) != PID_LAW_OK) {
	        Error_Handler();
	    }
	    out_BA = abs(round(control_BA * MOTOR_MAX_DUTY / U_MAX));

	    if (PID_Law_compute(&pid_BB, rif_BB_r, encoder_BB_pid.velocity, &control_BB) != PID_LAW_OK) {
	        Error_Handler();
	    }
	    out_BB = abs(round(control_BB * MOTOR_MAX_DUTY / U_MAX));

	    if (motor_set(&motor_FA, out_FA, (control_FA > 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
	        Error_Handler();
	    }

	    if (motor_set(&motor_FB, out_FB, (control_FB > 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
	        Error_Handler();
	    }

	    if (motor_set(&motor_BA, out_BA, (control_BA > 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
	        Error_Handler();
	    }

	    if (motor_set(&motor_BB, out_BB, (control_BB > 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
	        Error_Handler();
	    }


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
		rear_sign_step(rear_sign);
		led_render();

		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_LIGHT);
	}
  /* USER CODE END lightsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void executeSupervision(){
	debug_time = HAL_GetTick();

	do{
		debug_state = Board1_DW.is_Supervisor;
		debug_state_degraded = Board1_DW.is_Restablish;
		if(degraded == 0)
			state_good = debug_state;
		debug_count_step++;
		Board1_step();

		if(Board1_DW.is_Board_state != Board1_IN_Normal)
				degraded=degraded+1;

		if(Board1_DW.is_Board_state == Board1_IN_Single_Board)
			debug_decision = Board1_DW.decision;

		if(Board1_DW.is_Board_state == Board1_IN_Normal && Board1_DW.retransmitted)
			retransmit_seen_in_cycle = true;
	}
	while(Board1_DW.is_Supervisor != Board1_IN_Same_decision &&
			Board1_DW.is_Single_Board != Board1_IN_Other_board_failure &&
				Board1_DW.is_Degraded != Board1_IN_Restarting &&
					Board1_DW.is_Restablish != Boar_IN_Connection_restablished);

	debug_diff = HAL_GetTick() - debug_time;

	if (retransmit_seen_in_cycle){
		count_retransmit++;
		retransmit_seen_in_cycle = false;
	}

	debug_output = Board1_Y.output;
}

void deadlineProcedure(){
	__disable_irq(); // Disabilitazione di ogni ulteriore Interrupt

	// Ferma tutti i motori
	if (motor_set(&motor_FA, 0, CLOCKWISE) != MOTOR_OK) {
	    Error_Handler();
	}

	if (motor_set(&motor_FB, 0, CLOCKWISE) != MOTOR_OK) {
	    Error_Handler();
	}

	if (motor_set(&motor_BA, 0, CLOCKWISE) != MOTOR_OK) {
	    Error_Handler();
	}

	if (motor_set(&motor_BB, 0, CLOCKWISE) != MOTOR_OK) {
	    Error_Handler();
	}


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

