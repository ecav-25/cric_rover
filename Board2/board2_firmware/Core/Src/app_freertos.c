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
#include "Board2.h"
#include "dwt_delay.h"
#include "hcsr04.h"
#include "controller.h"
#include "mpu6050.h"
#include "motor.h"
#include "stdlib.h"
#include "deadline_watchdog.h"
#include "utils.h"
#include "hw_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DWD_FLAG_SUPERVISION		(1U << 0)
#define DWD_FLAG_SONAR_SENSORS		(1U << 1)

#define SYSTEM_ALIVE_MASK (DWD_FLAG_SUPERVISION | DWD_FLAG_SONAR_SENSORS)

#define SUPERVISION_PERIOD		(60)
#define TELEMETRY_LOGGER_PERIOD	(120)
#define SONAR_SCAN_PERIOD 		(40)
#define MAX_RPM 				(150)

#define OPENLOOP_STEP       	(OPENLOOP_RAMP_SLOPE * (SUPERVISION_PERIOD / 1000.0))

#define SONAR_NUMBER			(3)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static void app_delay_us(uint32_t us)
{
    (void)DWT_Delay_us(us);
}

typedef enum { S_LEFT = 0, S_CENTER, S_RIGHT } rr_state_t;

static volatile uint16_T g_sonar_distances[3];

static hcsr04_t us_left, us_center, us_right;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
Deadline_Watchdog_t hard_rt_deadline_wd;
uint16_T controller_x = 0;
uint16_T controller_y = 0;
boolean_T button1 = 0;
boolean_T button2 = 0;
boolean_T button3 = 0;
boolean_T button4 = 0;
uint8_T controller_battery = 0;

boolean_T b_r_stick = 0;
volatile boolean_T debug_r_stick = 0;
volatile boolean_T debug_button1 = 0;
volatile boolean_T debug_button2 = 0;
volatile boolean_T debug_button3 = 0;
volatile boolean_T debug_button4 = 0;

DecBus debug_decision;

Controller_t controller;
mpu6050_t mpu_device;
imu_vector_t acceleration;
imu_vector_t gyroyaw;

Motor_t motor_FA_openLoop;
Motor_t motor_FB_openLoop;
Motor_t motor_BA_openLoop;
Motor_t motor_BB_openLoop;

DecBus debug_output;
volatile uint32_t debug_count_step = 0;
volatile uint8_t debug_state_degraded = 0;
volatile uint8_t debug_state = 0;
volatile uint16_t debug_sonar1=400;
volatile uint16_t debug_sonar2=400;
volatile uint16_t debug_sonar3=400;
volatile MPU60X0_StatusTypeDef status_acc = 0;
volatile MPU60X0_StatusTypeDef status_gyro = 0;
volatile real32_T debug_acc_x = 0;
volatile real32_T debug_acc_y = 0;
volatile real32_T debug_gyroYaw = 0;
volatile int16_t debug_controller_x = 255;
volatile int16_t debug_controller_y = 255;
uint32_t debug_time = 0, debug_diff = 0, debug_diff_2 = 0;
uint32_t degraded=0;
uint8_t state_good=0;
boolean_T retransmit_seen_in_cycle = false;
uint32_t count_retransmit=0;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern I2C_HandleTypeDef hi2c1;

extern UART_HandleTypeDef huart3;

boolean_T deadline = 0;
/* USER CODE END Variables */
/* Definitions for readSonar */
osThreadId_t readSonarHandle;
uint32_t readSonarBuffer[ 384 ];
osStaticThreadDef_t readSonarControlBlock;
const osThreadAttr_t readSonar_attributes = {
  .name = "readSonar",
  .stack_mem = &readSonarBuffer[0],
  .stack_size = sizeof(readSonarBuffer),
  .cb_mem = &readSonarControlBlock,
  .cb_size = sizeof(readSonarControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
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
/* Definitions for telemetryLogger */
osThreadId_t telemetryLoggerHandle;
uint32_t telemetryLoggerBuffer[ 512 ];
osStaticThreadDef_t telemetryLoggerControlBlock;
const osThreadAttr_t telemetryLogger_attributes = {
  .name = "telemetryLogger",
  .stack_mem = &telemetryLoggerBuffer[0],
  .stack_size = sizeof(telemetryLoggerBuffer),
  .cb_mem = &telemetryLoggerControlBlock,
  .cb_size = sizeof(telemetryLoggerControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void supervision_read_inputs(void);
void executeSupervision();
static void supervision_apply_actuation(void);
void deadlineProcedure();
/* USER CODE END FunctionPrototypes */

void readSonarTask(void *argument);
void supervisionTask(void *argument);
void telemetryLoggerTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	DWD_Init(&hard_rt_deadline_wd, &htim4, SYSTEM_ALIVE_MASK, deadlineProcedure);
	(void)DWT_Delay_Init();

	hcsr04_cfg_t base = {
	  .htim = &htim2,
	  .timer_hz = 1000000,
	  .timeout_ms = 30,
	  .delay_us = app_delay_us
	};

	/* LEFT:  TIM2_CH1, TRIG PA0 */
	hcsr04_cfg_t cL = base;
	cL.channel = TIM_CHANNEL_1;
	cL.trig_port = TRIG_LEFT_GPIO_Port;
	cL.trig_pin  = TRIG_LEFT_Pin;

	/* CENTER: TIM2_CH2, TRIG PA1 */
	hcsr04_cfg_t cC = base;
	cC.channel = TIM_CHANNEL_2;
	cC.trig_port = TRIG_CENTER_GPIO_Port;
	cC.trig_pin  = TRIG_CENTER_Pin;

	/* RIGHT: TIM2_CH3, TRIG PA2 */
	hcsr04_cfg_t cR = base;
	cR.channel = TIM_CHANNEL_3;
	cR.trig_port = TRIG_RIGHT_GPIO_Port;
	cR.trig_pin  = TRIG_RIGHT_Pin;

	(void)HCSR04_Init(&us_left,   &cL);
	(void)HCSR04_Init(&us_center, &cC);
	(void)HCSR04_Init(&us_right,  &cR);

	telecontrol_init(&controller, &hi2c1);

	mpu6050_init(&mpu_device, &hi2c1, 0, NULL);

	if (motor_init(&motor_FA_openLoop, MOTOR_HW_CONFIG[MOTOR_FA].htim, MOTOR_HW_CONFIG[MOTOR_FA].channel, &MOTOR_HW_CONFIG[MOTOR_FA].calib) != MOTOR_OK){
	    Error_Handler();
	}

	if (motor_init(&motor_FB_openLoop, MOTOR_HW_CONFIG[MOTOR_FB].htim, MOTOR_HW_CONFIG[MOTOR_FB].channel, &MOTOR_HW_CONFIG[MOTOR_FB].calib) != MOTOR_OK) {
	    Error_Handler();
	}

	if (motor_init(&motor_BA_openLoop, MOTOR_HW_CONFIG[MOTOR_BA].htim, MOTOR_HW_CONFIG[MOTOR_BA].channel, &MOTOR_HW_CONFIG[MOTOR_BA].calib) != MOTOR_OK){
	    Error_Handler();
	}

	if (motor_init(&motor_BB_openLoop, MOTOR_HW_CONFIG[MOTOR_BB].htim, MOTOR_HW_CONFIG[MOTOR_BB].channel, &MOTOR_HW_CONFIG[MOTOR_BB].calib) != MOTOR_OK){
	    Error_Handler();
	}


	Board2_initialize();
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
  /* creation of readSonar */
  readSonarHandle = osThreadNew(readSonarTask, NULL, &readSonar_attributes);

  /* creation of supervision */
  supervisionHandle = osThreadNew(supervisionTask, NULL, &supervision_attributes);

  /* creation of telemetryLogger */
  telemetryLoggerHandle = osThreadNew(telemetryLoggerTask, NULL, &telemetryLogger_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_readSonarTask */
/**
  * @brief  Function implementing the readSonar thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_readSonarTask */
void readSonarTask(void *argument)
{
  /* USER CODE BEGIN readSonarTask */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(SONAR_SCAN_PERIOD);

	hcsr04_t *dev[SONAR_NUMBER] = { &us_left, &us_center, &us_right };

	// Variabili di stato per la gestione ciclica
	uint8_t curr_idx = 0;      // Indice del sonar da avviare (Start)
	bool first_run = true;     // Flag per saltare la lettura al primo avvio assoluto


	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		if (!first_run)
		{
			uint8_t prev_idx = (curr_idx == 0) ? (SONAR_NUMBER - 1) : (curr_idx - 1);

			uint16_T cm = 0;

			(void)HCSR04_GetDistanceCm(dev[prev_idx], &cm);

			g_sonar_distances[prev_idx]=cm;

			if (prev_idx == (SONAR_NUMBER - 1))
			{
				taskENTER_CRITICAL();

				Board2_U.sonar1 = g_sonar_distances[S_LEFT];
				Board2_U.sonar2 = g_sonar_distances[S_CENTER];
				Board2_U.sonar3 = g_sonar_distances[S_RIGHT];

				taskEXIT_CRITICAL();
			}
		}

		hcsr04_status_t st_start = HCSR04_Start(dev[curr_idx]);
		if (st_start != HCSR04_OK) {
			g_sonar_distances[curr_idx]=0;
		}

		curr_idx++;
		first_run=false;

		if (curr_idx >= SONAR_NUMBER) {
			curr_idx = 0;
		}

		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_SONAR_SENSORS);
	}

  /* USER CODE END readSonarTask */
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

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(SUPERVISION_PERIOD);

	for(;;){
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		supervision_read_inputs();

		executeSupervision();

		supervision_apply_actuation();

		DWD_Notify(&hard_rt_deadline_wd, DWD_FLAG_SUPERVISION);
	  }
  /* USER CODE END supervisionTask */
}

/* USER CODE BEGIN Header_telemetryLoggerTask */
/**
* @brief Function implementing the telemetryLogger thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_telemetryLoggerTask */
void telemetryLoggerTask(void *argument)
{
  /* USER CODE BEGIN telemetryLoggerTask */
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TELEMETRY_LOGGER_PERIOD);

    Telemetry_t telemetry;

    StateBusB1 stateB1;
    DecBus output;

    uint16_T sonar1, sonar2, sonar3;

    uint8_T supervision_state;
    uint8_T  light_state;
    boolean_T special_retro;

    if (telemetry_init(&telemetry) != CONTROLLER_OK) {
        Error_Handler();
    }

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        /* ===================== SNAPSHOT ATOMICO ===================== */
        taskENTER_CRITICAL();

        sonar1 = Board2_U.sonar1;
        sonar2 = Board2_U.sonar2;
        sonar3 = Board2_U.sonar3;

        output = Board2_Y.output;

        supervision_state = Board2_DW.is_Supervision_task;
        light_state       = Board2_DW.is_Normal_voltage_h;
        special_retro     = Board2_DW.special_retro;

        if (supervision_state == Board2_IN_Normal) {
            stateB1 = Board2_DW.global_state.stateB1;
        }

        taskEXIT_CRITICAL();
        /* ============================================================ */

        /* ===================== NORMAL MODE ===================== */
        if (supervision_state == Board2_IN_Normal)
        {
            telemetry_set_backward_mode(
                &telemetry,
                special_retro ? BW_SPECIAL : BW_NORMAL
            );

            telemetry_set_battery(
                &telemetry,
                calculate_battery_percent(stateB1.battery_voltage)
            );

            telemetry_set_temperature(
                &telemetry,
                saturate_temperature(stateB1.temperature)
            );

            telemetry_set_light_mode(
                &telemetry,
                get_light_mode(light_state)
            );

            telemetry_set_rpm(
                &telemetry,
                stateB1.velocity_FA,
                stateB1.velocity_FB,
                stateB1.velocity_BA,
                stateB1.velocity_BB
            );

            telemetry_set_operating_mode(&telemetry, OM_NORMAL);
        }
        /* ===================== NON-NORMAL MODES ===================== */
        else
        {
            telemetry_set_backward_mode(&telemetry, BW_NORMAL);
            telemetry_set_battery(&telemetry, 0);
            telemetry_set_temperature(&telemetry, 0);
            telemetry_set_light_mode(&telemetry, LIGHT_OFF);

            telemetry_set_rpm(&telemetry, 0, 0, 0, 0);

            telemetry_set_operating_mode(
                &telemetry,
                (supervision_state == Board2_IN_Degraded)
                    ? OM_DEGRADED
                    : OM_SINGLE_BOARD
            );
        }

        /* ===================== COMMON FIELDS ===================== */

        telemetry_set_sonars(
            &telemetry,
            sonar1,
            sonar2,
            sonar3
        );

        telemetry_set_targets(
            &telemetry,
            output.rif_FA,
            output.rif_FB,
            output.rif_BA,
            output.rif_BB);

        /* ===================== TX ===================== */

        //Qui va fatta una gestione senza ErrorHandler in caso di errore
        telecontrol_send_telemetry(&controller, &telemetry);
    }
  /* USER CODE END telemetryLoggerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void supervision_read_inputs(void)
{
    // Lettura hardware
    //Qui va fatta una gestione senza ErrorHandler in caso di errore
    telecontrol_read(&controller);
    mpu6050_get_gyro_value(&mpu_device, &gyroyaw);
    mpu6050_get_accel_value(&mpu_device, &acceleration);

    Board2_U.controller_x = get_telecontrol_bx(&controller);
    Board2_U.controller_y = get_telecontrol_ay(&controller);
    Board2_U.B1 = get_telecontrol_button_btn1(&controller);
    Board2_U.B2 = get_telecontrol_button_btn2(&controller);
    Board2_U.B3 = get_telecontrol_button_btn3(&controller);
    Board2_U.B4 = get_telecontrol_button_btn4(&controller);
    Board2_U.B_r_stick = get_telecontrol_b_btn(&controller);
    Board2_U.B_l_stick = get_telecontrol_a_btn(&controller);
    Board2_U.controller_battery = get_telecontrol_percentage(&controller);
    Board2_U.acceleration_x = acceleration.x;
    Board2_U.acceleration_y = acceleration.y;
    Board2_U.gyroYaw = gyroyaw.z;
}

void executeSupervision(){
	do{
		debug_state = Board2_DW.is_Supervisor;
		debug_state_degraded = Board2_DW.is_Restablish;
		if(degraded == 0)
			state_good = debug_state;
		debug_count_step++;
		Board2_step();
		if(Board2_DW.is_Supervision_task != Board2_IN_Normal)
			degraded=degraded+1;

		if(Board2_DW.is_Supervision_task == Board2_IN_Single_Board)
			debug_decision = Board2_DW.decision;

		if(Board2_DW.is_Supervision_task == Board2_IN_Normal && Board2_DW.retransmitted)
			retransmit_seen_in_cycle = true;
	}
	while(Board2_DW.is_Supervisor != Board2_IN_Same_decision &&
			Board2_DW.is_Single_Board != Board2_IN_Other_board_failure &&
				Board2_DW.is_Degraded != Board2_IN_Restarting &&
					Board2_DW.is_Restablish != Boar_IN_Connection_restablished && !deadline);

	if (retransmit_seen_in_cycle){
	      count_retransmit++;
	      retransmit_seen_in_cycle = false;
	}

	debug_output = Board2_Y.output;
}

static void supervision_apply_actuation(void)
{
    // Variabili statiche per mantenere lo stato della rampa tra le chiamate
    static real32_T rif_FA_r = 0;
    static real32_T rif_FB_r = 0;
    static real32_T rif_BA_r = 0;
    static real32_T rif_BB_r = 0;

    real32_T rif_FA, rif_FB, rif_BA, rif_BB;
    BRAKING_TYPE braking_mode;
    uint8_T duty_FA, duty_FB, duty_BA, duty_BB;

    // Gestione Relè
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin,
                      Board2_Y.output.relay ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Lettura output modello
    rif_FA = Board2_Y.output.rif_FA;
    rif_FB = Board2_Y.output.rif_FB;
    rif_BA = Board2_Y.output.rif_BA;
    rif_BB = Board2_Y.output.rif_BB;
    braking_mode = Board2_Y.output.brk_mode;

    // Logica Ramping
    if (braking_mode == EMERGENCY &&
        rif_FA == 0 && rif_FB == 0 && rif_BA == 0 && rif_BB == 0) {
        // Reset immediato rampe in emergenza
        rif_FA_r = rif_FA;
        rif_FB_r = rif_FB;
        rif_BA_r = rif_BA;
        rif_BB_r = rif_BB;
    }
    else if (braking_mode == NORMAL &&
             rif_FA == 0 && rif_FB == 0 && rif_BA == 0 && rif_BB == 0) {
        // Frenata dolce
        rif_FA_r = ramp(rif_FA_r, rif_FA, OPENLOOP_STEP * NORMAL_BRK_COEFF);
        rif_FB_r = ramp(rif_FB_r, rif_FB, OPENLOOP_STEP * NORMAL_BRK_COEFF);
        rif_BA_r = ramp(rif_BA_r, rif_BA, OPENLOOP_STEP * NORMAL_BRK_COEFF);
        rif_BB_r = ramp(rif_BB_r, rif_BB, OPENLOOP_STEP * NORMAL_BRK_COEFF);
    }
    else {
        // Funzionamento normale
        rif_FA_r = ramp(rif_FA_r, rif_FA, OPENLOOP_STEP);
        rif_FB_r = ramp(rif_FB_r, rif_FB, OPENLOOP_STEP);
        rif_BA_r = ramp(rif_BA_r, rif_BA, OPENLOOP_STEP);
        rif_BB_r = ramp(rif_BB_r, rif_BB, OPENLOOP_STEP);
    }

    /* ====== RPM -> duty % (0-100) ====== */
    duty_FA = (uint8_T)(abs(rif_FA_r) * MOTOR_MAX_DUTY / MAX_RPM);
    duty_FB = (uint8_T)(abs(rif_FB_r) * MOTOR_MAX_DUTY / MAX_RPM);
    duty_BA = (uint8_T)(abs(rif_BA_r) * MOTOR_MAX_DUTY / MAX_RPM);
    duty_BB = (uint8_T)(abs(rif_BB_r) * MOTOR_MAX_DUTY / MAX_RPM);

    /* ====== SATURAZIONE ====== */
    if (duty_FA > MOTOR_MAX_DUTY) duty_FA = MOTOR_MAX_DUTY;
    if (duty_FB > MOTOR_MAX_DUTY) duty_FB = MOTOR_MAX_DUTY;
    if (duty_BA > MOTOR_MAX_DUTY) duty_BA = MOTOR_MAX_DUTY;
    if (duty_BB > MOTOR_MAX_DUTY) duty_BB = MOTOR_MAX_DUTY;

    /* ====== COMANDO MOTORI ====== */
    if (motor_set(&motor_FA_openLoop, duty_FA,
                  (rif_FA_r >= 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
        Error_Handler();
    }

    if (motor_set(&motor_FB_openLoop, duty_FB,
                  (rif_FB_r >= 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
        Error_Handler();
    }

    if (motor_set(&motor_BA_openLoop, duty_BA,
                  (rif_BA_r >= 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
        Error_Handler();
    }

    if (motor_set(&motor_BB_openLoop, duty_BB,
                  (rif_BB_r >= 0) ? CLOCKWISE : COUNTERCLOCKWISE) != MOTOR_OK) {
        Error_Handler();
    }
}

void deadlineProcedure(){

	__disable_irq(); // Disabilitazione di ogni ulteriore Interrupt

	// ARRESTO DEL ROVER IN CONDIZIONI DI SICUREZZA

    // Disabilita il relè
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);

    // Ferma tutti i PWM dei motori
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);

    while(1) {
    		    // Blocco del sistema in condizioni di sicurezza
    }
}
/* USER CODE END Application */

