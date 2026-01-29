/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2.c
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 1.2285
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Jan 29 12:45:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Board2.h"
#include "rtwtypes.h"
#include "Board2_types.h"
#include <math.h>

/* Block states (default storage) */
DW_Board2_T Board2_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Board2_T Board2_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Board2_T Board2_Y;

/* Real-time model */
static RT_MODEL_Board2_T Board2_M_;
RT_MODEL_Board2_T *const Board2_M = &Board2_M_;

/* Forward declaration for local functions */
static void Board2_Disable_MUX(void);
static void Board2_Update_Local_State(void);
static void Board2_Open_Session(void);
static void Board2_Raise_MTalk(void);
static uint32_T Board2_Get_Timestamp(void);
static void Board2_Rover_Lights_OFF(void);
static void Board2_enter_internal_Normal(void);
static boolean_T Board2_Is_STalk_High(void);
static boolean_T Board2_Check_Timeout_Us(uint32_T b_start_time, uint32_T
  b_min_elapsed_time);
static void Board2_Abort_Communication(void);
static void Board2_Close_Session(void);
static void Board2_Lower_MTalk(void);
static void Board2_Send_Ping(void);
static void Board2_Wait_Ping(void);
static boolean_T Board2_Is_Rx_Finished(void);
static boolean_T Board2_Verify_Ping(void);
static boolean_T Board2_Is_Tx_Finished(void);
static void Board2_Compute_Degraded_Actions(void);
static void Bo_exit_internal_Normal_voltage(void);
static void Board2_exit_internal_Normal(void);
static void Board2_Enable_MUX(void);
static void Board2_Send_Decision(void);
static void Board2_Wait_Decision(void);
static void Board2_Send_Global_State(void);
static void Board2_Wait_Global_State(void);
static void Board2_Send_Local_State(void);
static void Board2_Wait_State(void);
static void Board2_Local_state_transmitted(void);
static boolean_T Board_Verify_Decision_Integrity(void);
static void Board2_Write_Output(void);
static void Board2_Receive_decision(void);
static boolean_T Board2_Verify_Global_Integrity(void);
static boolean_T Board2_Critical_Voltage(void);
static boolean_T Board2_Is_Rover_Stationary(void);
static boolean_T Board2_Emergency_B_Pressed(void);
static void Board2_Turn_Left(void);
static void Board2_Turn_Right(void);
static void Board2_Update_Angle(real32_T yaw);
static void Board2_Stop_Motors(void);
static void Board2_Emergency_sonar_routine(void);
static boolean_T Board2_Near_Obstacle(void);
static boolean_T Board2_Stop_B_Pressed(void);
static void Board2_Process_Evasive_Commands(void);
static boolean_T Board2_Is_Rover_Moving_Forward(void);
static boolean_T Board2_Emergency_S_Routine(void);
static boolean_T Board2_Mov_Obs_R_Routine(void);
static boolean_T Board2_Mov_Obs_L_Routine(void);
static boolean_T Board2_Stop_Slow_Routine(void);
static boolean_T Low_Controller_Battery_Routine(void);
static boolean_T Board2_Spec_Retro_Routine(void);
static boolean_T Board2_Stationary_Obs_Routine(void);
static void Board2_Process_User_Commands(void);
static void Board2_Turn_Back(void);
static void Board2_Select_routine(void);
static boolean_T Board2_Mov_Obs_Right(void);
static boolean_T Board2_Mov_Obs_Left(void);
static void Board2_Stop_slow_routine(void);
static void Board2_Normal_voltage(void);
static boolean_T Board2_Mode_B_Pressed(void);
static boolean_T Board2_Lights_B_Pressed(void);
static void Board2_Update_Rover_Lights(boolean_T white_led_when_stopped);
static void Board2_Actions(void);
static boolean_T Board2_isequal_l(MOVING_OBSTACLE_TYPE varargin_1,
  MOVING_OBSTACLE_TYPE varargin_2);
static boolean_T Board2_isequal(real32_T varargin_1_stateB1_battery_volt,
  real32_T varargin_1_stateB1_temperature, int16_T
  varargin_1_stateB1_velocity_FA, int16_T varargin_1_stateB1_velocity_FB,
  int16_T varargin_1_stateB1_velocity_BA, int16_T varargin_1_stateB1_velocity_BB,
  const StateBusB2 varargin_1_stateB2, MOVING_OBSTACLE_TYPE varargin_1_mov_obs,
  boolean_T varargin_1_spc_retro, uint8_T varargin_1_max_vel, boolean_T
  varargin_1_obs_detection, const StateBusB1 varargin_2_stateB1, const
  StateBusB2 varargin_2_stateB2, MOVING_OBSTACLE_TYPE varargin_2_mov_obs,
  boolean_T varargin_2_spc_retro, uint8_T varargin_2_max_vel, boolean_T
  varargin_2_obs_detection);
static void Board2_Receive_global_state(void);
static boolean_T Board2_Verify_State_Integrity(void);
static boolean_T Board2_Obs_Sonar2(void);
static boolean_T Board2_Check_Timeout_Ms(uint32_T b_start_time, uint32_T
  b_min_elapsed_time);
static boolean_T Board2_Obs_Sonar1(void);
static boolean_T Board2_Obs_Sonar3(void);
static void Board2_Moving_obstacle(void);
static boolean_T Board2_Low_Voltage(void);
static boolean_T Board2_High_Temp(void);
static void Boa_Battery_temperature_manager(void);
static boolean_T Board2_Button2_Pressed_Retro(void);
static boolean_T Board2_Button1_Pressed_Retro(void);
static boolean_T Board2_Button2_Pressed_Obs(void);
static boolean_T Board2_Button1_Pressed_Obs(void);
static boolean_T Board2_Detect_Limit_Activation(void);
static boolean_T Board2_Button1_Pressed_Vel(void);
static boolean_T Board2_L_Stick_Up(void);
static boolean_T Board2_L_Stick_Down(void);
static void Board2_Decrease_Max_Vel(void);
static void Board2_Increase_Max_Vel(void);
static void Board2_Combo(void);
static void Board2_Update_Global_State(void);
static void Board2_Receive_state(void);
static void Board2_Supervisor(void);
static void Board2_Init_Data_Structures(void);

/* Function for Chart: '<Root>/Board2' */
static void Board2_Disable_MUX(void)
{
  IO_Disable_MUX();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Update_Local_State(void)
{
  OS_Enter_Critical();

  /* Inport: '<Root>/acceleration_y' */
  Board2_DW.state.acceleration_y = Board2_U.acceleration_y;

  /* Inport: '<Root>/acceleration_x' */
  Board2_DW.state.acceleration_x = Board2_U.acceleration_x;

  /* Inport: '<Root>/gyroYaw' */
  Board2_DW.state.gyroYaw = Board2_U.gyroYaw;

  /* Inport: '<Root>/sonar1' */
  Board2_DW.state.sonar1 = Board2_U.sonar1;

  /* Inport: '<Root>/sonar2' */
  Board2_DW.state.sonar2 = Board2_U.sonar2;

  /* Inport: '<Root>/sonar3' */
  Board2_DW.state.sonar3 = Board2_U.sonar3;

  /* Inport: '<Root>/controller_y' */
  Board2_DW.state.controller_y = Board2_U.controller_y;

  /* Inport: '<Root>/controller_x' */
  Board2_DW.state.controller_x = Board2_U.controller_x;

  /* Inport: '<Root>/B1' */
  Board2_DW.state.button1 = Board2_U.B1;

  /* Inport: '<Root>/B2' */
  Board2_DW.state.button2 = Board2_U.B2;

  /* Inport: '<Root>/B3' */
  Board2_DW.state.button3 = Board2_U.B3;

  /* Inport: '<Root>/B4' */
  Board2_DW.state.button4 = Board2_U.B4;

  /* Inport: '<Root>/B_r_stick' */
  Board2_DW.state.r_stick_button = Board2_U.B_r_stick;

  /* Inport: '<Root>/B_l_stick' */
  Board2_DW.state.l_stick_button = Board2_U.B_l_stick;

  /* Inport: '<Root>/controller_battery' */
  Board2_DW.state.controller_battery = Board2_U.controller_battery;
  OS_Exit_Critical();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Open_Session(void)
{
  IO_Set_Session();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Raise_MTalk(void)
{
  IO_Set_MasterTalk();
}

/* Function for Chart: '<Root>/Board2' */
static uint32_T Board2_Get_Timestamp(void)
{
  return Time_Get_Tick();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Rover_Lights_OFF(void)
{
  Board2_DW.decision.led_A = OFF;
  Board2_DW.decision.led_B = OFF;
  Board2_DW.decision.rear_led = IDLE;
  Board2_DW.decision.rear_sign = SIGN_OFF;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_enter_internal_Normal(void)
{
  Board2_DW.is_active_Supervisor = 1U;
  Board2_DW.is_Supervisor = Board2_IN_Waiting_comunication;
  Board2_Update_Local_State();
  Board2_Open_Session();
  Board2_Raise_MTalk();
  Board2_DW.time_comm = Board2_Get_Timestamp();
  Board2_DW.is_active_Moving_obstacle = 1U;
  Board2_DW.is_active_No_obstacle = 1U;
  Board2_DW.is_No_obstacle = Board2_IN_No_movements;
  Board2_DW.moving_obstacle = NO_OBSTACLE;
  Board2_DW.is_active_Obstacle_from_left = 1U;
  Board2_DW.is_Obstacle_from_left = Board2_IN_Waiting;
  Board2_DW.is_active_Obstacle_from_right = 1U;
  Board2_DW.is_Obstacle_from_right = Board2_IN_Waiting;
  Board2_DW.is_active_Battery_temperature_m = 1U;
  Board2_DW.is_active_Normal_velocity = 1U;
  Board2_DW.is_Normal_velocity = Board2_IN_No_limitation;
  Board2_DW.limit_velocity = false;
  Board2_DW.is_active_Battery_manager = 1U;
  Board2_DW.is_Battery_manager = Board2_IN_Normal;
  Board2_DW.is_active_Temperature_manager = 1U;
  Board2_DW.is_Temperature_manager = Board2_IN_Normal_p;
  Board2_DW.is_active_Combo = 1U;
  Board2_DW.is_active_Special_retro = 1U;
  Board2_DW.special_retro = true;
  Board2_DW.is_Special_retro = Board2_IN_Special_retro_start;
  Board2_DW.is_active_Obstacle_detection = 1U;
  Board2_DW.obs_detection = true;
  Board2_DW.is_Obstacle_detection = Board2_IN_Special_retro_start;
  Board2_DW.is_active_Change_max_velocity = 1U;
  Board2_DW.max_vel = Board2_MAX_RPM;
  Board2_DW.is_Change_max_velocity = Board_IN_Manager_combo_velocity;
  Board2_DW.is_Manager_combo_velocity = Bo_IN_Change_max_velocity_start;
  Board2_DW.is_active_Actions = 1U;
  Board2_DW.is_active_Routine_manager = 1U;
  Board2_DW.is_Routine_manager = Board2_IN_Normal_voltage;
  Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
  Board2_DW.is_active_Mode_manager = 1U;
  Board2_DW.is_Mode_manager = Board2_IN_Normal_voltage;
  Board2_DW.is_Normal_voltage_m = Board2_IN_Mode_DEFAULT;
  Board2_DW.decision.mode = DEFAULT;
  Board2_DW.is_active_Lights_manager = 1U;
  Board2_DW.is_Lights_manager = Board2_IN_Normal_voltage;
  Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_OFF;
  Board2_Rover_Lights_OFF();
  Board2_DW.is_active_Relay_manager = 1U;
  Board2_DW.is_Relay_manager = Board2_IN_Normal_voltage;
  Board2_DW.decision.relay = true;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Is_STalk_High(void)
{
  return IO_Read_SlaveTalk();
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Check_Timeout_Us(uint32_T b_start_time, uint32_T
  b_min_elapsed_time)
{
  return Time_Check_Elapsed_us(b_start_time, b_min_elapsed_time);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Abort_Communication(void)
{
  UART_Stop_DMA();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Close_Session(void)
{
  IO_Reset_Session();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Lower_MTalk(void)
{
  IO_Reset_MasterTalk();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Send_Ping(void)
{
  UART_Send_Ping();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Wait_Ping(void)
{
  UART_Wait_Ping(&Board2_DW.receivedPing);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Is_Rx_Finished(void)
{
  return UART_Is_Rx_Complete();
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Verify_Ping(void)
{
  return UART_Check_Ping(Board2_DW.receivedPing);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Is_Tx_Finished(void)
{
  return UART_Is_Tx_Complete();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Compute_Degraded_Actions(void)
{
  int32_T tmp;
  real32_T forward;
  real32_T throttle;
  real32_T turn;
  OS_Enter_Critical();

  /* Inport: '<Root>/sonar1' incorporates:
   *  Inport: '<Root>/B3'
   *  Inport: '<Root>/B4'
   *  Inport: '<Root>/controller_battery'
   *  Inport: '<Root>/controller_y'
   *  Inport: '<Root>/sonar2'
   *  Inport: '<Root>/sonar3'
   */
  if (((Board2_U.sonar1 < Board2_MIN_DISTANCE) || (Board2_U.sonar2 <
        Board2_MAX_DISTANCE) || (Board2_U.sonar3 < Board2_MIN_DISTANCE)) &&
      (Board2_U.controller_y > Board2_CONTROLLER_ZERO)) {
    /* Outport: '<Root>/output' */
    Board2_Y.output.rif_FA = 0.0F;
    Board2_Y.output.rif_FB = 0.0F;
    Board2_Y.output.rif_BA = 0.0F;
    Board2_Y.output.rif_BB = 0.0F;
    Board2_Y.output.brk_mode = EMERGENCY;
    Board2_Y.output.led_A = OFF;
    Board2_Y.output.led_B = OFF;
    Board2_Y.output.rear_led = IDLE;
    Board2_Y.output.rear_sign = SIGN_OFF;
    Board2_Y.output.relay = true;
    OS_Exit_Critical();
  } else if (Board2_U.B4) {
    /* Outport: '<Root>/output' */
    Board2_Y.output.rif_FA = 0.0F;
    Board2_Y.output.rif_FB = 0.0F;
    Board2_Y.output.rif_BA = 0.0F;
    Board2_Y.output.rif_BB = 0.0F;
    Board2_Y.output.brk_mode = EMERGENCY;
    Board2_Y.output.led_A = OFF;
    Board2_Y.output.led_B = OFF;
    Board2_Y.output.rear_led = IDLE;
    Board2_Y.output.rear_sign = SIGN_OFF;
    Board2_Y.output.relay = true;
    OS_Exit_Critical();
  } else if (Board2_U.B3 || (Board2_U.controller_battery <=
              Board2_LOW_CONTROLLER_BATTERY)) {
    /* Outport: '<Root>/output' */
    Board2_Y.output.rif_FA = 0.0F;
    Board2_Y.output.rif_FB = 0.0F;
    Board2_Y.output.rif_BA = 0.0F;
    Board2_Y.output.rif_BB = 0.0F;
    Board2_Y.output.brk_mode = NORMAL;
    Board2_Y.output.led_A = OFF;
    Board2_Y.output.led_B = OFF;
    Board2_Y.output.rear_led = IDLE;
    Board2_Y.output.rear_sign = SIGN_OFF;
    Board2_Y.output.relay = true;
    OS_Exit_Critical();
  } else {
    throttle = ((real32_T)Board2_U.controller_y - Board2_CENTER) / Board2_CENTER;
    forward = throttle * 80.0F;

    /* Inport: '<Root>/controller_x' */
    turn = ((real32_T)Board2_U.controller_x - Board2_CENTER) / Board2_CENTER *
      80.0F;
    if (fabsf(throttle) < Board2_PURE_TURN_EPS) {
      forward = 0.0F;
    } else {
      throttle = fabsf(forward) * Board2_TURN_RATIO;
      if (fabsf(turn) > throttle) {
        if (turn < 0.0F) {
          tmp = -1;
        } else {
          tmp = (turn > 0.0F);
        }

        turn = (real32_T)tmp * throttle;
      }
    }

    throttle = forward + turn;
    forward -= turn;
    turn = fmaxf(fabsf(throttle), fabsf(forward));
    if (turn > 80.0F) {
      turn = 80.0F / turn;
      throttle *= turn;
      forward *= turn;
    }

    /* Outport: '<Root>/output' */
    Board2_Y.output.rif_FA = throttle;
    Board2_Y.output.rif_FB = forward;
    Board2_Y.output.rif_BA = throttle;
    Board2_Y.output.rif_BB = forward;
    Board2_Y.output.brk_mode = NONE;
    Board2_Y.output.led_A = OFF;
    Board2_Y.output.led_B = OFF;
    Board2_Y.output.rear_led = IDLE;
    Board2_Y.output.rear_sign = SIGN_OFF;
    Board2_Y.output.relay = true;
    OS_Exit_Critical();
  }

  /* End of Inport: '<Root>/sonar1' */
}

/* Function for Chart: '<Root>/Board2' */
static void Bo_exit_internal_Normal_voltage(void)
{
  Board2_DW.is_Control_controller_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Emergency_button_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Emergency_sonar_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Low_controller_battery_routi = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Moving_obstacle_from_left_ro = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Moving_obstacle_from_right_r = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Not_moving_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Special_retro_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Normal_voltage = Board2_IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_exit_internal_Normal(void)
{
  Board2_DW.is_Relay_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Relay_manager = 0U;
  Board2_DW.is_Normal_voltage_h = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Lights_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Lights_manager = 0U;
  Board2_DW.is_Normal_voltage_m = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Mode_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Mode_manager = 0U;
  Bo_exit_internal_Normal_voltage();
  Board2_DW.is_Routine_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Routine_manager = 0U;
  Board2_DW.is_active_Actions = 0U;
  Board2_DW.is_Manager_combo_velocity = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_Change_max_velocity = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Change_max_velocity = 0U;
  Board2_DW.is_Obstacle_detection = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Obstacle_detection = 0U;
  Board2_DW.is_Special_retro = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Special_retro = 0U;
  Board2_DW.is_active_Combo = 0U;
  Board2_DW.is_Temperature_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Temperature_manager = 0U;
  Board2_DW.is_Battery_manager = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Battery_manager = 0U;
  Board2_DW.is_Normal_velocity = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Normal_velocity = 0U;
  Board2_DW.is_active_Battery_temperature_m = 0U;
  Board2_DW.is_Obstacle_from_right = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Obstacle_from_right = 0U;
  Board2_DW.is_Obstacle_from_left = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Obstacle_from_left = 0U;
  Board2_DW.is_No_obstacle = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_No_obstacle = 0U;
  Board2_DW.is_active_Moving_obstacle = 0U;
  Board2_DW.is_Supervisor = Board2_IN_NO_ACTIVE_CHILD;
  Board2_DW.is_active_Supervisor = 0U;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Enable_MUX(void)
{
  IO_Enable_MUX();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Send_Decision(void)
{
  UART_Send_Decision(&Board2_DW.decision);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Wait_Decision(void)
{
  UART_Wait_Decision(&Board2_DW.receivedDecisionPacket);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Send_Global_State(void)
{
  UART_Send_GlobalState(&Board2_DW.global_state);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Wait_Global_State(void)
{
  UART_Wait_GlobalState(&Board2_DW.receivedGlobalStatePacket);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Send_Local_State(void)
{
  UART_Send_Local_State(&Board2_DW.state);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Wait_State(void)
{
  UART_Wait_State(&Board2_DW.receivedStatePacket);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Local_state_transmitted(void)
{
  boolean_T b;
  b = Board2_Is_STalk_High();
  if (b) {
    Board2_DW.is_Supervisor = Board2_IN_Receive_state;
    Board2_Wait_State();
    Board2_Lower_MTalk();
    Board2_DW.time_comm = Board2_Get_Timestamp();
  } else {
    b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
    if (b) {
      if (Board2_DW.retransmitted < Board2_MAX_RETRANSMIT) {
        Board2_DW.retransmitted = 1U;
        Board2_DW.is_Supervisor = Board2_IN_Transmit_Local_State;
        Board2_Send_Local_State();
        Board2_DW.time_comm = Board2_Get_Timestamp();
      } else {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board_Verify_Decision_Integrity(void)
{
  return CRC_Check_Decision(&Board2_DW.receivedDecisionPacket);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Write_Output(void)
{
  OS_Enter_Critical();

  /* Outport: '<Root>/output' */
  Board2_Y.output = Board2_DW.decision;
  OS_Exit_Critical();
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Receive_decision(void)
{
  boolean_T b;
  boolean_T d_p;
  b = Board2_Is_Rx_Finished();
  if (b) {
    b = Board_Verify_Decision_Integrity();
    if (b) {
      Board2_Close_Session();
      b = false;
      if (Board2_DW.decision.relay ==
          Board2_DW.receivedDecisionPacket.decision.relay) {
        d_p = false;
        if ((int32_T)Board2_DW.decision.mode == (int32_T)
            Board2_DW.receivedDecisionPacket.decision.mode) {
          d_p = true;
        }

        if (d_p) {
          d_p = false;
          if ((int32_T)Board2_DW.decision.rear_sign == (int32_T)
              Board2_DW.receivedDecisionPacket.decision.rear_sign) {
            d_p = true;
          }

          if (d_p) {
            d_p = false;
            if ((int32_T)Board2_DW.decision.rear_led == (int32_T)
                Board2_DW.receivedDecisionPacket.decision.rear_led) {
              d_p = true;
            }

            if (d_p) {
              d_p = false;
              if ((int32_T)Board2_DW.decision.led_B == (int32_T)
                  Board2_DW.receivedDecisionPacket.decision.led_B) {
                d_p = true;
              }

              if (d_p) {
                d_p = false;
                if ((int32_T)Board2_DW.decision.led_A == (int32_T)
                    Board2_DW.receivedDecisionPacket.decision.led_A) {
                  d_p = true;
                }

                if (d_p) {
                  d_p = false;
                  if ((int32_T)Board2_DW.decision.brk_mode == (int32_T)
                      Board2_DW.receivedDecisionPacket.decision.brk_mode) {
                    d_p = true;
                  }

                  if (d_p) {
                    if (Board2_DW.decision.rif_BB ==
                        Board2_DW.receivedDecisionPacket.decision.rif_BB) {
                      if (Board2_DW.decision.rif_BA ==
                          Board2_DW.receivedDecisionPacket.decision.rif_BA) {
                        d_p = ((Board2_DW.decision.rif_FB ==
                                Board2_DW.receivedDecisionPacket.decision.rif_FB)
                               && (Board2_DW.decision.rif_FA ==
                                   Board2_DW.receivedDecisionPacket.decision.rif_FA));
                      } else {
                        d_p = false;
                      }
                    } else {
                      d_p = false;
                    }
                  } else {
                    d_p = false;
                  }
                } else {
                  d_p = false;
                }
              } else {
                d_p = false;
              }
            } else {
              d_p = false;
            }
          } else {
            d_p = false;
          }
        } else {
          d_p = false;
        }
      } else {
        d_p = false;
      }

      if (d_p) {
        b = true;
      }

      if (b) {
        Board2_DW.is_Supervisor = Board2_IN_Same_decision;
        Board2_Write_Output();
      } else {
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Single_Board;
        Board2_DW.is_Single_Board = Board2_IN_Other_board_failure;
        Board2_Compute_Degraded_Actions();
      }
    } else {
      Board2_DW.is_Supervisor = Board2_IN_Receive_decision;
      Board2_Wait_Decision();
      Board2_Lower_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    }
  } else {
    b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
      Board2_DECISION_RECEIVE_TIMEOUT);
    if (b) {
      Board2_Compute_Degraded_Actions();
      Board2_exit_internal_Normal();
      Board2_Enable_MUX();
      Board2_Close_Session();
      Board2_Lower_MTalk();
      Board2_Abort_Communication();
      Board2_DW.is_Supervision_task = Board2_IN_Degraded;
      Board2_DW.is_Degraded = Board2_IN_Restarting;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Verify_Global_Integrity(void)
{
  return CRC_Check_GlobalState(&Board2_DW.receivedGlobalStatePacket);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Critical_Voltage(void)
{
  return Board2_DW.global_state.stateB1.battery_voltage <=
    Board2_CRITICAL_VOLTAGE;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Is_Rover_Stationary(void)
{
  int32_T b_k;
  int16_T b_y;
  boolean_T b_x[4];
  boolean_T exitg1;
  boolean_T y;
  if (Board2_DW.global_state.stateB1.velocity_FA < 0) {
    b_k = -Board2_DW.global_state.stateB1.velocity_FA;
    if (-Board2_DW.global_state.stateB1.velocity_FA > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board2_DW.global_state.stateB1.velocity_FA;
  }

  b_x[0] = (b_y <= Board2_STOP_THRESHOLD);
  if (Board2_DW.global_state.stateB1.velocity_FB < 0) {
    b_k = -Board2_DW.global_state.stateB1.velocity_FB;
    if (-Board2_DW.global_state.stateB1.velocity_FB > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board2_DW.global_state.stateB1.velocity_FB;
  }

  b_x[1] = (b_y <= Board2_STOP_THRESHOLD);
  if (Board2_DW.global_state.stateB1.velocity_BA < 0) {
    b_k = -Board2_DW.global_state.stateB1.velocity_BA;
    if (-Board2_DW.global_state.stateB1.velocity_BA > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board2_DW.global_state.stateB1.velocity_BA;
  }

  b_x[2] = (b_y <= Board2_STOP_THRESHOLD);
  if (Board2_DW.global_state.stateB1.velocity_BB < 0) {
    b_k = -Board2_DW.global_state.stateB1.velocity_BB;
    if (-Board2_DW.global_state.stateB1.velocity_BB > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board2_DW.global_state.stateB1.velocity_BB;
  }

  b_x[3] = (b_y <= Board2_STOP_THRESHOLD);
  y = true;
  b_k = 0;
  exitg1 = false;
  while ((!exitg1) && (b_k < 4)) {
    if (!b_x[b_k]) {
      y = false;
      exitg1 = true;
    } else {
      b_k++;
    }
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Emergency_B_Pressed(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    Board2_DW.global_state.stateB2.button4 &&
    (Board2_DW.global_state.stateB2.controller_battery >
     Board2_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Turn_Left(void)
{
  Board2_DW.decision.rif_FA = -20.0F;
  Board2_DW.decision.rif_FB = Board2_TURN_RPM;
  Board2_DW.decision.rif_BA = -20.0F;
  Board2_DW.decision.rif_BB = Board2_TURN_RPM;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Turn_Right(void)
{
  Board2_DW.decision.rif_FA = Board2_TURN_RPM;
  Board2_DW.decision.rif_FB = -20.0F;
  Board2_DW.decision.rif_BA = Board2_TURN_RPM;
  Board2_DW.decision.rif_BB = -20.0F;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Update_Angle(real32_T yaw)
{
  Board2_DW.angle += (yaw + Board2_DW.prevYaw) * 0.5F * Board2_PERIOD;
  Board2_DW.prevYaw = yaw;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Stop_Motors(void)
{
  Board2_DW.decision.rif_FA = 0.0F;
  Board2_DW.decision.rif_FB = 0.0F;
  Board2_DW.decision.rif_BA = 0.0F;
  Board2_DW.decision.rif_BB = 0.0F;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Emergency_sonar_routine(void)
{
  boolean_T b;
  b = Board2_Emergency_B_Pressed();
  if (b) {
    Board2_DW.is_Emergency_sonar_routine = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
    Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
    Board2_Stop_Motors();
    Board2_DW.decision.brk_mode = EMERGENCY;
  } else {
    switch (Board2_DW.is_Emergency_sonar_routine) {
     case Board2_IN_Emergency_sonar:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Is_Rover_Stationary();
      } else {
        b = false;
      }

      if (b) {
        if (Board2_DW.global_state.stateB2.sonar3 > Board2_DW.distance_threshold)
        {
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Turn_right;
          Board2_Turn_Right();
          Board2_DW.decision.brk_mode = NONE;
        } else if (Board2_DW.global_state.stateB2.sonar1 >
                   Board2_DW.distance_threshold) {
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Turn_left;
          Board2_Turn_Left();
          Board2_DW.decision.brk_mode = NONE;
        } else {
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
        }
      }
      break;

     case Board2_IN_Stop_left_rotation:
      Board2_DW.is_Emergency_sonar_routine = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      break;

     case Board2_IN_Stop_right_rotation:
      Board2_DW.is_Emergency_sonar_routine = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      break;

     case Board2_IN_Turn_left:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        Board2_Update_Angle(Board2_DW.global_state.stateB2.gyroYaw);
        if (fabsf(Board2_DW.angle) >= Board2_TURN_ANGLE) {
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Stop_left_rotation;
          Board2_Stop_Motors();
          Board2_DW.decision.brk_mode = NONE;
        } else {
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Turn_left;
          Board2_Turn_Left();
          Board2_DW.decision.brk_mode = NONE;
        }
      }
      break;

     case Board2_IN_Turn_right:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        Board2_Update_Angle(Board2_DW.global_state.stateB2.gyroYaw);
        if (fabsf(Board2_DW.angle) >= Board2_TURN_ANGLE) {
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Stop_right_rotation;
          Board2_Stop_Motors();
          Board2_DW.decision.brk_mode = NONE;
        } else {
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Turn_right;
          Board2_Turn_Right();
          Board2_DW.decision.brk_mode = NONE;
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Near_Obstacle(void)
{
  if (Board2_DW.global_state.obs_detection) {
    Board2_DW.distance_threshold = Board2_IMM_DISTANCE;
  } else {
    Board2_DW.distance_threshold = Board2_PROTECTION_DISTANCE;
  }

  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    ((Board2_DW.global_state.stateB2.sonar1 <= Board2_DW.distance_threshold) ||
     (Board2_DW.global_state.stateB2.sonar2 <= Board2_DW.distance_threshold) ||
     (Board2_DW.global_state.stateB2.sonar3 <= Board2_DW.distance_threshold));
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Stop_B_Pressed(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    Board2_DW.global_state.stateB2.button3 &&
    (Board2_DW.global_state.stateB2.controller_battery >
     Board2_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Process_Evasive_Commands(void)
{
  int32_T steering_dir;
  real32_T forward;
  real32_T throttle;
  real32_T turn;
  throttle = ((real32_T)Board2_DW.global_state.stateB2.controller_y -
              Board2_CENTER) / Board2_CENTER;
  switch (Board2_DW.global_state.mov_obs) {
   case MOVING_FROM_RIGHT:
    steering_dir = 1;
    break;

   case MOVING_FROM_LEFT:
    steering_dir = -1;
    break;

   default:
    steering_dir = 1;
    break;
  }

  forward = throttle * (real32_T)Board2_DW.global_state.max_vel;
  throttle = fabsf(throttle);
  turn = (0.3F * throttle + Board2_MIN_TURN_SCALE_EVASIVE) * (real32_T)
    (steering_dir * Board2_DW.global_state.max_vel);
  if (throttle >= Board2_PURE_TURN_EPS) {
    throttle = fabsf(forward) * Board2_TURN_RATIO;
    if (fabsf(turn) > throttle) {
      if (turn < 0.0F) {
        steering_dir = -1;
      } else {
        steering_dir = (turn > 0.0F);
      }

      turn = (real32_T)steering_dir * throttle;
    }
  }

  throttle = forward + turn;
  forward -= turn;
  turn = fmaxf(fabsf(throttle), fabsf(forward));
  if (turn > Board2_DW.global_state.max_vel) {
    turn = (real32_T)Board2_DW.global_state.max_vel / turn;
    throttle *= turn;
    forward *= turn;
  }

  Board2_DW.decision.rif_FA = throttle;
  Board2_DW.decision.rif_BA = throttle;
  Board2_DW.decision.rif_FB = forward;
  Board2_DW.decision.rif_BB = forward;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Is_Rover_Moving_Forward(void)
{
  int32_T k;
  boolean_T x[4];
  boolean_T exitg1;
  boolean_T y;
  x[0] = (Board2_DW.global_state.stateB1.velocity_FA > Board2_STOP_THRESHOLD);
  x[1] = (Board2_DW.global_state.stateB1.velocity_FB > Board2_STOP_THRESHOLD);
  x[2] = (Board2_DW.global_state.stateB1.velocity_BA > Board2_STOP_THRESHOLD);
  x[3] = (Board2_DW.global_state.stateB1.velocity_BB > Board2_STOP_THRESHOLD);
  y = true;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 4)) {
    if (!x[k]) {
      y = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Emergency_S_Routine(void)
{
  boolean_T y;
  if (Board2_DW.global_state.obs_detection) {
    Board2_DW.distance_threshold = Board2_IMM_DISTANCE;
  } else {
    Board2_DW.distance_threshold = Board2_PROTECTION_DISTANCE;
  }

  if ((Board2_DW.sfEvent == Board2_event_STEP) &&
      ((Board2_DW.global_state.stateB2.sonar1 <= Board2_DW.distance_threshold) ||
       (Board2_DW.global_state.stateB2.sonar2 <= Board2_DW.distance_threshold) ||
       (Board2_DW.global_state.stateB2.sonar3 <= Board2_DW.distance_threshold)))
  {
    y = Board2_Is_Rover_Moving_Forward();
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Mov_Obs_R_Routine(void)
{
  boolean_T y;
  if ((Board2_DW.sfEvent == Board2_event_STEP) &&
      (Board2_DW.global_state.mov_obs == MOVING_FROM_RIGHT)) {
    y = (Board2_Is_Rover_Moving_Forward() &&
         Board2_DW.global_state.obs_detection);
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Mov_Obs_L_Routine(void)
{
  boolean_T y;
  if ((Board2_DW.sfEvent == Board2_event_STEP) &&
      (Board2_DW.global_state.mov_obs == MOVING_FROM_LEFT)) {
    y = (Board2_Is_Rover_Moving_Forward() &&
         Board2_DW.global_state.obs_detection);
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Stop_Slow_Routine(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent == Board2_event_STEP) {
    if ((Board2_DW.global_state.stateB2.sonar2 <= Board2_MAX_DISTANCE) &&
        Board2_Is_Rover_Moving_Forward() && Board2_DW.global_state.obs_detection)
    {
      y = true;
    } else {
      y = (Board2_DW.global_state.stateB2.button3 &&
           (Board2_DW.global_state.stateB2.controller_battery >
            Board2_LOW_CONTROLLER_BATTERY));
    }
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Low_Controller_Battery_Routine(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.global_state.stateB2.controller_battery <=
     Board2_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Spec_Retro_Routine(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.global_state.stateB2.controller_y < Board2_CONTROLLER_ZERO) &&
    Board2_DW.global_state.spc_retro;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Stationary_Obs_Routine(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent == Board2_event_STEP) {
    y = (Board2_Is_Rover_Stationary() && ((Board2_DW.global_state.stateB2.sonar2
           <= Board2_MAX_DISTANCE) &&
          (Board2_DW.global_state.stateB2.controller_y > Board2_CONTROLLER_ZERO)
          && Board2_DW.global_state.obs_detection));
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Process_User_Commands(void)
{
  real32_T forward;
  real32_T throttle;
  real32_T turn;
  throttle = ((real32_T)Board2_DW.global_state.stateB2.controller_y -
              Board2_CENTER) / Board2_CENTER;
  forward = throttle * (real32_T)Board2_DW.global_state.max_vel;
  turn = ((real32_T)Board2_DW.global_state.stateB2.controller_x - Board2_CENTER)
    / Board2_CENTER * (real32_T)Board2_DW.global_state.max_vel;
  if (fabsf(throttle) < Board2_PURE_TURN_EPS) {
    forward = 0.0F;
  } else {
    throttle = fabsf(forward) * Board2_TURN_RATIO;
    if (fabsf(turn) > throttle) {
      int32_T tmp;
      if (turn < 0.0F) {
        tmp = -1;
      } else {
        tmp = (turn > 0.0F);
      }

      turn = (real32_T)tmp * throttle;
    }
  }

  throttle = forward + turn;
  forward -= turn;
  turn = fmaxf(fabsf(throttle), fabsf(forward));
  if (turn > Board2_DW.global_state.max_vel) {
    turn = (real32_T)Board2_DW.global_state.max_vel / turn;
    throttle *= turn;
    forward *= turn;
  }

  Board2_DW.decision.rif_FA = throttle;
  Board2_DW.decision.rif_BA = throttle;
  Board2_DW.decision.rif_FB = forward;
  Board2_DW.decision.rif_BB = forward;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Turn_Back(void)
{
  Board2_DW.decision.rif_FA = Board2_TURN_BACK_RPM;
  Board2_DW.decision.rif_FB = -40.0F;
  Board2_DW.decision.rif_BA = Board2_TURN_BACK_RPM;
  Board2_DW.decision.rif_BB = -40.0F;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Select_routine(void)
{
  boolean_T b;
  b = Board2_Emergency_B_Pressed();
  if (b) {
    Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
    Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
    Board2_Stop_Motors();
    Board2_DW.decision.brk_mode = EMERGENCY;
  } else {
    b = Board2_Emergency_S_Routine();
    if (b) {
      Board2_DW.is_Normal_voltage = Boar_IN_Emergency_sonar_routine;
      Board2_DW.angle = 0.0F;
      Board2_DW.prevYaw = 0.0F;
      Board2_DW.is_Emergency_sonar_routine = Board2_IN_Emergency_sonar;
      Board2_Stop_Motors();
      Board2_DW.decision.brk_mode = EMERGENCY;
    } else {
      b = Board2_Mov_Obs_R_Routine();
      if (b) {
        Board2_DW.is_Normal_voltage = IN_Moving_obstacle_from_right_r;
        Board2_DW.angle = 0.0F;
        Board2_DW.prevYaw = 0.0F;
        Board2_DW.is_Moving_obstacle_from_right_r = Board2_IN_Turn_right_j;
        Board2_Process_Evasive_Commands();
        Board2_DW.decision.brk_mode = NONE;
      } else {
        b = Board2_Mov_Obs_L_Routine();
        if (b) {
          Board2_DW.is_Normal_voltage = IN_Moving_obstacle_from_left_ro;
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Moving_obstacle_from_left_ro = Board2_IN_Turn_left_k;
          Board2_Process_Evasive_Commands();
          Board2_DW.decision.brk_mode = NONE;
        } else {
          b = Board2_Stop_Slow_Routine();
          if (b) {
            Board2_DW.is_Normal_voltage = Board2_IN_Stop_slow_routine;
            Board2_DW.is_Stop_slow_routine = Board2_IN_Stop_slow;
            Board2_Stop_Motors();
            Board2_DW.decision.brk_mode = NORMAL;
          } else {
            b = Low_Controller_Battery_Routine();
            if (b) {
              Board2_DW.is_Normal_voltage = IN_Low_controller_battery_routi;
              Board2_DW.is_Low_controller_battery_routi =
                Board2_IN_Control_battery_stop;
              Board2_Stop_Motors();
              Board2_DW.decision.brk_mode = NORMAL;
            } else {
              b = Board2_Spec_Retro_Routine();
              if (b) {
                Board2_DW.is_Normal_voltage = Board2_IN_Special_retro_routine;
                Board2_DW.angle = 0.0F;
                Board2_DW.prevYaw = 0.0F;
                Board2_DW.is_Special_retro_routine = Board2_IN_Turn_back;
                Board2_Turn_Back();
                Board2_DW.decision.brk_mode = NONE;
              } else {
                b = Board2_Stationary_Obs_Routine();
                if (b) {
                  Board2_DW.is_Normal_voltage = Board2_IN_Not_moving_routine;
                  Board2_DW.is_Not_moving_routine = Board2_IN_Not_moving;
                  Board2_Stop_Motors();
                  Board2_DW.decision.brk_mode = NORMAL;
                } else if (Board2_DW.sfEvent == Board2_event_STEP) {
                  Board2_DW.is_Normal_voltage = B_IN_Control_controller_routine;
                  Board2_DW.is_Control_controller_routine =
                    Boar_IN_Control_from_controller;
                  Board2_Process_User_Commands();
                  Board2_DW.decision.brk_mode = NONE;
                }
              }
            }
          }
        }
      }
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Mov_Obs_Right(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.global_state.mov_obs == MOVING_FROM_RIGHT) &&
    Board2_DW.global_state.obs_detection;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Mov_Obs_Left(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.global_state.mov_obs == MOVING_FROM_LEFT) &&
    Board2_DW.global_state.obs_detection;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Stop_slow_routine(void)
{
  boolean_T b;
  b = Board2_Emergency_B_Pressed();
  if (b) {
    Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
    Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
    Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
    Board2_Stop_Motors();
    Board2_DW.decision.brk_mode = EMERGENCY;
  } else {
    b = Board2_Near_Obstacle();
    if (b) {
      Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
      Board2_DW.is_Normal_voltage = Boar_IN_Emergency_sonar_routine;
      Board2_DW.angle = 0.0F;
      Board2_DW.prevYaw = 0.0F;
      Board2_DW.is_Emergency_sonar_routine = Board2_IN_Emergency_sonar;
      Board2_Stop_Motors();
      Board2_DW.decision.brk_mode = EMERGENCY;
    } else {
      b = Board2_Mov_Obs_Right();
      if (b) {
        Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = IN_Moving_obstacle_from_right_r;
        Board2_DW.angle = 0.0F;
        Board2_DW.prevYaw = 0.0F;
        Board2_DW.is_Moving_obstacle_from_right_r = Board2_IN_Turn_right_j;
        Board2_Process_Evasive_Commands();
        Board2_DW.decision.brk_mode = NONE;
      } else {
        b = Board2_Mov_Obs_Left();
        if (b) {
          Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = IN_Moving_obstacle_from_left_ro;
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Moving_obstacle_from_left_ro = Board2_IN_Turn_left_k;
          Board2_Process_Evasive_Commands();
          Board2_DW.decision.brk_mode = NONE;
        } else if (Board2_DW.is_Stop_slow_routine == Board2_IN_Stop_slow) {
          b = Board2_Is_Rover_Stationary();
          if (b) {
            Board2_DW.is_Stop_slow_routine = Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
          }
        }
      }
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Normal_voltage(void)
{
  boolean_T b;
  if (Board2_DW.sfEvent == Board2_event_STEP) {
    b = Board2_Critical_Voltage();
  } else {
    b = false;
  }

  if (b) {
    Bo_exit_internal_Normal_voltage();
    Board2_DW.is_Routine_manager = Board2_IN_Critical_voltage;
    Board2_Stop_Motors();
    Board2_DW.decision.brk_mode = EMERGENCY;
  } else {
    switch (Board2_DW.is_Normal_voltage) {
     case B_IN_Control_controller_routine:
      if (Board2_DW.is_Control_controller_routine ==
          Boar_IN_Control_from_controller) {
        Board2_DW.is_Control_controller_routine = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      }
      break;

     case Boa_IN_Emergency_button_routine:
      if (Board2_DW.is_Emergency_button_routine == Board2_IN_Emergency_button) {
        b = Board2_Is_Rover_Stationary();
        if (b) {
          Board2_DW.is_Emergency_button_routine = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
        }
      }
      break;

     case Boar_IN_Emergency_sonar_routine:
      Board2_Emergency_sonar_routine();
      break;

     case IN_Low_controller_battery_routi:
      if (Board2_DW.is_Low_controller_battery_routi ==
          Board2_IN_Control_battery_stop) {
        Board2_DW.is_Low_controller_battery_routi = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      }
      break;

     case IN_Moving_obstacle_from_left_ro:
      b = Board2_Emergency_B_Pressed();
      if (b) {
        Board2_DW.is_Moving_obstacle_from_left_ro = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
        Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
        Board2_Stop_Motors();
        Board2_DW.decision.brk_mode = EMERGENCY;
      } else {
        b = Board2_Near_Obstacle();
        if (b) {
          Board2_DW.is_Moving_obstacle_from_left_ro = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = Boar_IN_Emergency_sonar_routine;
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Emergency_sonar;
          Board2_Stop_Motors();
          Board2_DW.decision.brk_mode = EMERGENCY;
        } else {
          b = Board2_Stop_B_Pressed();
          if (b) {
            Board2_DW.is_Moving_obstacle_from_left_ro =
              Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.is_Normal_voltage = Board2_IN_Stop_slow_routine;
            Board2_DW.is_Stop_slow_routine = Board2_IN_Stop_slow;
            Board2_Stop_Motors();
            Board2_DW.decision.brk_mode = NORMAL;
          } else if ((Board2_DW.is_Moving_obstacle_from_left_ro ==
                      Board2_IN_Turn_left_k) && (Board2_DW.sfEvent ==
                      Board2_event_STEP)) {
            Board2_Update_Angle(Board2_DW.global_state.stateB2.gyroYaw);
            if (fabsf(Board2_DW.angle) >= Board2_TURN_ANGLE) {
              Board2_DW.angle = 0.0F;
              Board2_DW.prevYaw = 0.0F;
              Board2_DW.is_Moving_obstacle_from_left_ro =
                Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
            } else {
              Board2_Process_Evasive_Commands();
              Board2_DW.decision.brk_mode = NONE;
            }
          }
        }
      }
      break;

     case IN_Moving_obstacle_from_right_r:
      b = Board2_Emergency_B_Pressed();
      if (b) {
        Board2_DW.is_Moving_obstacle_from_right_r = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
        Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
        Board2_Stop_Motors();
        Board2_DW.decision.brk_mode = EMERGENCY;
      } else {
        b = Board2_Near_Obstacle();
        if (b) {
          Board2_DW.is_Moving_obstacle_from_right_r = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = Boar_IN_Emergency_sonar_routine;
          Board2_DW.angle = 0.0F;
          Board2_DW.prevYaw = 0.0F;
          Board2_DW.is_Emergency_sonar_routine = Board2_IN_Emergency_sonar;
          Board2_Stop_Motors();
          Board2_DW.decision.brk_mode = EMERGENCY;
        } else {
          b = Board2_Stop_B_Pressed();
          if (b) {
            Board2_DW.is_Moving_obstacle_from_right_r =
              Board2_IN_NO_ACTIVE_CHILD;
            Board2_DW.is_Normal_voltage = Board2_IN_Stop_slow_routine;
            Board2_DW.is_Stop_slow_routine = Board2_IN_Stop_slow;
            Board2_Stop_Motors();
            Board2_DW.decision.brk_mode = NORMAL;
          } else if ((Board2_DW.is_Moving_obstacle_from_right_r ==
                      Board2_IN_Turn_right_j) && (Board2_DW.sfEvent ==
                      Board2_event_STEP)) {
            Board2_Update_Angle(Board2_DW.global_state.stateB2.gyroYaw);
            if (fabsf(Board2_DW.angle) >= Board2_TURN_ANGLE) {
              Board2_DW.angle = 0.0F;
              Board2_DW.prevYaw = 0.0F;
              Board2_DW.is_Moving_obstacle_from_right_r =
                Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
            } else {
              Board2_Process_Evasive_Commands();
              Board2_DW.decision.brk_mode = NONE;
            }
          }
        }
      }
      break;

     case Board2_IN_Not_moving_routine:
      if (Board2_DW.is_Not_moving_routine == Board2_IN_Not_moving) {
        Board2_DW.is_Not_moving_routine = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      }
      break;

     case Board2_IN_Select_routine:
      Board2_Select_routine();
      break;

     case Board2_IN_Special_retro_routine:
      b = Board2_Emergency_B_Pressed();
      if (b) {
        Board2_DW.is_Special_retro_routine = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Normal_voltage = Boa_IN_Emergency_button_routine;
        Board2_DW.is_Emergency_button_routine = Board2_IN_Emergency_button;
        Board2_Stop_Motors();
        Board2_DW.decision.brk_mode = EMERGENCY;
      } else {
        b = Board2_Stop_B_Pressed();
        if (b) {
          Board2_DW.is_Special_retro_routine = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Normal_voltage = Board2_IN_Stop_slow_routine;
          Board2_DW.is_Stop_slow_routine = Board2_IN_Stop_slow;
          Board2_Stop_Motors();
          Board2_DW.decision.brk_mode = NORMAL;
        } else {
          switch (Board2_DW.is_Special_retro_routine) {
           case Board2_IN_Stop_back_rotation:
            if ((Board2_DW.global_state.stateB2.controller_y >=
                 Board2_CONTROLLER_ZERO) ||
                (Board2_DW.global_state.stateB2.controller_x !=
                 Board2_CONTROLLER_ZERO)) {
              Board2_DW.is_Special_retro_routine = Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
            }
            break;

           case Board2_IN_Turn_back:
            if (Board2_DW.sfEvent == Board2_event_STEP) {
              Board2_Update_Angle(Board2_DW.global_state.stateB2.gyroYaw);
              if (fabsf(Board2_DW.angle) >= Board2_TURN_BACK_ANGLE) {
                Board2_DW.angle = 0.0F;
                Board2_DW.prevYaw = 0.0F;
                Board2_DW.is_Special_retro_routine =
                  Board2_IN_Stop_back_rotation;
                Board2_Stop_Motors();
                Board2_DW.decision.brk_mode = NONE;
              } else {
                Board2_DW.is_Special_retro_routine = Board2_IN_Turn_back;
                Board2_Turn_Back();
                Board2_DW.decision.brk_mode = NONE;
              }
            }
            break;
          }
        }
      }
      break;

     case Board2_IN_Stop_slow_routine:
      Board2_Stop_slow_routine();
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Mode_B_Pressed(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.global_state.stateB2.l_stick_button &&
         (!Board2_DW.prev_l_stick_button));
    Board2_DW.prev_l_stick_button =
      Board2_DW.global_state.stateB2.l_stick_button;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Lights_B_Pressed(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.global_state.stateB2.r_stick_button &&
         (!Board2_DW.prev_r_stick_button));
    Board2_DW.prev_r_stick_button =
      Board2_DW.global_state.stateB2.r_stick_button;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Update_Rover_Lights(boolean_T white_led_when_stopped)
{
  boolean_T x[4];
  if (Board2_DW.decision.brk_mode != NONE) {
    Board2_DW.decision.led_A = WHITE;
    Board2_DW.decision.led_B = WHITE;
    Board2_DW.decision.rear_led = BRAKING_LIGHT;
  } else if (((Board2_DW.decision.rif_FA + Board2_DW.decision.rif_BA) / 2.0F <
              0.0F) && ((Board2_DW.decision.rif_FB + Board2_DW.decision.rif_BB) /
                        2.0F > 1.0F)) {
    Board2_DW.decision.led_A = BLINKING_RED;
    Board2_DW.decision.led_B = WHITE;
    Board2_DW.decision.rear_led = ARROW_LEFT;
  } else if (((Board2_DW.decision.rif_FA + Board2_DW.decision.rif_BA) / 2.0F >
              1.0F) && ((Board2_DW.decision.rif_FB + Board2_DW.decision.rif_BB) /
                        2.0F < 0.0F)) {
    Board2_DW.decision.led_A = WHITE;
    Board2_DW.decision.led_B = BLINKING_RED;
    Board2_DW.decision.rear_led = ARROW_RIGHT;
  } else {
    real32_T left_ref;
    real32_T right_ref;
    boolean_T e_y;
    left_ref = (Board2_DW.decision.rif_FA + Board2_DW.decision.rif_BA) / 2.0F;
    right_ref = (Board2_DW.decision.rif_FB + Board2_DW.decision.rif_BB) / 2.0F;
    e_y = ((left_ref > 1.0F) && (right_ref > 1.0F));
    if (e_y && (right_ref - left_ref > Board2_DW.TURN_THRESHOLD)) {
      Board2_DW.decision.led_A = BLINKING_RED;
      Board2_DW.decision.led_B = WHITE;
      Board2_DW.decision.rear_led = ARROW_LEFT;
    } else if (e_y && ((Board2_DW.decision.rif_FA + Board2_DW.decision.rif_BA) /
                       2.0F - (Board2_DW.decision.rif_FB +
                 Board2_DW.decision.rif_BB) / 2.0F > Board2_DW.TURN_THRESHOLD))
    {
      Board2_DW.decision.led_A = WHITE;
      Board2_DW.decision.led_B = BLINKING_RED;
      Board2_DW.decision.rear_led = ARROW_RIGHT;
    } else {
      int32_T k;
      boolean_T exitg1;
      x[0] = (Board2_DW.decision.rif_FA < 0.0F);
      x[1] = (Board2_DW.decision.rif_FB < 0.0F);
      x[2] = (Board2_DW.decision.rif_BA < 0.0F);
      x[3] = (Board2_DW.decision.rif_BB < 0.0F);
      e_y = true;
      k = 0;
      exitg1 = false;
      while ((!exitg1) && (k < 4)) {
        if (!x[k]) {
          e_y = false;
          exitg1 = true;
        } else {
          k++;
        }
      }

      if (e_y) {
        Board2_DW.decision.led_A = WHITE;
        Board2_DW.decision.led_B = WHITE;
        Board2_DW.decision.rear_led = BACKWARD_LIGHTS;
      } else {
        x[0] = (Board2_DW.decision.rif_FA > 1.0F);
        x[1] = (Board2_DW.decision.rif_FB > 1.0F);
        x[2] = (Board2_DW.decision.rif_BA > 1.0F);
        x[3] = (Board2_DW.decision.rif_BB > 1.0F);
        e_y = true;
        k = 0;
        exitg1 = false;
        while ((!exitg1) && (k < 4)) {
          if (!x[k]) {
            e_y = false;
            exitg1 = true;
          } else {
            k++;
          }
        }

        if (e_y && (Board2_DW.decision.rif_FA == Board2_DW.decision.rif_FB) &&
            (Board2_DW.decision.rif_FA == Board2_DW.decision.rif_BA) &&
            (Board2_DW.decision.rif_FA == Board2_DW.decision.rif_BB)) {
          Board2_DW.decision.led_A = WHITE;
          Board2_DW.decision.led_B = WHITE;
          Board2_DW.decision.rear_led = BACKLIGHTS;
        } else {
          if (white_led_when_stopped) {
            Board2_DW.decision.led_A = WHITE;
            Board2_DW.decision.led_B = WHITE;
          } else {
            Board2_DW.decision.led_A = OFF;
            Board2_DW.decision.led_B = OFF;
          }

          Board2_DW.decision.rear_led = BACKLIGHTS;
        }
      }
    }
  }

  switch (Board2_DW.decision.mode) {
   case DEFAULT:
    Board2_DW.decision.rear_sign = SIGN_WHITE;
    break;

   case SPORT:
    Board2_DW.decision.rear_sign = SIGN_ORANGE;
    break;

   default:
    Board2_DW.decision.rear_sign = SIGN_GREEN;
    break;
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Actions(void)
{
  boolean_T b;
  if (Board2_DW.is_active_Routine_manager != 0) {
    switch (Board2_DW.is_Routine_manager) {
     case Board2_IN_Critical_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Routine_manager = Board2_IN_Normal_voltage;
        Board2_DW.is_Normal_voltage = Board2_IN_Select_routine;
      }
      break;

     case Board2_IN_Normal_voltage:
      Board2_Normal_voltage();
      break;
    }
  }

  if (Board2_DW.is_active_Mode_manager != 0) {
    switch (Board2_DW.is_Mode_manager) {
     case Board2_IN_Critical_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Mode_manager = Board2_IN_Normal_voltage;
        Board2_DW.is_Normal_voltage_m = Board2_IN_Mode_DEFAULT;
        Board2_DW.decision.mode = DEFAULT;
      }
      break;

     case Board2_IN_Normal_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Normal_voltage_m = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Mode_manager = Board2_IN_Critical_voltage;
        Board2_DW.decision.mode = ECO;
      } else {
        switch (Board2_DW.is_Normal_voltage_m) {
         case Board2_IN_Mode_DEFAULT:
          b = Board2_Mode_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_m = Board2_IN_Mode_SPORT;
            Board2_DW.decision.mode = SPORT;
          }
          break;

         case Board2_IN_Mode_ECO:
          b = Board2_Mode_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_m = Board2_IN_Mode_DEFAULT;
            Board2_DW.decision.mode = DEFAULT;
          }
          break;

         case Board2_IN_Mode_SPORT:
          b = Board2_Mode_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_m = Board2_IN_Mode_ECO;
            Board2_DW.decision.mode = ECO;
          }
          break;
        }
      }
      break;
    }
  }

  if (Board2_DW.is_active_Lights_manager != 0) {
    switch (Board2_DW.is_Lights_manager) {
     case Board2_IN_Critical_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Lights_manager = Board2_IN_Normal_voltage;
        Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_OFF;
        Board2_Rover_Lights_OFF();
      }
      break;

     case Board2_IN_Normal_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Normal_voltage_h = Board2_IN_NO_ACTIVE_CHILD;
        Board2_DW.is_Lights_manager = Board2_IN_Critical_voltage;
        Board2_Rover_Lights_OFF();
      } else {
        switch (Board2_DW.is_Normal_voltage_h) {
         case Board2_IN_Lights_AUTO:
          b = Board2_Lights_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_OFF;
            Board2_Rover_Lights_OFF();
          } else if (Board2_DW.sfEvent == Board2_event_STEP) {
            Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_AUTO;
            Board2_Update_Rover_Lights(false);
          }
          break;

         case Board2_IN_Lights_OFF:
          b = Board2_Lights_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_ON;
            Board2_Update_Rover_Lights(true);
          }
          break;

         case Board2_IN_Lights_ON:
          b = Board2_Lights_B_Pressed();
          if (b) {
            Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_AUTO;
            Board2_Update_Rover_Lights(false);
          } else if (Board2_DW.sfEvent == Board2_event_STEP) {
            Board2_DW.is_Normal_voltage_h = Board2_IN_Lights_ON;
            Board2_Update_Rover_Lights(true);
          }
          break;
        }
      }
      break;
    }
  }

  if (Board2_DW.is_active_Relay_manager != 0) {
    switch (Board2_DW.is_Relay_manager) {
     case Board2_IN_Critical_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Relay_manager = Board2_IN_Normal_voltage;
        Board2_DW.decision.relay = true;
      }
      break;

     case Board2_IN_Normal_voltage:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Critical_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Relay_manager = Board2_IN_Critical_voltage;
        Board2_DW.decision.relay = false;
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_isequal_l(MOVING_OBSTACLE_TYPE varargin_1,
  MOVING_OBSTACLE_TYPE varargin_2)
{
  boolean_T p;
  p = false;
  if ((int32_T)varargin_1 == (int32_T)varargin_2) {
    p = true;
  }

  return p;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_isequal(real32_T varargin_1_stateB1_battery_volt,
  real32_T varargin_1_stateB1_temperature, int16_T
  varargin_1_stateB1_velocity_FA, int16_T varargin_1_stateB1_velocity_FB,
  int16_T varargin_1_stateB1_velocity_BA, int16_T varargin_1_stateB1_velocity_BB,
  const StateBusB2 varargin_1_stateB2, MOVING_OBSTACLE_TYPE varargin_1_mov_obs,
  boolean_T varargin_1_spc_retro, uint8_T varargin_1_max_vel, boolean_T
  varargin_1_obs_detection, const StateBusB1 varargin_2_stateB1, const
  StateBusB2 varargin_2_stateB2, MOVING_OBSTACLE_TYPE varargin_2_mov_obs,
  boolean_T varargin_2_spc_retro, uint8_T varargin_2_max_vel, boolean_T
  varargin_2_obs_detection)
{
  boolean_T e_p;
  boolean_T p;
  p = false;
  if (varargin_1_obs_detection == varargin_2_obs_detection) {
    if (varargin_1_max_vel == varargin_2_max_vel) {
      if (varargin_1_spc_retro == varargin_2_spc_retro) {
        if (Board2_isequal_l(varargin_1_mov_obs, varargin_2_mov_obs)) {
          if (varargin_1_stateB2.controller_battery ==
              varargin_2_stateB2.controller_battery) {
            if (varargin_1_stateB2.l_stick_button ==
                varargin_2_stateB2.l_stick_button) {
              if (varargin_1_stateB2.r_stick_button ==
                  varargin_2_stateB2.r_stick_button) {
                if (varargin_1_stateB2.button4 == varargin_2_stateB2.button4) {
                  if (varargin_1_stateB2.button3 == varargin_2_stateB2.button3)
                  {
                    if (varargin_1_stateB2.button2 == varargin_2_stateB2.button2)
                    {
                      if (varargin_1_stateB2.button1 ==
                          varargin_2_stateB2.button1) {
                        if (varargin_1_stateB2.controller_x ==
                            varargin_2_stateB2.controller_x) {
                          if (varargin_1_stateB2.controller_y ==
                              varargin_2_stateB2.controller_y) {
                            if (varargin_1_stateB2.sonar3 ==
                                varargin_2_stateB2.sonar3) {
                              if (varargin_1_stateB2.sonar2 ==
                                  varargin_2_stateB2.sonar2) {
                                if (varargin_1_stateB2.sonar1 ==
                                    varargin_2_stateB2.sonar1) {
                                  if (varargin_1_stateB2.gyroYaw ==
                                      varargin_2_stateB2.gyroYaw) {
                                    e_p = ((varargin_1_stateB2.acceleration_x ==
                                            varargin_2_stateB2.acceleration_x) &&
                                           (varargin_1_stateB2.acceleration_y ==
                                            varargin_2_stateB2.acceleration_y));
                                  } else {
                                    e_p = false;
                                  }
                                } else {
                                  e_p = false;
                                }
                              } else {
                                e_p = false;
                              }
                            } else {
                              e_p = false;
                            }
                          } else {
                            e_p = false;
                          }
                        } else {
                          e_p = false;
                        }
                      } else {
                        e_p = false;
                      }
                    } else {
                      e_p = false;
                    }
                  } else {
                    e_p = false;
                  }
                } else {
                  e_p = false;
                }
              } else {
                e_p = false;
              }
            } else {
              e_p = false;
            }
          } else {
            e_p = false;
          }

          if (e_p) {
            if (varargin_1_stateB1_velocity_BB == varargin_2_stateB1.velocity_BB)
            {
              if (varargin_1_stateB1_velocity_BA ==
                  varargin_2_stateB1.velocity_BA) {
                if (varargin_1_stateB1_velocity_FB ==
                    varargin_2_stateB1.velocity_FB) {
                  if (varargin_1_stateB1_velocity_FA ==
                      varargin_2_stateB1.velocity_FA) {
                    e_p = ((varargin_1_stateB1_temperature ==
                            varargin_2_stateB1.temperature) &&
                           (varargin_1_stateB1_battery_volt ==
                            varargin_2_stateB1.battery_voltage));
                  } else {
                    e_p = false;
                  }
                } else {
                  e_p = false;
                }
              } else {
                e_p = false;
              }
            } else {
              e_p = false;
            }
          } else {
            e_p = false;
          }
        } else {
          e_p = false;
        }
      } else {
        e_p = false;
      }
    } else {
      e_p = false;
    }
  } else {
    e_p = false;
  }

  if (e_p) {
    p = true;
  }

  return p;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Receive_global_state(void)
{
  int32_T b_previousEvent;
  boolean_T b;
  b = Board2_Is_Rx_Finished();
  if (b) {
    b = Board2_Verify_Global_Integrity();
    if (b) {
      b_previousEvent = Board2_DW.sfEvent;
      Board2_DW.sfEvent = Board2_event_STEP;
      if (Board2_DW.is_active_Actions != 0) {
        Board2_Actions();
      }

      Board2_DW.sfEvent = b_previousEvent;
      Board2_Raise_MTalk();
      if (Board2_isequal(Board2_DW.global_state.stateB1.battery_voltage,
                         Board2_DW.global_state.stateB1.temperature,
                         Board2_DW.global_state.stateB1.velocity_FA,
                         Board2_DW.global_state.stateB1.velocity_FB,
                         Board2_DW.global_state.stateB1.velocity_BA,
                         Board2_DW.global_state.stateB1.velocity_BB,
                         Board2_DW.global_state.stateB2,
                         Board2_DW.global_state.mov_obs,
                         Board2_DW.global_state.spc_retro,
                         Board2_DW.global_state.max_vel,
                         Board2_DW.global_state.obs_detection,
                         Board2_DW.receivedGlobalStatePacket.global_state.stateB1,
                         Board2_DW.receivedGlobalStatePacket.global_state.stateB2,
                         Board2_DW.receivedGlobalStatePacket.global_state.mov_obs,
                         Board2_DW.receivedGlobalStatePacket.global_state.spc_retro,
                         Board2_DW.receivedGlobalStatePacket.global_state.max_vel,
                         Board2_DW.receivedGlobalStatePacket.global_state.obs_detection))
      {
        Board2_DW.is_Supervisor = Board2_IN_Global_state_received;
        Board2_DW.time_comm = Board2_Get_Timestamp();
      } else {
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Single_Board;
        Board2_DW.is_Single_Board = Board2_IN_Other_board_failure;
        Board2_Compute_Degraded_Actions();
      }
    } else {
      Board2_DW.is_Supervisor = Board2_IN_Receive_global_state;
      Board2_Wait_Global_State();
      Board2_Lower_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    }
  } else {
    b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
      Bo_GLOBAL_STATE_RECEIVE_TIMEOUT);
    if (b) {
      Board2_Compute_Degraded_Actions();
      Board2_exit_internal_Normal();
      Board2_Enable_MUX();
      Board2_Close_Session();
      Board2_Lower_MTalk();
      Board2_Abort_Communication();
      Board2_DW.is_Supervision_task = Board2_IN_Degraded;
      Board2_DW.is_Degraded = Board2_IN_Restarting;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Verify_State_Integrity(void)
{
  return CRC_Check_State(&Board2_DW.receivedStatePacket);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Obs_Sonar2(void)
{
  return (Board2_DW.state.sonar2 >= Board2_MIN_DISTANCE) &&
    (Board2_DW.state.sonar2 <= Board2_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Check_Timeout_Ms(uint32_T b_start_time, uint32_T
  b_min_elapsed_time)
{
  return Time_Check_Elapsed_ms(b_start_time, b_min_elapsed_time);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Obs_Sonar1(void)
{
  return (Board2_DW.state.sonar1 >= Board2_MIN_DISTANCE) &&
    (Board2_DW.state.sonar1 <= Board2_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Obs_Sonar3(void)
{
  return (Board2_DW.state.sonar3 >= Board2_MIN_DISTANCE) &&
    (Board2_DW.state.sonar3 <= Board2_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Moving_obstacle(void)
{
  boolean_T b;
  if ((Board2_DW.is_active_No_obstacle != 0) && (Board2_DW.is_No_obstacle ==
       Board2_IN_No_movements) && (Board2_DW.sfEvent == Board2_event_STEP)) {
    Board2_DW.moving_obstacle = NO_OBSTACLE;
  }

  if (Board2_DW.is_active_Obstacle_from_left != 0) {
    switch (Board2_DW.is_Obstacle_from_left) {
     case Bo_IN_Moving_obstacle_from_left:
      Board2_DW.is_Obstacle_from_left = Board2_IN_Waiting;
      break;

     case Board2_IN_Obstacle_left:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Obs_Sonar2();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Obstacle_from_left = Bo_IN_Moving_obstacle_from_left;
        Board2_DW.moving_obstacle = MOVING_FROM_LEFT;
      } else if ((Board2_DW.sfEvent == Board2_event_STEP) &&
                 Board2_Check_Timeout_Ms(Board2_DW.time_obs_s1,
                  Board2_OBS_TIMEOUT)) {
        Board2_DW.is_Obstacle_from_left = Board2_IN_Waiting;
      }
      break;

     case Board2_IN_Waiting:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        if (Board2_Obs_Sonar1()) {
          b = !Board2_Obs_Sonar2();
        } else {
          b = false;
        }
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Obstacle_from_left = Board2_IN_Obstacle_left;
        Board2_DW.time_obs_s1 = Board2_Get_Timestamp();
      }
      break;
    }
  }

  if (Board2_DW.is_active_Obstacle_from_right != 0) {
    switch (Board2_DW.is_Obstacle_from_right) {
     case B_IN_Moving_obstacle_from_right:
      Board2_DW.is_Obstacle_from_right = Board2_IN_Waiting;
      break;

     case Board2_IN_Obstacle_right:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Obs_Sonar2();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Obstacle_from_right = B_IN_Moving_obstacle_from_right;
        Board2_DW.moving_obstacle = MOVING_FROM_RIGHT;
      } else if ((Board2_DW.sfEvent == Board2_event_STEP) &&
                 Board2_Check_Timeout_Ms(Board2_DW.time_obs_s3,
                  Board2_OBS_TIMEOUT)) {
        Board2_DW.is_Obstacle_from_right = Board2_IN_Waiting;
      }
      break;

     case Board2_IN_Waiting:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        if (Board2_Obs_Sonar3()) {
          b = !Board2_Obs_Sonar2();
        } else {
          b = false;
        }
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Obstacle_from_right = Board2_IN_Obstacle_right;
        Board2_DW.time_obs_s3 = Board2_Get_Timestamp();
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Low_Voltage(void)
{
  return Board2_DW.receivedStatePacket.state.battery_voltage <=
    Board2_LOW_VOLTAGE;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_High_Temp(void)
{
  return Board2_DW.receivedStatePacket.state.temperature >=
    Board2_HIGH_TEMPERATURE;
}

/* Function for Chart: '<Root>/Board2' */
static void Boa_Battery_temperature_manager(void)
{
  boolean_T b;
  if ((Board2_DW.is_active_Normal_velocity != 0) &&
      (Board2_DW.is_Normal_velocity == Board2_IN_No_limitation) &&
      (Board2_DW.sfEvent == Board2_event_STEP)) {
    Board2_DW.limit_velocity = false;
  }

  if (Board2_DW.is_active_Battery_manager != 0) {
    switch (Board2_DW.is_Battery_manager) {
     case Board2_IN_Limited:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_Low_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Battery_manager = Board2_IN_Normal;
      } else if (Board2_DW.sfEvent == Board2_event_STEP) {
        Board2_DW.is_Battery_manager = Board2_IN_Limited;
        Board2_DW.limit_velocity = true;
      }
      break;

     case Board2_IN_Normal:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_Low_Voltage();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Battery_manager = Board2_IN_Limited;
        Board2_DW.limit_velocity = true;
      }
      break;
    }
  }

  if (Board2_DW.is_active_Temperature_manager != 0) {
    switch (Board2_DW.is_Temperature_manager) {
     case Board2_IN_High_temperature:
      if ((Board2_DW.sfEvent == Board2_event_STEP) && Board2_Check_Timeout_Ms
          (Board2_DW.time_temp, Board2_TEMP_TIMEOUT)) {
        Board2_DW.is_Temperature_manager = Board2_IN_Limited_e;
        Board2_DW.limit_velocity = true;
      } else {
        if (Board2_DW.sfEvent == Board2_event_STEP) {
          b = !Board2_High_Temp();
        } else {
          b = false;
        }

        if (b) {
          Board2_DW.is_Temperature_manager = Board2_IN_Normal_p;
        }
      }
      break;

     case Board2_IN_Limited_e:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = !Board2_High_Temp();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Temperature_manager = Board2_IN_Normal_p;
      } else if (Board2_DW.sfEvent == Board2_event_STEP) {
        Board2_DW.is_Temperature_manager = Board2_IN_Limited_e;
        Board2_DW.limit_velocity = true;
      }
      break;

     case Board2_IN_Normal_p:
      if (Board2_DW.sfEvent == Board2_event_STEP) {
        b = Board2_High_Temp();
      } else {
        b = false;
      }

      if (b) {
        Board2_DW.is_Temperature_manager = Board2_IN_High_temperature;
        Board2_DW.time_temp = Board2_Get_Timestamp();
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Button2_Pressed_Retro(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.state.button2 && (!Board2_DW.prev_button2_retro));
    Board2_DW.prev_button2_retro = Board2_DW.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Button1_Pressed_Retro(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.state.button1 && (!Board2_DW.prev_button1_retro));
    Board2_DW.prev_button1_retro = Board2_DW.state.button1;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Button2_Pressed_Obs(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.state.button1 && (!Board2_DW.prev_button2_obs));
    Board2_DW.prev_button2_obs = Board2_DW.state.button1;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Button1_Pressed_Obs(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.state.button2 && (!Board2_DW.prev_button1_obs));
    Board2_DW.prev_button1_obs = Board2_DW.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Detect_Limit_Activation(void)
{
  boolean_T y;
  y = (Board2_DW.limit_velocity && (!Board2_DW.prev_limit_state));
  Board2_DW.prev_limit_state = Board2_DW.limit_velocity;
  if (y && (Board2_DW.max_vel > Board2_LIMITED_RPM)) {
    Board2_DW.max_vel = Board2_LIMITED_RPM;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_Button1_Pressed_Vel(void)
{
  boolean_T y;
  if (Board2_DW.sfEvent != Board2_event_STEP) {
    y = false;
  } else {
    y = (Board2_DW.state.button2 && (!Board2_DW.prev_button1_vel));
    Board2_DW.prev_button1_vel = Board2_DW.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_L_Stick_Up(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.state.controller_y >= Board2_INCLINATION_INCREASE_VEL);
}

/* Function for Chart: '<Root>/Board2' */
static boolean_T Board2_L_Stick_Down(void)
{
  return (Board2_DW.sfEvent == Board2_event_STEP) &&
    (Board2_DW.state.controller_y <= Board2_INCLINATION_DECREASE_VEL);
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Decrease_Max_Vel(void)
{
  uint32_T qY;
  qY = (uint32_T)Board2_DW.max_vel -
    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
    /*MW:OvSatOk*/ Board2_VEL_CHANGE;
  if (qY > Board2_DW.max_vel) {
    qY = 0U;
  }

  if ((int32_T)qY < Board2_MIN_RPM) {
    Board2_DW.max_vel = Board2_MIN_RPM;
  } else {
    Board2_DW.max_vel = (uint8_T)qY;
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Increase_Max_Vel(void)
{
  uint32_T tmp;
  uint8_T current_limit;
  if (Board2_DW.limit_velocity) {
    current_limit = Board2_LIMITED_RPM;
  } else {
    current_limit = Board2_MAX_RPM;
  }

  tmp = (uint32_T)Board2_DW.max_vel + Board2_VEL_CHANGE;
  if (tmp > 255U) {
    tmp = 255U;
  }

  if ((int32_T)tmp > current_limit) {
    Board2_DW.max_vel = current_limit;
  } else {
    Board2_DW.max_vel = (uint8_T)tmp;
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Combo(void)
{
  boolean_T b;
  if (Board2_DW.is_active_Special_retro != 0) {
    switch (Board2_DW.is_Special_retro) {
     case Board2_IN_First_button:
      b = Board2_Button2_Pressed_Retro();
      if (b) {
        Board2_DW.is_Special_retro = Board2_IN_Special_retro_change;
        Board2_DW.special_retro = !Board2_DW.special_retro;
      } else if ((Board2_DW.sfEvent == Board2_event_STEP) &&
                 Board2_Check_Timeout_Ms(Board2_DW.time_button_retro,
                  Board2_BUTTON_TIMEOUT)) {
        Board2_DW.is_Special_retro = Board2_IN_Special_retro_start;
      }
      break;

     case Board2_IN_Special_retro_change:
      Board2_DW.is_Special_retro = Board2_IN_Special_retro_start;
      break;

     case Board2_IN_Special_retro_start:
      b = Board2_Button1_Pressed_Retro();
      if (b) {
        Board2_DW.is_Special_retro = Board2_IN_First_button;
        Board2_DW.time_button_retro = Board2_Get_Timestamp();
      }
      break;
    }
  }

  if (Board2_DW.is_active_Obstacle_detection != 0) {
    switch (Board2_DW.is_Obstacle_detection) {
     case Board2_IN_First_button:
      b = Board2_Button2_Pressed_Obs();
      if (b) {
        Board2_DW.is_Obstacle_detection = Board2_IN_Special_retro_change;
        Board2_DW.obs_detection = !Board2_DW.obs_detection;
      } else if ((Board2_DW.sfEvent == Board2_event_STEP) &&
                 Board2_Check_Timeout_Ms(Board2_DW.time_button_obs,
                  Board2_BUTTON_TIMEOUT)) {
        Board2_DW.is_Obstacle_detection = Board2_IN_Special_retro_start;
      }
      break;

     case Board2_IN_Special_retro_change:
      Board2_DW.is_Obstacle_detection = Board2_IN_Special_retro_start;
      break;

     case Board2_IN_Special_retro_start:
      b = Board2_Button1_Pressed_Obs();
      if (b) {
        Board2_DW.is_Obstacle_detection = Board2_IN_First_button;
        Board2_DW.time_button_obs = Board2_Get_Timestamp();
      }
      break;
    }
  }

  if ((Board2_DW.is_active_Change_max_velocity != 0) &&
      (Board2_DW.is_Change_max_velocity == Board_IN_Manager_combo_velocity)) {
    b = Board2_Detect_Limit_Activation();
    if (b) {
      Board2_DW.is_Manager_combo_velocity = Bo_IN_Change_max_velocity_start;
    } else {
      switch (Board2_DW.is_Manager_combo_velocity) {
       case Bo_IN_Change_max_velocity_start:
        b = Board2_Button1_Pressed_Vel();
        if (b) {
          Board2_DW.is_Manager_combo_velocity = Board2_IN_First_button_p;
          Board2_DW.time_button_vel = Board2_Get_Timestamp();
        }
        break;

       case Board2_IN_First_button_p:
        b = Board2_L_Stick_Up();
        if (b) {
          Board2_DW.is_Manager_combo_velocity = Board2_IN_Max_velocity_increase;
          Board2_Increase_Max_Vel();
        } else {
          b = Board2_L_Stick_Down();
          if (b) {
            Board2_DW.is_Manager_combo_velocity =
              Board2_IN_Max_velocity_decrease;
            Board2_Decrease_Max_Vel();
          } else if ((Board2_DW.sfEvent == Board2_event_STEP) &&
                     Board2_Check_Timeout_Ms(Board2_DW.time_button_vel,
                      Board2_BUTTON_TIMEOUT)) {
            Board2_DW.is_Manager_combo_velocity =
              Bo_IN_Change_max_velocity_start;
          }
        }
        break;

       case Board2_IN_Max_velocity_decrease:
        Board2_DW.is_Manager_combo_velocity = Bo_IN_Change_max_velocity_start;
        break;

       case Board2_IN_Max_velocity_increase:
        Board2_DW.is_Manager_combo_velocity = Bo_IN_Change_max_velocity_start;
        break;
      }
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Update_Global_State(void)
{
  Board2_DW.global_state.stateB1 = Board2_DW.receivedStatePacket.state;
  Board2_DW.global_state.stateB2 = Board2_DW.state;
  Board2_DW.global_state.mov_obs = Board2_DW.moving_obstacle;
  Board2_DW.global_state.spc_retro = Board2_DW.special_retro;
  Board2_DW.global_state.max_vel = Board2_DW.max_vel;
  Board2_DW.global_state.obs_detection = Board2_DW.obs_detection;
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Receive_state(void)
{
  int32_T b_previousEvent;
  boolean_T b;
  b = Board2_Is_Rx_Finished();
  if (b) {
    b = Board2_Verify_State_Integrity();
    if (b) {
      b_previousEvent = Board2_DW.sfEvent;
      Board2_DW.sfEvent = Board2_event_STEP;
      if (Board2_DW.is_active_Moving_obstacle != 0) {
        Board2_Moving_obstacle();
      }

      Board2_DW.sfEvent = Board2_event_STEP;
      if (Board2_DW.is_active_Battery_temperature_m != 0) {
        Boa_Battery_temperature_manager();
      }

      Board2_DW.sfEvent = Board2_event_STEP;
      if (Board2_DW.is_active_Combo != 0) {
        Board2_Combo();
      }

      Board2_DW.sfEvent = b_previousEvent;
      Board2_Update_Global_State();
      Board2_DW.is_Supervisor = Board2_IN_State_received;
      Board2_Raise_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      Board2_DW.is_Supervisor = Board2_IN_Receive_state;
      Board2_Wait_State();
      Board2_Lower_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    }
  } else {
    b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
      Board2_STATE_RECEIVE_TIMEOUT);
    if (b) {
      Board2_Compute_Degraded_Actions();
      Board2_exit_internal_Normal();
      Board2_Enable_MUX();
      Board2_Close_Session();
      Board2_Lower_MTalk();
      Board2_Abort_Communication();
      Board2_DW.is_Supervision_task = Board2_IN_Degraded;
      Board2_DW.is_Degraded = Board2_IN_Restarting;
    }
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Supervisor(void)
{
  boolean_T b;
  switch (Board2_DW.is_Supervisor) {
   case Board2_IN_Decision_transmitted:
    b = Board2_Is_STalk_High();
    if (b) {
      Board2_DW.is_Supervisor = Board2_IN_Receive_decision;
      Board2_Wait_Decision();
      Board2_Lower_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
      if (b) {
        if (Board2_DW.retransmitted < Board2_MAX_RETRANSMIT) {
          Board2_DW.retransmitted = 1U;
          Board2_DW.is_Supervisor = Board2_IN_Transmit_Decision;
          Board2_Send_Decision();
          Board2_DW.time_comm = Board2_Get_Timestamp();
        } else {
          Board2_Compute_Degraded_Actions();
          Board2_exit_internal_Normal();
          Board2_Enable_MUX();
          Board2_Close_Session();
          Board2_Lower_MTalk();
          Board2_Abort_Communication();
          Board2_DW.is_Supervision_task = Board2_IN_Degraded;
          Board2_DW.is_Degraded = Board2_IN_Restarting;
        }
      }
    }
    break;

   case IN_Global_Local_state_transmitt:
    b = Board2_Is_STalk_High();
    if (b) {
      Board2_DW.is_Supervisor = Board2_IN_Receive_global_state;
      Board2_Wait_Global_State();
      Board2_Lower_MTalk();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
      if (b) {
        if (Board2_DW.retransmitted < Board2_MAX_RETRANSMIT) {
          Board2_DW.retransmitted = 1U;
          Board2_DW.is_Supervisor = Board2_IN_Transmit_Global_State;
          Board2_Send_Global_State();
          Board2_DW.time_comm = Board2_Get_Timestamp();
        } else {
          Board2_Compute_Degraded_Actions();
          Board2_exit_internal_Normal();
          Board2_Enable_MUX();
          Board2_Close_Session();
          Board2_Lower_MTalk();
          Board2_Abort_Communication();
          Board2_DW.is_Supervision_task = Board2_IN_Degraded;
          Board2_DW.is_Degraded = Board2_IN_Restarting;
        }
      }
    }
    break;

   case Board2_IN_Global_state_received:
    b = !Board2_Is_STalk_High();
    if (b) {
      Board2_DW.is_Supervisor = Board2_IN_Transmit_Decision;
      Board2_Send_Decision();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;

   case Boar_IN_Local_state_transmitted:
    Board2_Local_state_transmitted();
    break;

   case Board2_IN_Receive_decision:
    Board2_Receive_decision();
    break;

   case Board2_IN_Receive_global_state:
    Board2_Receive_global_state();
    break;

   case Board2_IN_Receive_state:
    Board2_Receive_state();
    break;

   case Board2_IN_Same_decision:
    Board2_DW.is_Supervisor = Board2_IN_Waiting_comunication;
    Board2_Update_Local_State();
    Board2_Open_Session();
    Board2_Raise_MTalk();
    Board2_DW.time_comm = Board2_Get_Timestamp();
    break;

   case Board2_IN_State_received:
    b = !Board2_Is_STalk_High();
    if (b) {
      Board2_DW.is_Supervisor = Board2_IN_Transmit_Global_State;
      Board2_Send_Global_State();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;

   case Board2_IN_Transmit_Decision:
    b = Board2_Is_Tx_Finished();
    if (b) {
      Board2_DW.is_Supervisor = Board2_IN_Decision_transmitted;
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
        Board2_DECISION_SEND_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;

   case Board2_IN_Transmit_Global_State:
    b = Board2_Is_Tx_Finished();
    if (b) {
      Board2_DW.is_Supervisor = IN_Global_Local_state_transmitt;
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
        Board_GLOBAL_STATE_SEND_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;

   case Board2_IN_Transmit_Local_State:
    b = Board2_Is_Tx_Finished();
    if (b) {
      Board2_DW.is_Supervisor = Boar_IN_Local_state_transmitted;
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_STATE_SEND_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;

   case Board2_IN_Waiting_comunication:
    b = !Board2_Is_STalk_High();
    if (b) {
      Board2_DW.retransmitted = 0U;
      Board2_DW.is_Supervisor = Board2_IN_Transmit_Local_State;
      Board2_Send_Local_State();
      Board2_DW.time_comm = Board2_Get_Timestamp();
    } else {
      b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_INITIAL_TIMEOUT);
      if (b) {
        Board2_Compute_Degraded_Actions();
        Board2_exit_internal_Normal();
        Board2_Enable_MUX();
        Board2_Close_Session();
        Board2_Lower_MTalk();
        Board2_Abort_Communication();
        Board2_DW.is_Supervision_task = Board2_IN_Degraded;
        Board2_DW.is_Degraded = Board2_IN_Restarting;
      }
    }
    break;
  }
}

/* Function for Chart: '<Root>/Board2' */
static void Board2_Init_Data_Structures(void)
{
  static const StateBusB2 tmp = { 0.0F,/* acceleration_y */
    0.0F,                              /* acceleration_x */
    0.0F,                              /* gyroYaw */
    0U,                                /* sonar1 */
    0U,                                /* sonar2 */
    0U,                                /* sonar3 */
    0U,                                /* controller_y */
    0U,                                /* controller_x */
    false,                             /* button1 */
    false,                             /* button2 */
    false,                             /* button3 */
    false,                             /* button4 */
    false,                             /* r_stick_button */
    false,                             /* l_stick_button */
    0U                                 /* controller_battery */
  };

  static const GSBus tmp_0 = { { 0.0F, /* battery_voltage */
      0.0F,                            /* temperature */
      0,                               /* velocity_FA */
      0,                               /* velocity_FB */
      0,                               /* velocity_BA */
      0                                /* velocity_BB */
    },                                 /* stateB1 */

    { 0.0F,                            /* acceleration_y */
      0.0F,                            /* acceleration_x */
      0.0F,                            /* gyroYaw */
      0U,                              /* sonar1 */
      0U,                              /* sonar2 */
      0U,                              /* sonar3 */
      0U,                              /* controller_y */
      0U,                              /* controller_x */
      false,                           /* button1 */
      false,                           /* button2 */
      false,                           /* button3 */
      false,                           /* button4 */
      false,                           /* r_stick_button */
      false,                           /* l_stick_button */
      0U                               /* controller_battery */
    },                                 /* stateB2 */
    NO_OBSTACLE,                       /* mov_obs */
    false,                             /* spc_retro */
    0U,                                /* max_vel */
    true                               /* obs_detection */
  };

  static const DecBus tmp_1 = { 0.0F,  /* rif_FA */
    0.0F,                              /* rif_FB */
    0.0F,                              /* rif_BA */
    0.0F,                              /* rif_BB */
    NONE,                              /* brk_mode */
    OFF,                               /* led_A */
    OFF,                               /* led_B */
    IDLE,                              /* rear_led */
    SIGN_OFF,                          /* rear_sign */
    DEFAULT,                           /* mode */
    false                              /* relay */
  };

  static const PacketStateB1 tmp_2 = { { 0.0F,/* battery_voltage */
      0.0F,                            /* temperature */
      0,                               /* velocity_FA */
      0,                               /* velocity_FB */
      0,                               /* velocity_BA */
      0                                /* velocity_BB */
    },                                 /* state */
    0U                                 /* crc */
  };

  static const PacketGstate tmp_3 = { { { 0.0F,/* battery_voltage */
        0.0F,                          /* temperature */
        0,                             /* velocity_FA */
        0,                             /* velocity_FB */
        0,                             /* velocity_BA */
        0                              /* velocity_BB */
      },                               /* stateB1 */

      { 0.0F,                          /* acceleration_y */
        0.0F,                          /* acceleration_x */
        0.0F,                          /* gyroYaw */
        0U,                            /* sonar1 */
        0U,                            /* sonar2 */
        0U,                            /* sonar3 */
        0U,                            /* controller_y */
        0U,                            /* controller_x */
        false,                         /* button1 */
        false,                         /* button2 */
        false,                         /* button3 */
        false,                         /* button4 */
        false,                         /* r_stick_button */
        false,                         /* l_stick_button */
        0U                             /* controller_battery */
      },                               /* stateB2 */
      NO_OBSTACLE,                     /* mov_obs */
      false,                           /* spc_retro */
      0U,                              /* max_vel */
      true                             /* obs_detection */
    },                                 /* global_state */
    0U                                 /* crc */
  };

  static const PacketDecision tmp_4 = { { 0.0F,/* rif_FA */
      0.0F,                            /* rif_FB */
      0.0F,                            /* rif_BA */
      0.0F,                            /* rif_BB */
      NONE,                            /* brk_mode */
      OFF,                             /* led_A */
      OFF,                             /* led_B */
      IDLE,                            /* rear_led */
      SIGN_OFF,                        /* rear_sign */
      DEFAULT,                         /* mode */
      false                            /* relay */
    },                                 /* decision */
    0U                                 /* crc */
  };

  Board2_DW.state = tmp;
  Board2_DW.global_state = tmp_0;
  Board2_DW.decision = tmp_1;
  Board2_DW.receivedStatePacket = tmp_2;
  Board2_DW.receivedGlobalStatePacket = tmp_3;
  Board2_DW.receivedDecisionPacket = tmp_4;
}

/* Model step function */
void Board2_step(void)
{
  boolean_T b;

  /* Chart: '<Root>/Board2' */
  Board2_DW.sfEvent = Board2_CALL_EVENT;
  if (Board2_DW.is_active_c1_Board2 == 0) {
    Board2_DW.is_active_c1_Board2 = 1U;
    Board2_Init_Data_Structures();
    Board2_Lower_MTalk();
    Board2_DW.is_c1_Board2 = Board2_IN_Supervision_task;
    Board2_DW.is_Supervision_task = Board2_IN_Normal;
    Board2_Disable_MUX();
    Board2_enter_internal_Normal();
  } else if (Board2_DW.is_c1_Board2 == Board2_IN_Supervision_task) {
    switch (Board2_DW.is_Supervision_task) {
     case Board2_IN_Degraded:
      switch (Board2_DW.is_Degraded) {
       case Board2_IN_Restablish:
        switch (Board2_DW.is_Restablish) {
         case Boar_IN_Connection_restablished:
          Board2_DW.is_Restablish = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Degraded = Board2_IN_NO_ACTIVE_CHILD;
          Board2_DW.is_Supervision_task = Board2_IN_Normal;
          Board2_Disable_MUX();
          Board2_enter_internal_Normal();
          break;

         case Board2_IN_Ping_transmitted:
          b = Board2_Is_STalk_High();
          if (b) {
            Board2_DW.is_Restablish = Board2_IN_Receive_ping;
            Board2_Wait_Ping();
            Board2_Lower_MTalk();
            Board2_DW.time_comm = Board2_Get_Timestamp();
          } else {
            b = Board2_Check_Timeout_Us(Board2_DW.time_comm, Board2_WAIT_TIMEOUT);
            if (b) {
              if (Board2_DW.retransmitted < Board2_MAX_RETRANSMIT) {
                Board2_DW.retransmitted = 1U;
                Board2_DW.is_Restablish = Board2_IN_Transmit_ping;
                Board2_Send_Ping();
                Board2_DW.time_comm = Board2_Get_Timestamp();
              } else {
                Board2_Abort_Communication();
                Board2_Close_Session();
                Board2_Lower_MTalk();
                Board2_DW.is_Restablish = Board2_IN_NO_ACTIVE_CHILD;
                Board2_DW.is_Degraded = Board2_IN_Restarting;
              }
            }
          }
          break;

         case Board2_IN_Receive_ping:
          b = Board2_Is_Rx_Finished();
          if (b) {
            b = Board2_Verify_Ping();
            if (b) {
              Board2_Close_Session();
              Board2_DW.is_Restablish = Boar_IN_Connection_restablished;
            } else {
              Board2_DW.is_Restablish = Board2_IN_Receive_ping;
              Board2_Wait_Ping();
              Board2_Lower_MTalk();
              Board2_DW.time_comm = Board2_Get_Timestamp();
            }
          } else {
            b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
              Board2_BYTE_RECEIVE_TIMEOUT);
            if (b) {
              Board2_Abort_Communication();
              Board2_Close_Session();
              Board2_Lower_MTalk();
              Board2_DW.is_Restablish = Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Degraded = Board2_IN_Restarting;
            }
          }
          break;

         case Board_IN_Starting_to_restablish:
          b = !Board2_Is_STalk_High();
          if (b) {
            Board2_DW.retransmitted = 0U;
            Board2_DW.is_Restablish = Board2_IN_Transmit_ping;
            Board2_Send_Ping();
            Board2_DW.time_comm = Board2_Get_Timestamp();
          } else {
            b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
              Board2_INITIAL_TIMEOUT);
            if (b) {
              Board2_Abort_Communication();
              Board2_Close_Session();
              Board2_Lower_MTalk();
              Board2_DW.is_Restablish = Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Degraded = Board2_IN_Restarting;
            }
          }
          break;

         case Board2_IN_Transmit_ping:
          b = Board2_Is_Tx_Finished();
          if (b) {
            Board2_DW.is_Restablish = Board2_IN_Ping_transmitted;
            Board2_DW.time_comm = Board2_Get_Timestamp();
          } else {
            b = Board2_Check_Timeout_Us(Board2_DW.time_comm,
              Board2_BYTE_SEND_TIMEOUT);
            if (b) {
              Board2_Abort_Communication();
              Board2_Close_Session();
              Board2_Lower_MTalk();
              Board2_DW.is_Restablish = Board2_IN_NO_ACTIVE_CHILD;
              Board2_DW.is_Degraded = Board2_IN_Restarting;
            }
          }
          break;
        }
        break;

       case Board2_IN_Restarting:
        Board2_DW.is_Degraded = Board2_IN_Restablish;
        Board2_Compute_Degraded_Actions();
        Board2_DW.is_Restablish = Board_IN_Starting_to_restablish;
        Board2_Open_Session();
        Board2_Raise_MTalk();
        Board2_DW.time_comm = Board2_Get_Timestamp();
        break;
      }
      break;

     case Board2_IN_Normal:
      if (Board2_DW.is_active_Supervisor != 0) {
        Board2_Supervisor();
      }

      if (Board2_DW.is_Supervision_task == Board2_IN_Normal) {
        if (Board2_DW.is_active_Moving_obstacle != 0) {
          Board2_Moving_obstacle();
        }

        if (Board2_DW.is_active_Battery_temperature_m != 0) {
          Boa_Battery_temperature_manager();
        }

        if (Board2_DW.is_active_Combo != 0) {
          Board2_Combo();
        }

        if (Board2_DW.is_active_Actions != 0) {
          Board2_Actions();
        }
      }
      break;

     case Board2_IN_Single_Board:
      if (Board2_DW.is_Single_Board == Board2_IN_Other_board_failure) {
        Board2_Compute_Degraded_Actions();
      }
      break;
    }
  }

  /* End of Chart: '<Root>/Board2' */
}

/* Model initialize function */
void Board2_initialize(void)
{
  /* SystemInitialize for Chart: '<Root>/Board2' */
  Board2_DW.sfEvent = Board2_CALL_EVENT;
  Board2_DW.TURN_THRESHOLD = 5.0F;
}

/* Model terminate function */
void Board2_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
