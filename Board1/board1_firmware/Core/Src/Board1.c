/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board1.c
 *
 * Code generated for Simulink model 'Board1'.
 *
 * Model version                  : 1.2403
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Feb  5 17:49:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Board1.h"
#include "rtwtypes.h"
#include "Board1_types.h"
#include <math.h>

/* Named constants for Chart: '<Root>/Board1' */
#define Bo_GLOBAL_STATE_RECEIVE_TIMEOUT (5500U)
#define Board1_BUTTON_TIMEOUT          (700U)
#define Board1_BYTE_RECEIVE_TIMEOUT    (1000U)
#define Board1_BYTE_SEND_TIMEOUT       (500U)
#define Board1_CALL_EVENT              (-1)
#define Board1_CENTER                  (255.0F)
#define Board1_CONTROLLER_ZERO         ((uint16_T)255U)
#define Board1_CRITICAL_VOLTAGE        (9.0F)
#define Board1_DECISION_RECEIVE_TIMEOUT (3400U)
#define Board1_DECISION_SEND_TIMEOUT   (2900U)
#define Board1_HIGH_TEMPERATURE        (60.0F)
#define Board1_IMM_DISTANCE            ((uint16_T)70U)
#define Board1_INCLINATION_INCREASE_VEL ((uint16_T)450U)
#define Board1_LIMITED_RPM             ((uint8_T)80U)
#define Board1_LOW_CONTROLLER_BATTERY  ((uint8_T)5U)
#define Board1_LOW_VOLTAGE             (9.83F)
#define Board1_MAX_DISTANCE            ((uint16_T)300U)
#define Board1_MAX_RETRANSMIT          ((uint8_T)1U)
#define Board1_MAX_RPM                 ((uint8_T)150U)
#define Board1_MIN_DISTANCE            ((uint16_T)150U)
#define Board1_MIN_TURN_SCALE_EVASIVE  (0.2F)
#define Board1_OBS_TIMEOUT             (5000U)
#define Board1_PERIOD                  (0.06F)
#define Board1_PROTECTION_DISTANCE     ((uint16_T)40U)
#define Board1_PURE_TURN_EPS           (0.01F)
#define Board1_STATE_RECEIVE_TIMEOUT   (3800U)
#define Board1_STATE_SEND_TIMEOUT      (2200U)
#define Board1_STOP_THRESHOLD          ((uint16_T)1U)
#define Board1_TEMP_TIMEOUT            (15000U)
#define Board1_TURN_ANGLE              (45.0F)
#define Board1_TURN_BACK_ANGLE         (180.0F)
#define Board1_TURN_BACK_COUNT         (40U)
#define Board1_TURN_BACK_RPM           (40.0F)
#define Board1_TURN_COUNT              (20U)
#define Board1_TURN_RATIO              (0.35F)
#define Board1_TURN_RPM                (20.0F)
#define Board1_TURN_THRESHOLD          (5.0F)
#define Board1_VEL_CHANGE              ((uint8_T)10U)
#define Board1_WAIT_TIMEOUT            (500U)
#define Board1_event_STEP              (1483)
#define Board_GLOBAL_STATE_SEND_TIMEOUT (5000U)

/* Block states (default storage) */
DW_Board1_T Board1_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Board1_T Board1_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Board1_T Board1_Y;

/* Real-time model */
static RT_MODEL_Board1_T Board1_M_;
RT_MODEL_Board1_T *const Board1_M = &Board1_M_;

/* Forward declaration for local functions */
static void Board1_enter_internal_Normal(void);
static boolean_T Board1_isSessionhigh(void);
static boolean_T Board1_Check_Timeout_Us(uint32_T b_start_time, uint32_T
  b_min_elapsed_time);
static void Board1_Abort_Communication(void);
static void Board1_setSTalk(void);
static void Board1_Send_Ping(void);
static uint32_T Board1_Get_Timestamp(void);
static boolean_T Board1_Is_Rx_Finished(void);
static boolean_T Board1_Verify_Ping(void);
static void Board1_Wait_Ping(void);
static void Board1_resetSTalk(void);
static boolean_T Board1_isMTalkhigh(void);
static boolean_T Board1_Is_Tx_Finished(void);
static void Board1_stop_motors(boolean_T degraded);
static void Board1_Send_Decision(void);
static void Board1_exit_internal_Normal(void);
static void Board1_Write_Output(void);
static void Board1_Decision_transmitted(void);
static void Board1_Send_Global_State(void);
static boolean_T Board1_isequal_o(MOVING_OBSTACLE_TYPE varargin_1,
  MOVING_OBSTACLE_TYPE varargin_2);
static boolean_T Board1_isequal(real32_T varargin_1_stateB1_battery_volt,
  real32_T varargin_1_stateB1_temperature, int16_T
  varargin_1_stateB1_velocity_FA, int16_T varargin_1_stateB1_velocity_FB,
  int16_T varargin_1_stateB1_velocity_BA, int16_T varargin_1_stateB1_velocity_BB,
  boolean_T varargin_1_stateB1_motorError_F, boolean_T
  varargin_1_stateB1_motorError_0, boolean_T varargin_1_stateB1_motorError_B,
  boolean_T varargin_1_stateB1_motorError_1, const StateBusB2 varargin_1_stateB2,
  MOVING_OBSTACLE_TYPE varargin_1_mov_obs, boolean_T varargin_1_spc_retro,
  boolean_T varargin_1_limit_vel, int8_T varargin_1_change_vel, boolean_T
  varargin_1_obs_detection, const StateBusB1 varargin_2_stateB1, const
  StateBusB2 varargin_2_stateB2, MOVING_OBSTACLE_TYPE varargin_2_mov_obs,
  boolean_T varargin_2_spc_retro, boolean_T varargin_2_limit_vel, int8_T
  varargin_2_change_vel, boolean_T varargin_2_obs_detection);
static void Board1_Wait_Decision(void);
static void Global_Local_state_transmitted(void);
static void Board1_Send_Local_State(void);
static void Board1_Wait_Global_State(void);
static boolean_T Board_Verify_Decision_Integrity(void);
static boolean_T Board1_Critical_Voltage(void);
static boolean_T Board1_Motor_Error(void);
static boolean_T Board1_Detect_Limit_Activation(void);
static void Board1_Decrease_Max_Vel(void);
static void Board1_Increase_Max_Vel(void);
static boolean_T Board1_Critical_Voltage_Occured(void);
static boolean_T Board1_Motor_Error_Occured(void);
static boolean_T Board1_Is_Rover_Stationary(void);
static boolean_T Board1_Emergency_B_Pressed(void);
static void Board1_Turn_Left(void);
static void Board1_Turn_Right(void);
static void Board1_Update_Angle(real32_T yaw);
static void Board1_Stop_Motors(void);
static void Board1_Emergency_sonar_routine(void);
static boolean_T Board1_Near_Obstacle(void);
static boolean_T Board1_Stop_B_Pressed(void);
static void Board1_Process_Evasive_Commands(boolean_T turn_right);
static void Moving_obstacle_from_right_rout(void);
static boolean_T Board1_Is_Rover_Moving_Forward(void);
static boolean_T Board1_Emergency_S_Routine(void);
static boolean_T Board1_Mov_Obs_R_Routine(void);
static boolean_T Board1_Mov_Obs_L_Routine(void);
static boolean_T Board1_Stop_Slow_Routine(void);
static boolean_T Low_Controller_Battery_Routine(void);
static boolean_T Board1_Spec_Retro_Routine(void);
static boolean_T Board1_Stationary_Obs_Routine(void);
static void Board1_Process_User_Commands(void);
static void Board1_Turn_Back(void);
static void enter_internal_Moving_obstacle_(void);
static void enter_internal_Moving_obstacl_f(void);
static void Board1_Select_routine(void);
static void Board1_Special_retro_routine(void);
static boolean_T Board1_Mov_Obs_Right(void);
static boolean_T Board1_Mov_Obs_Left(void);
static void Bo_exit_internal_Normal_routine(void);
static void Board1_Routine_manager(void);
static boolean_T Board1_Mode_B_Pressed(void);
static void Board1_Rover_Lights_OFF(void);
static boolean_T Board1_Lights_B_Pressed(void);
static void Board1_Update_Rover_Lights(boolean_T white_led_when_stopped);
static void Board1_Rover_Lights_Motor_Error(void);
static void Board1_Board_decision(void);
static void Board1_Receive_decision(void);
static boolean_T Board1_Verify_Global_Integrity(void);
static boolean_T Board1_Obs_Sonar2(void);
static boolean_T Board1_Check_Timeout_Ms(uint32_T b_start_time, uint32_T
  b_min_elapsed_time);
static boolean_T Board1_Obs_Sonar1(void);
static boolean_T Board1_Obs_Sonar3(void);
static boolean_T Board1_Low_Voltage(void);
static boolean_T Board1_High_Temp(void);
static boolean_T Board1_Button2_Pressed_Retro(void);
static boolean_T Board1_Button1_Pressed_Retro(void);
static boolean_T Board1_Button2_Pressed_Obs(void);
static boolean_T Board1_Button1_Pressed_Obs(void);
static boolean_T Board1_Button1_Pressed_Vel(void);
static boolean_T Board1_L_Stick_Up(void);
static boolean_T Board1_L_Stick_Down(void);
static void Board1_Combo(void);
static void Board1_Global_state_compute(void);
static void Board1_Update_Global_State(void);
static void Board1_Receive_global_state(void);
static boolean_T Board1_Verify_State_Integrity(void);
static void Board1_Wait_State(void);
static void Board1_Update_Local_State(void);
static void Board1_Normal(void);
static void Board1_Init_Data(void);

/* Function for Chart: '<Root>/Board1' */
static void Board1_enter_internal_Normal(void)
{
  Board1_DW.is_active_Supervisor = 1U;
  Board1_DW.is_Supervisor = Board1_IN_Waiting_to_start;
  Board1_DW.is_active_Global_state_compute = 1U;
  Board1_DW.is_active_Moving_obstacle = 1U;
  Board1_DW.is_active_No_obstacle = 1U;
  Board1_DW.is_No_obstacle = Board1_IN_No_movements;
  Board1_DW.moving_from_left = false;
  Board1_DW.moving_from_right = false;
  Board1_DW.is_active_Obstacle_from_left = 1U;
  Board1_DW.is_Obstacle_from_left = Board1_IN_Waiting;
  Board1_DW.is_active_Obstacle_from_right = 1U;
  Board1_DW.is_Obstacle_from_right = Board1_IN_Waiting;
  Board1_DW.is_active_Battery_temperature_m = 1U;
  Board1_DW.is_active_Normal_velocity = 1U;
  Board1_DW.is_Normal_velocity = Board1_IN_No_limitation;
  Board1_DW.limit_velocity = false;
  Board1_DW.is_active_Battery_manager = 1U;
  Board1_DW.is_Battery_manager = Board1_IN_Normal;
  Board1_DW.is_active_Temperature_manager = 1U;
  Board1_DW.is_Temperature_manager = Board1_IN_Normal_k;
  Board1_DW.is_active_Combo = 1U;
  Board1_DW.is_active_Special_retro = 1U;
  Board1_DW.is_Special_retro = Board1_IN_Special_retro_start;
  Board1_DW.is_active_Obstacle_detection = 1U;
  Board1_DW.is_Obstacle_detection = Board1_IN_Special_retro_start;
  Board1_DW.is_active_Change_max_velocity = 1U;
  Board1_DW.is_Change_max_velocity = Bo_IN_Change_max_velocity_start;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_isSessionhigh(void)
{
  return IO_Read_Session();
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Check_Timeout_Us(uint32_T b_start_time, uint32_T
  b_min_elapsed_time)
{
  return Time_Check_Elapsed_us(b_start_time, b_min_elapsed_time);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Abort_Communication(void)
{
  UART_Stop_DMA();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_setSTalk(void)
{
  IO_Set_SlaveTalk();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Send_Ping(void)
{
  UART_Send_Ping();
}

/* Function for Chart: '<Root>/Board1' */
static uint32_T Board1_Get_Timestamp(void)
{
  return Time_Get_Tick();
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Is_Rx_Finished(void)
{
  return UART_Is_Rx_Complete();
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Verify_Ping(void)
{
  return UART_Check_Ping(Board1_DW.receivedPing);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Wait_Ping(void)
{
  UART_Wait_Ping(&Board1_DW.receivedPing);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_resetSTalk(void)
{
  IO_Reset_SlaveTalk();
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_isMTalkhigh(void)
{
  return IO_Read_MasterTalk();
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Is_Tx_Finished(void)
{
  return UART_Is_Tx_Complete();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_stop_motors(boolean_T degraded)
{
  OS_Enter_Critical();

  /* Outport: '<Root>/output' */
  Board1_Y.output.rif_FA = 0.0F;
  Board1_Y.output.rif_FB = 0.0F;
  Board1_Y.output.rif_BA = 0.0F;
  Board1_Y.output.rif_BB = 0.0F;
  Board1_Y.output.brk_mode = EMERGENCY;
  Board1_Y.output.relay = false;

  /* Inport: '<Root>/battery_voltage' */
  if (Board1_U.battery_voltage < Board1_CRITICAL_VOLTAGE) {
    /* Outport: '<Root>/output' */
    Board1_Y.output.led_A = OFF;
    Board1_Y.output.led_B = OFF;
    Board1_Y.output.rear_sign = SIGN_OFF;
    Board1_Y.output.rear_led = IDLE;
  } else if (degraded) {
    /* Outport: '<Root>/output' */
    Board1_Y.output.led_A = OFF;
    Board1_Y.output.led_B = OFF;
    Board1_Y.output.rear_led = DEGRADED_LIGHTS;
    Board1_Y.output.rear_sign = SIGN_YELLOW;
  } else {
    /* Outport: '<Root>/output' */
    Board1_Y.output.led_A = RED;
    Board1_Y.output.led_B = RED;
    Board1_Y.output.rear_led = BRAKING_LIGHTS;
    Board1_Y.output.rear_sign = SIGN_RED;
  }

  /* End of Inport: '<Root>/battery_voltage' */
  OS_Exit_Critical();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Send_Decision(void)
{
  UART_Send_Decision(&Board1_DW.decision);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_exit_internal_Normal(void)
{
  Board1_DW.is_Change_max_velocity = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Change_max_velocity = 0U;
  Board1_DW.is_Obstacle_detection = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Obstacle_detection = 0U;
  Board1_DW.is_Special_retro = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Special_retro = 0U;
  Board1_DW.is_active_Combo = 0U;
  Board1_DW.is_Temperature_manager = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Temperature_manager = 0U;
  Board1_DW.is_Battery_manager = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Battery_manager = 0U;
  Board1_DW.is_Normal_velocity = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Normal_velocity = 0U;
  Board1_DW.is_active_Battery_temperature_m = 0U;
  Board1_DW.is_Obstacle_from_right = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Obstacle_from_right = 0U;
  Board1_DW.is_Obstacle_from_left = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Obstacle_from_left = 0U;
  Board1_DW.is_No_obstacle = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_No_obstacle = 0U;
  Board1_DW.is_active_Moving_obstacle = 0U;
  Board1_DW.is_active_Global_state_compute = 0U;
  Board1_DW.is_Supervisor = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_active_Supervisor = 0U;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Write_Output(void)
{
  OS_Enter_Critical();

  /* Outport: '<Root>/output' */
  Board1_Y.output = Board1_DW.decision;
  OS_Exit_Critical();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Decision_transmitted(void)
{
  boolean_T b;
  boolean_T d_p;
  b = !Board1_isSessionhigh();
  if (b) {
    b = false;
    if (Board1_DW.decision.relay ==
        Board1_DW.receivedDecisionPacket.decision.relay) {
      d_p = false;
      if ((int32_T)Board1_DW.decision.mode == (int32_T)
          Board1_DW.receivedDecisionPacket.decision.mode) {
        d_p = true;
      }

      if (d_p) {
        d_p = false;
        if ((int32_T)Board1_DW.decision.rear_sign == (int32_T)
            Board1_DW.receivedDecisionPacket.decision.rear_sign) {
          d_p = true;
        }

        if (d_p) {
          d_p = false;
          if ((int32_T)Board1_DW.decision.rear_led == (int32_T)
              Board1_DW.receivedDecisionPacket.decision.rear_led) {
            d_p = true;
          }

          if (d_p) {
            d_p = false;
            if ((int32_T)Board1_DW.decision.led_B == (int32_T)
                Board1_DW.receivedDecisionPacket.decision.led_B) {
              d_p = true;
            }

            if (d_p) {
              d_p = false;
              if ((int32_T)Board1_DW.decision.led_A == (int32_T)
                  Board1_DW.receivedDecisionPacket.decision.led_A) {
                d_p = true;
              }

              if (d_p) {
                d_p = false;
                if ((int32_T)Board1_DW.decision.brk_mode == (int32_T)
                    Board1_DW.receivedDecisionPacket.decision.brk_mode) {
                  d_p = true;
                }

                if (d_p) {
                  if (Board1_DW.decision.rif_BB ==
                      Board1_DW.receivedDecisionPacket.decision.rif_BB) {
                    if (Board1_DW.decision.rif_BA ==
                        Board1_DW.receivedDecisionPacket.decision.rif_BA) {
                      d_p = ((Board1_DW.decision.rif_FB ==
                              Board1_DW.receivedDecisionPacket.decision.rif_FB) &&
                             (Board1_DW.decision.rif_FA ==
                              Board1_DW.receivedDecisionPacket.decision.rif_FA));
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
      Board1_DW.is_Supervisor = Board1_IN_Same_decision;
      Board1_Write_Output();
    } else {
      Board1_exit_internal_Normal();
      Board1_Abort_Communication();
      Board1_DW.is_Board_state = Board1_IN_Single_Board;
      Board1_resetSTalk();
      Board1_DW.is_Single_Board = Board1_IN_Other_board_failure;
      Board1_stop_motors(false);
    }
  } else {
    b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
    if (b) {
      if (Board1_DW.retransmitted < Board1_MAX_RETRANSMIT) {
        Board1_DW.retransmitted = 1U;
        Board1_DW.is_Supervisor = Board1_IN_Transmit_Decision;
        Board1_Send_Decision();
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        Board1_stop_motors(true);
        Board1_exit_internal_Normal();
        Board1_Abort_Communication();
        Board1_DW.is_Board_state = Board1_IN_Degraded;
        Board1_setSTalk();
        Board1_DW.is_Degraded = Board1_IN_Restarting;
      }
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Send_Global_State(void)
{
  UART_Send_GlobalState(&Board1_DW.global_state);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_isequal_o(MOVING_OBSTACLE_TYPE varargin_1,
  MOVING_OBSTACLE_TYPE varargin_2)
{
  boolean_T p;
  p = false;
  if ((int32_T)varargin_1 == (int32_T)varargin_2) {
    p = true;
  }

  return p;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_isequal(real32_T varargin_1_stateB1_battery_volt,
  real32_T varargin_1_stateB1_temperature, int16_T
  varargin_1_stateB1_velocity_FA, int16_T varargin_1_stateB1_velocity_FB,
  int16_T varargin_1_stateB1_velocity_BA, int16_T varargin_1_stateB1_velocity_BB,
  boolean_T varargin_1_stateB1_motorError_F, boolean_T
  varargin_1_stateB1_motorError_0, boolean_T varargin_1_stateB1_motorError_B,
  boolean_T varargin_1_stateB1_motorError_1, const StateBusB2 varargin_1_stateB2,
  MOVING_OBSTACLE_TYPE varargin_1_mov_obs, boolean_T varargin_1_spc_retro,
  boolean_T varargin_1_limit_vel, int8_T varargin_1_change_vel, boolean_T
  varargin_1_obs_detection, const StateBusB1 varargin_2_stateB1, const
  StateBusB2 varargin_2_stateB2, MOVING_OBSTACLE_TYPE varargin_2_mov_obs,
  boolean_T varargin_2_spc_retro, boolean_T varargin_2_limit_vel, int8_T
  varargin_2_change_vel, boolean_T varargin_2_obs_detection)
{
  boolean_T e_p;
  boolean_T p;
  p = false;
  if (varargin_1_obs_detection == varargin_2_obs_detection) {
    if (varargin_1_change_vel == varargin_2_change_vel) {
      if (varargin_1_limit_vel == varargin_2_limit_vel) {
        if (varargin_1_spc_retro == varargin_2_spc_retro) {
          if (Board1_isequal_o(varargin_1_mov_obs, varargin_2_mov_obs)) {
            if (varargin_1_stateB2.gyroError == varargin_2_stateB2.gyroError) {
              if (varargin_1_stateB2.controllerError ==
                  varargin_2_stateB2.controllerError) {
                if (varargin_1_stateB2.controller_battery ==
                    varargin_2_stateB2.controller_battery) {
                  if (varargin_1_stateB2.l_stick_button ==
                      varargin_2_stateB2.l_stick_button) {
                    if (varargin_1_stateB2.r_stick_button ==
                        varargin_2_stateB2.r_stick_button) {
                      if (varargin_1_stateB2.button4 ==
                          varargin_2_stateB2.button4) {
                        if (varargin_1_stateB2.button3 ==
                            varargin_2_stateB2.button3) {
                          if (varargin_1_stateB2.button2 ==
                              varargin_2_stateB2.button2) {
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
                                      e_p = ((varargin_1_stateB2.sonar1 ==
                                              varargin_2_stateB2.sonar1) &&
                                             (varargin_1_stateB2.gyroYaw ==
                                              varargin_2_stateB2.gyroYaw));
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
              if (varargin_1_stateB1_motorError_1 ==
                  varargin_2_stateB1.motorError_BB) {
                if (varargin_1_stateB1_motorError_B ==
                    varargin_2_stateB1.motorError_BA) {
                  if (varargin_1_stateB1_motorError_0 ==
                      varargin_2_stateB1.motorError_FB) {
                    if (varargin_1_stateB1_motorError_F ==
                        varargin_2_stateB1.motorError_FA) {
                      if (varargin_1_stateB1_velocity_BB ==
                          varargin_2_stateB1.velocity_BB) {
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

/* Function for Chart: '<Root>/Board1' */
static void Board1_Wait_Decision(void)
{
  UART_Wait_Decision(&Board1_DW.receivedDecisionPacket);
}

/* Function for Chart: '<Root>/Board1' */
static void Global_Local_state_transmitted(void)
{
  boolean_T b;
  b = Board1_isMTalkhigh();
  if (b) {
    if (Board1_isequal(Board1_DW.global_state.stateB1.battery_voltage,
                       Board1_DW.global_state.stateB1.temperature,
                       Board1_DW.global_state.stateB1.velocity_FA,
                       Board1_DW.global_state.stateB1.velocity_FB,
                       Board1_DW.global_state.stateB1.velocity_BA,
                       Board1_DW.global_state.stateB1.velocity_BB,
                       Board1_DW.global_state.stateB1.motorError_FA,
                       Board1_DW.global_state.stateB1.motorError_FB,
                       Board1_DW.global_state.stateB1.motorError_BA,
                       Board1_DW.global_state.stateB1.motorError_BB,
                       Board1_DW.global_state.stateB2,
                       Board1_DW.global_state.mov_obs,
                       Board1_DW.global_state.spc_retro,
                       Board1_DW.global_state.limit_vel,
                       Board1_DW.global_state.change_vel,
                       Board1_DW.global_state.obs_detection,
                       Board1_DW.receivedGlobalStatePacket.global_state.stateB1,
                       Board1_DW.receivedGlobalStatePacket.global_state.stateB2,
                       Board1_DW.receivedGlobalStatePacket.global_state.mov_obs,
                       Board1_DW.receivedGlobalStatePacket.global_state.spc_retro,
                       Board1_DW.receivedGlobalStatePacket.global_state.limit_vel,
                       Board1_DW.receivedGlobalStatePacket.global_state.change_vel,
                       Board1_DW.receivedGlobalStatePacket.global_state.obs_detection))
    {
      Board1_DW.is_Supervisor = Board1_IN_Receive_decision;
      Board1_Wait_Decision();
      Board1_resetSTalk();
      Board1_DW.time_comm = Board1_Get_Timestamp();
    } else {
      Board1_exit_internal_Normal();
      Board1_Abort_Communication();
      Board1_DW.is_Board_state = Board1_IN_Single_Board;
      Board1_resetSTalk();
      Board1_DW.is_Single_Board = Board1_IN_Other_board_failure;
      Board1_stop_motors(false);
    }
  } else {
    b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
    if (b) {
      if (Board1_DW.retransmitted < Board1_MAX_RETRANSMIT) {
        Board1_DW.retransmitted = 1U;
        Board1_DW.is_Supervisor = Board1_IN_Transmit_Global_State;
        Board1_Send_Global_State();
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        Board1_stop_motors(true);
        Board1_exit_internal_Normal();
        Board1_Abort_Communication();
        Board1_DW.is_Board_state = Board1_IN_Degraded;
        Board1_setSTalk();
        Board1_DW.is_Degraded = Board1_IN_Restarting;
      }
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Send_Local_State(void)
{
  UART_Send_Local_State(&Board1_DW.state);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Wait_Global_State(void)
{
  UART_Wait_GlobalState(&Board1_DW.receivedGlobalStatePacket);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board_Verify_Decision_Integrity(void)
{
  return CRC_Check_Decision(&Board1_DW.receivedDecisionPacket);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Critical_Voltage(void)
{
  return Board1_DW.global_state.stateB1.battery_voltage <=
    Board1_CRITICAL_VOLTAGE;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Motor_Error(void)
{
  return Board1_DW.global_state.stateB1.motorError_FA ||
    Board1_DW.global_state.stateB1.motorError_FB ||
    Board1_DW.global_state.stateB1.motorError_BA ||
    Board1_DW.global_state.stateB1.motorError_BB;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Detect_Limit_Activation(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.global_state.limit_vel && (!Board1_DW.prev_limit_state));
    if (y && (Board1_DW.max_velocity > Board1_LIMITED_RPM)) {
      Board1_DW.max_velocity = Board1_LIMITED_RPM;
    }

    Board1_DW.prev_limit_state = Board1_DW.global_state.limit_vel;
    Board1_DW.change_velocity = 0;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Decrease_Max_Vel(void)
{
  uint32_T qY;
  qY = (uint32_T)Board1_DW.max_velocity -
    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
    /*MW:OvSatOk*/ Board1_VEL_CHANGE;
  if (qY > Board1_DW.max_velocity) {
    qY = 0U;
  }

  if ((int32_T)qY < Board1_DW.MIN_RPM) {
    Board1_DW.max_velocity = Board1_DW.MIN_RPM;
  } else {
    Board1_DW.max_velocity = (uint8_T)qY;
  }

  Board1_DW.change_velocity = 0;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Increase_Max_Vel(void)
{
  uint32_T tmp;
  uint8_T current_limit;
  if (Board1_DW.global_state.limit_vel) {
    current_limit = Board1_LIMITED_RPM;
  } else {
    current_limit = Board1_MAX_RPM;
  }

  tmp = (uint32_T)Board1_DW.max_velocity + Board1_VEL_CHANGE;
  if (tmp > 255U) {
    tmp = 255U;
  }

  if ((int32_T)tmp > current_limit) {
    Board1_DW.max_velocity = current_limit;
  } else {
    Board1_DW.max_velocity = (uint8_T)tmp;
  }

  Board1_DW.change_velocity = 0;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Critical_Voltage_Occured(void)
{
  return Board1_DW.working_status == CRITICAL_VOLTAGE_WORKING;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Motor_Error_Occured(void)
{
  return Board1_DW.working_status == MOTOR_ERROR_WORKING;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Is_Rover_Stationary(void)
{
  int32_T b_k;
  int16_T b_y;
  boolean_T b_x[4];
  boolean_T exitg1;
  boolean_T y;
  if (Board1_DW.global_state.stateB1.velocity_FA < 0) {
    b_k = -Board1_DW.global_state.stateB1.velocity_FA;
    if (-Board1_DW.global_state.stateB1.velocity_FA > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board1_DW.global_state.stateB1.velocity_FA;
  }

  b_x[0] = (b_y <= Board1_STOP_THRESHOLD);
  if (Board1_DW.global_state.stateB1.velocity_FB < 0) {
    b_k = -Board1_DW.global_state.stateB1.velocity_FB;
    if (-Board1_DW.global_state.stateB1.velocity_FB > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board1_DW.global_state.stateB1.velocity_FB;
  }

  b_x[1] = (b_y <= Board1_STOP_THRESHOLD);
  if (Board1_DW.global_state.stateB1.velocity_BA < 0) {
    b_k = -Board1_DW.global_state.stateB1.velocity_BA;
    if (-Board1_DW.global_state.stateB1.velocity_BA > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board1_DW.global_state.stateB1.velocity_BA;
  }

  b_x[2] = (b_y <= Board1_STOP_THRESHOLD);
  if (Board1_DW.global_state.stateB1.velocity_BB < 0) {
    b_k = -Board1_DW.global_state.stateB1.velocity_BB;
    if (-Board1_DW.global_state.stateB1.velocity_BB > 32767) {
      b_k = 32767;
    }

    b_y = (int16_T)b_k;
  } else {
    b_y = Board1_DW.global_state.stateB1.velocity_BB;
  }

  b_x[3] = (b_y <= Board1_STOP_THRESHOLD);
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

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Emergency_B_Pressed(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    Board1_DW.global_state.stateB2.button4 &&
    (Board1_DW.global_state.stateB2.controller_battery >
     Board1_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Turn_Left(void)
{
  Board1_DW.decision.rif_FA = -20.0F;
  Board1_DW.decision.rif_FB = Board1_TURN_RPM;
  Board1_DW.decision.rif_BA = -20.0F;
  Board1_DW.decision.rif_BB = Board1_TURN_RPM;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Turn_Right(void)
{
  Board1_DW.decision.rif_FA = Board1_TURN_RPM;
  Board1_DW.decision.rif_FB = -20.0F;
  Board1_DW.decision.rif_BA = Board1_TURN_RPM;
  Board1_DW.decision.rif_BB = -20.0F;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Update_Angle(real32_T yaw)
{
  Board1_DW.angle += (yaw + Board1_DW.prevYaw) * 0.5F * Board1_PERIOD;
  Board1_DW.prevYaw = yaw;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Stop_Motors(void)
{
  Board1_DW.decision.rif_FA = 0.0F;
  Board1_DW.decision.rif_FB = 0.0F;
  Board1_DW.decision.rif_BA = 0.0F;
  Board1_DW.decision.rif_BB = 0.0F;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Emergency_sonar_routine(void)
{
  uint32_T qY;
  boolean_T b;
  b = Board1_Emergency_B_Pressed();
  if (b) {
    Board1_DW.is_Emergency_sonar_routine = Board1_IN_NO_ACTIVE_CHILD;
    Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
    Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
    Board1_Stop_Motors();
    Board1_DW.decision.brk_mode = EMERGENCY;
  } else {
    switch (Board1_DW.is_Emergency_sonar_routine) {
     case Board1_IN_Emergency_sonar:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Is_Rover_Stationary();
      } else {
        b = false;
      }

      if (b) {
        if (Board1_DW.global_state.stateB2.sonar3 > Board1_DW.distance_threshold)
        {
          if (!Board1_DW.global_state.stateB2.gyroError) {
            Board1_DW.angle = 0.0F;
            Board1_DW.prevYaw = 0.0F;
            Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_right_gyro;
            Board1_Turn_Right();
            Board1_DW.decision.brk_mode = NONE;
          } else {
            Board1_DW.turn_counter = 0U;
            Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_right_no_gyro;
            Board1_Turn_Right();
            Board1_DW.decision.brk_mode = NONE;
          }
        } else if (Board1_DW.global_state.stateB2.sonar1 >
                   Board1_DW.distance_threshold) {
          if (!Board1_DW.global_state.stateB2.gyroError) {
            Board1_DW.angle = 0.0F;
            Board1_DW.prevYaw = 0.0F;
            Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_left;
            Board1_Turn_Left();
            Board1_DW.decision.brk_mode = NONE;
          } else {
            Board1_DW.turn_counter = 0U;
            Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_left_no_gyro;
            Board1_Turn_Left();
            Board1_DW.decision.brk_mode = NONE;
          }
        } else {
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
        }
      }
      break;

     case Board1_IN_Stop_left_rotation:
      Board1_DW.is_Emergency_sonar_routine = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
      break;

     case Board1_IN_Stop_right_rotation:
      Board1_DW.is_Emergency_sonar_routine = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
      break;

     case Board1_IN_Turn_left:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        Board1_Update_Angle(Board1_DW.global_state.stateB2.gyroYaw);
        if (fabsf(Board1_DW.angle) >= Board1_TURN_ANGLE) {
          Board1_DW.angle = 0.0F;
          Board1_DW.prevYaw = 0.0F;
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Stop_left_rotation;
          Board1_Stop_Motors();
          Board1_DW.decision.brk_mode = NONE;
        } else {
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_left;
          Board1_Turn_Left();
          Board1_DW.decision.brk_mode = NONE;
        }
      }
      break;

     case Board1_IN_Turn_left_no_gyro:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        qY = Board1_DW.turn_counter + /*MW:OvSatOk*/ 1U;
        if (Board1_DW.turn_counter + 1U < Board1_DW.turn_counter) {
          qY = MAX_uint32_T;
        }

        Board1_DW.turn_counter = qY;
        if (Board1_DW.turn_counter >= Board1_TURN_COUNT) {
          Board1_DW.turn_counter = 0U;
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Stop_left_rotation;
          Board1_Stop_Motors();
          Board1_DW.decision.brk_mode = NONE;
        } else {
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_left_no_gyro;
          Board1_Turn_Left();
          Board1_DW.decision.brk_mode = NONE;
        }
      }
      break;

     case Board1_IN_Turn_right_gyro:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        Board1_Update_Angle(Board1_DW.global_state.stateB2.gyroYaw);
        if (fabsf(Board1_DW.angle) >= Board1_TURN_ANGLE) {
          Board1_DW.angle = 0.0F;
          Board1_DW.prevYaw = 0.0F;
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Stop_right_rotation;
          Board1_Stop_Motors();
          Board1_DW.decision.brk_mode = NONE;
        } else {
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_right_gyro;
          Board1_Turn_Right();
          Board1_DW.decision.brk_mode = NONE;
        }
      }
      break;

     case Board1_IN_Turn_right_no_gyro:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        qY = Board1_DW.turn_counter + /*MW:OvSatOk*/ 1U;
        if (Board1_DW.turn_counter + 1U < Board1_DW.turn_counter) {
          qY = MAX_uint32_T;
        }

        Board1_DW.turn_counter = qY;
        if (Board1_DW.turn_counter >= Board1_TURN_COUNT) {
          Board1_DW.turn_counter = 0U;
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Stop_right_rotation;
          Board1_Stop_Motors();
          Board1_DW.decision.brk_mode = NONE;
        } else {
          Board1_DW.is_Emergency_sonar_routine = Board1_IN_Turn_right_no_gyro;
          Board1_Turn_Right();
          Board1_DW.decision.brk_mode = NONE;
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Near_Obstacle(void)
{
  if (Board1_DW.global_state.obs_detection) {
    Board1_DW.distance_threshold = Board1_IMM_DISTANCE;
  } else {
    Board1_DW.distance_threshold = Board1_PROTECTION_DISTANCE;
  }

  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    ((Board1_DW.global_state.stateB2.sonar1 <= Board1_DW.distance_threshold) ||
     (Board1_DW.global_state.stateB2.sonar2 <= Board1_DW.distance_threshold) ||
     (Board1_DW.global_state.stateB2.sonar3 <= Board1_DW.distance_threshold));
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Stop_B_Pressed(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    Board1_DW.global_state.stateB2.button3 &&
    (Board1_DW.global_state.stateB2.controller_battery >
     Board1_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Process_Evasive_Commands(boolean_T turn_right)
{
  int32_T tmp;
  real32_T forward;
  real32_T throttle;
  real32_T turn;
  throttle = ((real32_T)Board1_DW.global_state.stateB2.controller_y -
              Board1_CENTER) / Board1_CENTER;
  forward = throttle * (real32_T)Board1_DW.max_velocity;
  throttle = fabsf(throttle);
  if (turn_right) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  turn = (0.3F * throttle + Board1_MIN_TURN_SCALE_EVASIVE) * (real32_T)(tmp *
    Board1_DW.max_velocity);
  if (throttle >= Board1_PURE_TURN_EPS) {
    throttle = fabsf(forward) * Board1_TURN_RATIO;
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
  if (turn > Board1_DW.max_velocity) {
    turn = (real32_T)Board1_DW.max_velocity / turn;
    throttle *= turn;
    forward *= turn;
  }

  Board1_DW.decision.rif_FA = throttle;
  Board1_DW.decision.rif_BA = throttle;
  Board1_DW.decision.rif_FB = forward;
  Board1_DW.decision.rif_BB = forward;
}

/* Function for Chart: '<Root>/Board1' */
static void Moving_obstacle_from_right_rout(void)
{
  uint32_T qY;
  boolean_T b;
  b = Board1_Emergency_B_Pressed();
  if (b) {
    Board1_DW.is_Moving_obstacle_from_right_r = Board1_IN_NO_ACTIVE_CHILD;
    Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
    Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
    Board1_Stop_Motors();
    Board1_DW.decision.brk_mode = EMERGENCY;
  } else {
    b = Board1_Near_Obstacle();
    if (b) {
      Board1_DW.is_Moving_obstacle_from_right_r = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.is_Normal_routine = Boar_IN_Emergency_sonar_routine;
      Board1_DW.is_Emergency_sonar_routine = Board1_IN_Emergency_sonar;
      Board1_Stop_Motors();
      Board1_DW.decision.brk_mode = EMERGENCY;
    } else {
      b = Board1_Stop_B_Pressed();
      if (b) {
        Board1_DW.is_Moving_obstacle_from_right_r = Board1_IN_NO_ACTIVE_CHILD;
        Board1_DW.is_Normal_routine = Board1_IN_Stop_slow_routine;
        Board1_DW.is_Stop_slow_routine = Board1_IN_Stop_slow;
        Board1_Stop_Motors();
        Board1_DW.decision.brk_mode = NORMAL;
      } else {
        switch (Board1_DW.is_Moving_obstacle_from_right_r) {
         case Board_IN_Turn_moving_right_gyro:
          if (Board1_DW.sfEvent == Board1_event_STEP) {
            Board1_Update_Angle(Board1_DW.global_state.stateB2.gyroYaw);
            if (fabsf(Board1_DW.angle) >= Board1_TURN_ANGLE) {
              Board1_DW.angle = 0.0F;
              Board1_DW.prevYaw = 0.0F;
              Board1_DW.is_Moving_obstacle_from_right_r =
                Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
            } else {
              Board1_DW.is_Moving_obstacle_from_right_r =
                Board_IN_Turn_moving_right_gyro;
              Board1_Process_Evasive_Commands(true);
              Board1_DW.decision.brk_mode = NONE;
            }
          }
          break;

         case Bo_IN_Turn_moving_right_no_gyro:
          if (Board1_DW.sfEvent == Board1_event_STEP) {
            qY = Board1_DW.turn_counter + /*MW:OvSatOk*/ 1U;
            if (Board1_DW.turn_counter + 1U < Board1_DW.turn_counter) {
              qY = MAX_uint32_T;
            }

            Board1_DW.turn_counter = qY;
            if (Board1_DW.turn_counter >= Board1_TURN_COUNT) {
              Board1_DW.turn_counter = 0U;
              Board1_DW.is_Moving_obstacle_from_right_r =
                Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
            } else {
              Board1_DW.is_Moving_obstacle_from_right_r =
                Bo_IN_Turn_moving_right_no_gyro;
              Board1_Process_Evasive_Commands(true);
              Board1_DW.decision.brk_mode = NONE;
            }
          }
          break;
        }
      }
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Is_Rover_Moving_Forward(void)
{
  int32_T k;
  boolean_T x[4];
  boolean_T exitg1;
  boolean_T y;
  x[0] = (Board1_DW.global_state.stateB1.velocity_FA > Board1_STOP_THRESHOLD);
  x[1] = (Board1_DW.global_state.stateB1.velocity_FB > Board1_STOP_THRESHOLD);
  x[2] = (Board1_DW.global_state.stateB1.velocity_BA > Board1_STOP_THRESHOLD);
  x[3] = (Board1_DW.global_state.stateB1.velocity_BB > Board1_STOP_THRESHOLD);
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

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Emergency_S_Routine(void)
{
  boolean_T y;
  if (Board1_DW.global_state.obs_detection) {
    Board1_DW.distance_threshold = Board1_IMM_DISTANCE;
  } else {
    Board1_DW.distance_threshold = Board1_PROTECTION_DISTANCE;
  }

  if ((Board1_DW.sfEvent == Board1_event_STEP) &&
      ((Board1_DW.global_state.stateB2.sonar1 <= Board1_DW.distance_threshold) ||
       (Board1_DW.global_state.stateB2.sonar2 <= Board1_DW.distance_threshold) ||
       (Board1_DW.global_state.stateB2.sonar3 <= Board1_DW.distance_threshold)))
  {
    y = Board1_Is_Rover_Moving_Forward();
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Mov_Obs_R_Routine(void)
{
  boolean_T y;
  if ((Board1_DW.sfEvent == Board1_event_STEP) &&
      (Board1_DW.global_state.mov_obs == MOVING_FROM_RIGHT)) {
    y = (Board1_Is_Rover_Moving_Forward() &&
         Board1_DW.global_state.obs_detection);
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Mov_Obs_L_Routine(void)
{
  boolean_T y;
  if ((Board1_DW.sfEvent == Board1_event_STEP) &&
      (Board1_DW.global_state.mov_obs == MOVING_FROM_LEFT)) {
    y = (Board1_Is_Rover_Moving_Forward() &&
         Board1_DW.global_state.obs_detection);
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Stop_Slow_Routine(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent == Board1_event_STEP) {
    if (((Board1_DW.global_state.stateB2.sonar2 <= Board1_MAX_DISTANCE) ||
         (Board1_DW.global_state.mov_obs == MOVING_FROM_BOTH)) &&
        Board1_Is_Rover_Moving_Forward() && Board1_DW.global_state.obs_detection)
    {
      y = true;
    } else {
      y = ((Board1_DW.global_state.stateB2.button3 &&
            (Board1_DW.global_state.stateB2.controller_battery >
             Board1_LOW_CONTROLLER_BATTERY)) ||
           Board1_DW.global_state.stateB2.controllerError);
    }
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Low_Controller_Battery_Routine(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.global_state.stateB2.controller_battery <=
     Board1_LOW_CONTROLLER_BATTERY);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Spec_Retro_Routine(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.global_state.stateB2.controller_y < Board1_CONTROLLER_ZERO) &&
    Board1_DW.global_state.spc_retro;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Stationary_Obs_Routine(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent == Board1_event_STEP) {
    y = (Board1_Is_Rover_Stationary() && ((Board1_DW.global_state.stateB2.sonar2
           <= Board1_MAX_DISTANCE) &&
          (Board1_DW.global_state.stateB2.controller_y > Board1_CONTROLLER_ZERO)
          && Board1_DW.global_state.obs_detection));
  } else {
    y = false;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Process_User_Commands(void)
{
  real32_T forward;
  real32_T throttle;
  real32_T turn;
  throttle = ((real32_T)Board1_DW.global_state.stateB2.controller_y -
              Board1_CENTER) / Board1_CENTER;
  forward = throttle * (real32_T)Board1_DW.max_velocity;
  turn = ((real32_T)Board1_DW.global_state.stateB2.controller_x - Board1_CENTER)
    / Board1_CENTER * (real32_T)Board1_DW.max_velocity;
  if (fabsf(throttle) < Board1_PURE_TURN_EPS) {
    forward = 0.0F;
  } else {
    throttle = fabsf(forward) * Board1_TURN_RATIO;
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
  if (turn > Board1_DW.max_velocity) {
    turn = (real32_T)Board1_DW.max_velocity / turn;
    throttle *= turn;
    forward *= turn;
  }

  Board1_DW.decision.rif_FA = throttle;
  Board1_DW.decision.rif_BA = throttle;
  Board1_DW.decision.rif_FB = forward;
  Board1_DW.decision.rif_BB = forward;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Turn_Back(void)
{
  Board1_DW.decision.rif_FA = Board1_TURN_BACK_RPM;
  Board1_DW.decision.rif_FB = -40.0F;
  Board1_DW.decision.rif_BA = Board1_TURN_BACK_RPM;
  Board1_DW.decision.rif_BB = -40.0F;
}

/* Function for Chart: '<Root>/Board1' */
static void enter_internal_Moving_obstacle_(void)
{
  if (!Board1_DW.global_state.stateB2.gyroError) {
    Board1_DW.angle = 0.0F;
    Board1_DW.prevYaw = 0.0F;
    Board1_DW.is_Moving_obstacle_from_left_ro = Board1_IN_Turn_moving_left_gyro;
    Board1_Process_Evasive_Commands(false);
    Board1_DW.decision.brk_mode = NONE;
  } else {
    Board1_DW.turn_counter = 0U;
    Board1_DW.is_Moving_obstacle_from_left_ro = Boa_IN_Turn_moving_left_no_gyro;
    Board1_Process_Evasive_Commands(false);
    Board1_DW.decision.brk_mode = NONE;
  }
}

/* Function for Chart: '<Root>/Board1' */
static void enter_internal_Moving_obstacl_f(void)
{
  if (!Board1_DW.global_state.stateB2.gyroError) {
    Board1_DW.angle = 0.0F;
    Board1_DW.prevYaw = 0.0F;
    Board1_DW.is_Moving_obstacle_from_right_r = Board_IN_Turn_moving_right_gyro;
    Board1_Process_Evasive_Commands(true);
    Board1_DW.decision.brk_mode = NONE;
  } else {
    Board1_DW.turn_counter = 0U;
    Board1_DW.is_Moving_obstacle_from_right_r = Bo_IN_Turn_moving_right_no_gyro;
    Board1_Process_Evasive_Commands(true);
    Board1_DW.decision.brk_mode = NONE;
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Select_routine(void)
{
  boolean_T b;
  b = Board1_Emergency_B_Pressed();
  if (b) {
    Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
    Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
    Board1_Stop_Motors();
    Board1_DW.decision.brk_mode = EMERGENCY;
  } else {
    b = Board1_Emergency_S_Routine();
    if (b) {
      Board1_DW.is_Normal_routine = Boar_IN_Emergency_sonar_routine;
      Board1_DW.is_Emergency_sonar_routine = Board1_IN_Emergency_sonar;
      Board1_Stop_Motors();
      Board1_DW.decision.brk_mode = EMERGENCY;
    } else {
      b = Board1_Mov_Obs_R_Routine();
      if (b) {
        Board1_DW.is_Normal_routine = IN_Moving_obstacle_from_right_r;
        enter_internal_Moving_obstacl_f();
      } else {
        b = Board1_Mov_Obs_L_Routine();
        if (b) {
          Board1_DW.is_Normal_routine = IN_Moving_obstacle_from_left_ro;
          enter_internal_Moving_obstacle_();
        } else {
          b = Board1_Stop_Slow_Routine();
          if (b) {
            Board1_DW.is_Normal_routine = Board1_IN_Stop_slow_routine;
            Board1_DW.is_Stop_slow_routine = Board1_IN_Stop_slow;
            Board1_Stop_Motors();
            Board1_DW.decision.brk_mode = NORMAL;
          } else {
            b = Low_Controller_Battery_Routine();
            if (b) {
              Board1_DW.is_Normal_routine = IN_Low_controller_battery_routi;
              Board1_DW.is_Low_controller_battery_routi =
                Board1_IN_Control_battery_stop;
              Board1_Stop_Motors();
              Board1_DW.decision.brk_mode = NORMAL;
            } else {
              b = Board1_Spec_Retro_Routine();
              if (b) {
                Board1_DW.is_Normal_routine = Board1_IN_Special_retro_routine;
                Board1_DW.special_retro_rotating = true;
                if (!Board1_DW.global_state.stateB2.gyroError) {
                  Board1_DW.angle = 0.0F;
                  Board1_DW.prevYaw = 0.0F;
                  Board1_DW.is_Special_retro_routine = Board1_IN_Turn_back_gyro;
                  Board1_Turn_Back();
                  Board1_DW.decision.brk_mode = NONE;
                } else {
                  Board1_DW.turn_counter = 0U;
                  Board1_DW.is_Special_retro_routine =
                    Board1_IN_Turn_back_no_gyro;
                  Board1_Turn_Back();
                  Board1_DW.decision.brk_mode = NONE;
                }
              } else {
                b = Board1_Stationary_Obs_Routine();
                if (b) {
                  Board1_DW.is_Normal_routine = Board1_IN_Not_moving_routine;
                  Board1_DW.is_Not_moving_routine = Board1_IN_Not_moving;
                  Board1_Stop_Motors();
                  Board1_DW.decision.brk_mode = NORMAL;
                } else if (Board1_DW.sfEvent == Board1_event_STEP) {
                  Board1_DW.is_Normal_routine = B_IN_Control_controller_routine;
                  Board1_DW.is_Control_controller_routine =
                    Boar_IN_Control_from_controller;
                  Board1_Process_User_Commands();
                  Board1_DW.decision.brk_mode = NONE;
                }
              }
            }
          }
        }
      }
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Special_retro_routine(void)
{
  uint32_T qY;
  boolean_T b;
  b = Board1_Emergency_B_Pressed();
  if (b) {
    Board1_DW.special_retro_rotating = false;
    Board1_DW.is_Special_retro_routine = Board1_IN_NO_ACTIVE_CHILD;
    Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
    Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
    Board1_Stop_Motors();
    Board1_DW.decision.brk_mode = EMERGENCY;
  } else {
    b = Board1_Stop_B_Pressed();
    if (b) {
      Board1_DW.special_retro_rotating = false;
      Board1_DW.is_Special_retro_routine = Board1_IN_NO_ACTIVE_CHILD;
      Board1_DW.is_Normal_routine = Board1_IN_Stop_slow_routine;
      Board1_DW.is_Stop_slow_routine = Board1_IN_Stop_slow;
      Board1_Stop_Motors();
      Board1_DW.decision.brk_mode = NORMAL;
    } else {
      switch (Board1_DW.is_Special_retro_routine) {
       case Board1_IN_Stop_back_rotation:
        if ((Board1_DW.global_state.stateB2.controller_y >=
             Board1_CONTROLLER_ZERO) ||
            (Board1_DW.global_state.stateB2.controller_x !=
             Board1_CONTROLLER_ZERO)) {
          Board1_DW.is_Special_retro_routine = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
        }
        break;

       case Board1_IN_Turn_back_gyro:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          Board1_Update_Angle(Board1_DW.global_state.stateB2.gyroYaw);
          if (fabsf(Board1_DW.angle) >= Board1_TURN_BACK_ANGLE) {
            Board1_DW.angle = 0.0F;
            Board1_DW.prevYaw = 0.0F;
            Board1_DW.special_retro_rotating = false;
            Board1_DW.is_Special_retro_routine = Board1_IN_Stop_back_rotation;
            Board1_Stop_Motors();
            Board1_DW.decision.brk_mode = NONE;
          } else {
            Board1_DW.is_Special_retro_routine = Board1_IN_Turn_back_gyro;
            Board1_Turn_Back();
            Board1_DW.decision.brk_mode = NONE;
          }
        }
        break;

       case Board1_IN_Turn_back_no_gyro:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          qY = Board1_DW.turn_counter + /*MW:OvSatOk*/ 1U;
          if (Board1_DW.turn_counter + 1U < Board1_DW.turn_counter) {
            qY = MAX_uint32_T;
          }

          Board1_DW.turn_counter = qY;
          if (Board1_DW.turn_counter >= Board1_TURN_BACK_COUNT) {
            Board1_DW.turn_counter = 0U;
            Board1_DW.special_retro_rotating = false;
            Board1_DW.is_Special_retro_routine = Board1_IN_Stop_back_rotation;
            Board1_Stop_Motors();
            Board1_DW.decision.brk_mode = NONE;
          } else {
            Board1_DW.is_Special_retro_routine = Board1_IN_Turn_back_no_gyro;
            Board1_Turn_Back();
            Board1_DW.decision.brk_mode = NONE;
          }
        }
        break;
      }
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Mov_Obs_Right(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.global_state.mov_obs == MOVING_FROM_RIGHT) &&
    Board1_DW.global_state.obs_detection;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Mov_Obs_Left(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.global_state.mov_obs == MOVING_FROM_LEFT) &&
    Board1_DW.global_state.obs_detection;
}

/* Function for Chart: '<Root>/Board1' */
static void Bo_exit_internal_Normal_routine(void)
{
  Board1_DW.is_Control_controller_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Emergency_button_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Emergency_sonar_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Low_controller_battery_routi = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Moving_obstacle_from_left_ro = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Moving_obstacle_from_right_r = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Not_moving_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Special_retro_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
  Board1_DW.is_Normal_routine = Board1_IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Routine_manager(void)
{
  uint32_T qY;
  boolean_T b;
  if ((Board1_DW.is_active_Max_velocity_handler != 0) &&
      (Board1_DW.is_Max_velocity_handler == Board_IN_Waiting_change_max_vel)) {
    b = Board1_Detect_Limit_Activation();
    if (!b) {
      if ((Board1_DW.sfEvent == Board1_event_STEP) &&
          (Board1_DW.global_state.change_vel == Board1_VEL_CHANGE)) {
        Board1_Increase_Max_Vel();
      } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                 (Board1_DW.global_state.change_vel == -10)) {
        Board1_Decrease_Max_Vel();
      }
    }
  }

  if (Board1_DW.is_active_Compute_routine != 0) {
    switch (Board1_DW.is_Compute_routine) {
     case Boa_IN_Critical_voltage_routine:
      b = !Board1_Critical_Voltage_Occured();
      if (b) {
        Board1_DW.is_Compute_routine = Board1_IN_Normal_routine;
        Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
      }
      break;

     case Board1_IN_Motor_error_routine:
      break;

     case Board1_IN_Normal_routine:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Motor_Error_Occured();
      } else {
        b = false;
      }

      if (b) {
        Bo_exit_internal_Normal_routine();
        Board1_DW.is_Compute_routine = Board1_IN_Motor_error_routine;
        Board1_Stop_Motors();
        Board1_DW.decision.brk_mode = EMERGENCY;
      } else {
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Critical_Voltage_Occured();
        } else {
          b = false;
        }

        if (b) {
          Bo_exit_internal_Normal_routine();
          Board1_DW.is_Compute_routine = Boa_IN_Critical_voltage_routine;
          Board1_Stop_Motors();
          Board1_DW.decision.brk_mode = EMERGENCY;
        } else {
          switch (Board1_DW.is_Normal_routine) {
           case B_IN_Control_controller_routine:
            if (Board1_DW.is_Control_controller_routine ==
                Boar_IN_Control_from_controller) {
              Board1_DW.is_Control_controller_routine =
                Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
            }
            break;

           case Boa_IN_Emergency_button_routine:
            if (Board1_DW.is_Emergency_button_routine ==
                Board1_IN_Emergency_button) {
              b = Board1_Is_Rover_Stationary();
              if (b) {
                Board1_DW.is_Emergency_button_routine =
                  Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
              }
            }
            break;

           case Boar_IN_Emergency_sonar_routine:
            Board1_Emergency_sonar_routine();
            break;

           case IN_Low_controller_battery_routi:
            if (Board1_DW.is_Low_controller_battery_routi ==
                Board1_IN_Control_battery_stop) {
              Board1_DW.is_Low_controller_battery_routi =
                Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
            }
            break;

           case IN_Moving_obstacle_from_left_ro:
            b = Board1_Emergency_B_Pressed();
            if (b) {
              Board1_DW.is_Moving_obstacle_from_left_ro =
                Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
              Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
              Board1_Stop_Motors();
              Board1_DW.decision.brk_mode = EMERGENCY;
            } else {
              b = Board1_Near_Obstacle();
              if (b) {
                Board1_DW.is_Moving_obstacle_from_left_ro =
                  Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Normal_routine = Boar_IN_Emergency_sonar_routine;
                Board1_DW.is_Emergency_sonar_routine = Board1_IN_Emergency_sonar;
                Board1_Stop_Motors();
                Board1_DW.decision.brk_mode = EMERGENCY;
              } else {
                b = Board1_Stop_B_Pressed();
                if (b) {
                  Board1_DW.is_Moving_obstacle_from_left_ro =
                    Board1_IN_NO_ACTIVE_CHILD;
                  Board1_DW.is_Normal_routine = Board1_IN_Stop_slow_routine;
                  Board1_DW.is_Stop_slow_routine = Board1_IN_Stop_slow;
                  Board1_Stop_Motors();
                  Board1_DW.decision.brk_mode = NORMAL;
                } else {
                  switch (Board1_DW.is_Moving_obstacle_from_left_ro) {
                   case Board1_IN_Turn_moving_left_gyro:
                    if (Board1_DW.sfEvent == Board1_event_STEP) {
                      Board1_Update_Angle(Board1_DW.global_state.stateB2.gyroYaw);
                      if (fabsf(Board1_DW.angle) >= Board1_TURN_ANGLE) {
                        Board1_DW.angle = 0.0F;
                        Board1_DW.prevYaw = 0.0F;
                        Board1_DW.is_Moving_obstacle_from_left_ro =
                          Board1_IN_NO_ACTIVE_CHILD;
                        Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
                      } else {
                        Board1_DW.is_Moving_obstacle_from_left_ro =
                          Board1_IN_Turn_moving_left_gyro;
                        Board1_Process_Evasive_Commands(false);
                        Board1_DW.decision.brk_mode = NONE;
                      }
                    }
                    break;

                   case Boa_IN_Turn_moving_left_no_gyro:
                    if (Board1_DW.sfEvent == Board1_event_STEP) {
                      qY = Board1_DW.turn_counter + /*MW:OvSatOk*/ 1U;
                      if (Board1_DW.turn_counter + 1U < Board1_DW.turn_counter)
                      {
                        qY = MAX_uint32_T;
                      }

                      Board1_DW.turn_counter = qY;
                      if (Board1_DW.turn_counter >= Board1_TURN_COUNT) {
                        Board1_DW.turn_counter = 0U;
                        Board1_DW.is_Moving_obstacle_from_left_ro =
                          Board1_IN_NO_ACTIVE_CHILD;
                        Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
                      } else {
                        Board1_DW.is_Moving_obstacle_from_left_ro =
                          Boa_IN_Turn_moving_left_no_gyro;
                        Board1_Process_Evasive_Commands(false);
                        Board1_DW.decision.brk_mode = NONE;
                      }
                    }
                    break;
                  }
                }
              }
            }
            break;

           case IN_Moving_obstacle_from_right_r:
            Moving_obstacle_from_right_rout();
            break;

           case Board1_IN_Not_moving_routine:
            if (Board1_DW.is_Not_moving_routine == Board1_IN_Not_moving) {
              Board1_DW.is_Not_moving_routine = Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
            }
            break;

           case Board1_IN_Select_routine:
            Board1_Select_routine();
            break;

           case Board1_IN_Special_retro_routine:
            Board1_Special_retro_routine();
            break;

           case Board1_IN_Stop_slow_routine:
            b = Board1_Emergency_B_Pressed();
            if (b) {
              Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Normal_routine = Boa_IN_Emergency_button_routine;
              Board1_DW.is_Emergency_button_routine = Board1_IN_Emergency_button;
              Board1_Stop_Motors();
              Board1_DW.decision.brk_mode = EMERGENCY;
            } else {
              b = Board1_Near_Obstacle();
              if (b) {
                Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Normal_routine = Boar_IN_Emergency_sonar_routine;
                Board1_DW.is_Emergency_sonar_routine = Board1_IN_Emergency_sonar;
                Board1_Stop_Motors();
                Board1_DW.decision.brk_mode = EMERGENCY;
              } else {
                b = Board1_Mov_Obs_Right();
                if (b) {
                  Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
                  Board1_DW.is_Normal_routine = IN_Moving_obstacle_from_right_r;
                  enter_internal_Moving_obstacl_f();
                } else {
                  b = Board1_Mov_Obs_Left();
                  if (b) {
                    Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
                    Board1_DW.is_Normal_routine =
                      IN_Moving_obstacle_from_left_ro;
                    enter_internal_Moving_obstacle_();
                  } else if (Board1_DW.is_Stop_slow_routine ==
                             Board1_IN_Stop_slow) {
                    b = Board1_Is_Rover_Stationary();
                    if (b) {
                      Board1_DW.is_Stop_slow_routine = Board1_IN_NO_ACTIVE_CHILD;
                      Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
                    }
                  }
                }
              }
            }
            break;
          }
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Mode_B_Pressed(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.global_state.stateB2.l_stick_button &&
         (!Board1_DW.prev_l_stick_button));
    Board1_DW.prev_l_stick_button =
      Board1_DW.global_state.stateB2.l_stick_button;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Rover_Lights_OFF(void)
{
  Board1_DW.decision.led_A = OFF;
  Board1_DW.decision.led_B = OFF;
  Board1_DW.decision.rear_led = IDLE;
  Board1_DW.decision.rear_sign = SIGN_OFF;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Lights_B_Pressed(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.global_state.stateB2.r_stick_button &&
         (!Board1_DW.prev_r_stick_button));
    Board1_DW.prev_r_stick_button =
      Board1_DW.global_state.stateB2.r_stick_button;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Update_Rover_Lights(boolean_T white_led_when_stopped)
{
  boolean_T x[4];
  if (Board1_DW.special_retro_rotating) {
    Board1_DW.decision.led_A = BLINKING_WHITE;
    Board1_DW.decision.led_B = BLINKING_WHITE;
    Board1_DW.decision.rear_led = SPECIAL_LIGHTS;
  } else if (Board1_DW.decision.brk_mode != NONE) {
    Board1_DW.decision.led_A = WHITE;
    Board1_DW.decision.led_B = WHITE;
    Board1_DW.decision.rear_led = BRAKING_LIGHTS;
  } else if (((Board1_DW.decision.rif_FA + Board1_DW.decision.rif_BA) / 2.0F <
              0.0F) && ((Board1_DW.decision.rif_FB + Board1_DW.decision.rif_BB) /
                        2.0F > 1.0F)) {
    Board1_DW.decision.led_A = BLINKING_RED;
    Board1_DW.decision.led_B = WHITE;
    Board1_DW.decision.rear_led = ARROW_LEFT;
  } else if (((Board1_DW.decision.rif_FA + Board1_DW.decision.rif_BA) / 2.0F >
              1.0F) && ((Board1_DW.decision.rif_FB + Board1_DW.decision.rif_BB) /
                        2.0F < 0.0F)) {
    Board1_DW.decision.led_A = WHITE;
    Board1_DW.decision.led_B = BLINKING_RED;
    Board1_DW.decision.rear_led = ARROW_RIGHT;
  } else {
    real32_T left_ref;
    real32_T right_ref;
    boolean_T e_y;
    left_ref = (Board1_DW.decision.rif_FA + Board1_DW.decision.rif_BA) / 2.0F;
    right_ref = (Board1_DW.decision.rif_FB + Board1_DW.decision.rif_BB) / 2.0F;
    e_y = ((left_ref > 1.0F) && (right_ref > 1.0F));
    if (e_y && (right_ref - left_ref > Board1_TURN_THRESHOLD)) {
      Board1_DW.decision.led_A = BLINKING_RED;
      Board1_DW.decision.led_B = WHITE;
      Board1_DW.decision.rear_led = ARROW_LEFT;
    } else if (e_y && ((Board1_DW.decision.rif_FA + Board1_DW.decision.rif_BA) /
                       2.0F - (Board1_DW.decision.rif_FB +
                 Board1_DW.decision.rif_BB) / 2.0F > Board1_TURN_THRESHOLD)) {
      Board1_DW.decision.led_A = WHITE;
      Board1_DW.decision.led_B = BLINKING_RED;
      Board1_DW.decision.rear_led = ARROW_RIGHT;
    } else {
      int32_T k;
      boolean_T exitg1;
      x[0] = (Board1_DW.decision.rif_FA < 0.0F);
      x[1] = (Board1_DW.decision.rif_FB < 0.0F);
      x[2] = (Board1_DW.decision.rif_BA < 0.0F);
      x[3] = (Board1_DW.decision.rif_BB < 0.0F);
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
        Board1_DW.decision.led_A = WHITE;
        Board1_DW.decision.led_B = WHITE;
        Board1_DW.decision.rear_led = BACKWARD_LIGHTS;
      } else {
        x[0] = (Board1_DW.decision.rif_FA > 1.0F);
        x[1] = (Board1_DW.decision.rif_FB > 1.0F);
        x[2] = (Board1_DW.decision.rif_BA > 1.0F);
        x[3] = (Board1_DW.decision.rif_BB > 1.0F);
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

        if (e_y && (Board1_DW.decision.rif_FA == Board1_DW.decision.rif_FB) &&
            (Board1_DW.decision.rif_FA == Board1_DW.decision.rif_BA) &&
            (Board1_DW.decision.rif_FA == Board1_DW.decision.rif_BB)) {
          Board1_DW.decision.led_A = WHITE;
          Board1_DW.decision.led_B = WHITE;
          Board1_DW.decision.rear_led = BACKLIGHTS;
        } else {
          if (white_led_when_stopped) {
            Board1_DW.decision.led_A = WHITE;
            Board1_DW.decision.led_B = WHITE;
          } else {
            Board1_DW.decision.led_A = OFF;
            Board1_DW.decision.led_B = OFF;
          }

          Board1_DW.decision.rear_led = BACKLIGHTS;
        }
      }
    }
  }

  switch (Board1_DW.decision.mode) {
   case DEFAULT:
    Board1_DW.decision.rear_sign = SIGN_WHITE;
    break;

   case SPORT:
    Board1_DW.decision.rear_sign = SIGN_ORANGE;
    break;

   default:
    Board1_DW.decision.rear_sign = SIGN_GREEN;
    break;
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Rover_Lights_Motor_Error(void)
{
  Board1_DW.decision.led_A = BLINKING_RED;
  Board1_DW.decision.led_B = BLINKING_RED;
  Board1_DW.decision.rear_led = EMERGENCY_LIGHTS;
  Board1_DW.decision.rear_sign = SIGN_RED;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Board_decision(void)
{
  boolean_T b;
  if (Board1_DW.is_active_Working_status_manage != 0) {
    switch (Board1_DW.is_Working_status_manager) {
     case Boa_IN_Critical_voltage_working:
      b = !Board1_Critical_Voltage();
      if (b) {
        Board1_DW.is_Working_status_manager = Board1_IN_Normal_working;
        Board1_DW.working_status = NORMAL_WORKING;
      }
      break;

     case Board1_IN_Motor_error_working:
      break;

     case Board1_IN_Normal_working:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Motor_Error();
      } else {
        b = false;
      }

      if (b) {
        Board1_DW.is_Working_status_manager = Board1_IN_Motor_error_working;
        Board1_DW.working_status = MOTOR_ERROR_WORKING;
      } else {
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Critical_Voltage();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Working_status_manager = Boa_IN_Critical_voltage_working;
          Board1_DW.working_status = CRITICAL_VOLTAGE_WORKING;
        }
      }
      break;
    }
  }

  if (Board1_DW.is_active_Routine_manager != 0) {
    Board1_Routine_manager();
  }

  if (Board1_DW.is_active_Mode_manager != 0) {
    switch (Board1_DW.is_Mode_manager) {
     case Boa_IN_Critical_voltage_driving:
      b = !Board1_Critical_Voltage_Occured();
      if (b) {
        Board1_DW.is_Mode_manager = Board1_IN_Normal_driving;
        Board1_DW.is_Normal_driving = Board1_IN_Mode_DEFAULT;
        Board1_DW.decision.mode = DEFAULT;
      }
      break;

     case Board1_IN_Motor_error_driving:
      break;

     case Board1_IN_Normal_driving:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Motor_Error_Occured();
      } else {
        b = false;
      }

      if (b) {
        Board1_DW.is_Normal_driving = Board1_IN_NO_ACTIVE_CHILD;
        Board1_DW.is_Mode_manager = Board1_IN_Motor_error_driving;
        Board1_DW.decision.mode = DEFAULT;
      } else {
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Critical_Voltage_Occured();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Normal_driving = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.is_Mode_manager = Boa_IN_Critical_voltage_driving;
          Board1_DW.decision.mode = ECO;
        } else {
          switch (Board1_DW.is_Normal_driving) {
           case Board1_IN_Mode_DEFAULT:
            b = Board1_Mode_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_driving = Board1_IN_Mode_SPORT;
              Board1_DW.decision.mode = SPORT;
            }
            break;

           case Board1_IN_Mode_ECO:
            b = Board1_Mode_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_driving = Board1_IN_Mode_DEFAULT;
              Board1_DW.decision.mode = DEFAULT;
            }
            break;

           case Board1_IN_Mode_SPORT:
            b = Board1_Mode_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_driving = Board1_IN_Mode_ECO;
              Board1_DW.decision.mode = ECO;
            }
            break;
          }
        }
      }
      break;
    }
  }

  if (Board1_DW.is_active_Lights_manager != 0) {
    switch (Board1_DW.is_Lights_manager) {
     case Boar_IN_Critical_voltage_lights:
      b = !Board1_Critical_Voltage_Occured();
      if (b) {
        Board1_DW.is_Lights_manager = Board1_IN_Normal_lights;
        Board1_DW.is_Normal_lights = Board1_IN_Lights_OFF;
        Board1_Rover_Lights_OFF();
      }
      break;

     case Board1_IN_Motor_error_lights:
      break;

     case Board1_IN_Normal_lights:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Motor_Error_Occured();
      } else {
        b = false;
      }

      if (b) {
        Board1_DW.is_Normal_lights = Board1_IN_NO_ACTIVE_CHILD;
        Board1_DW.is_Lights_manager = Board1_IN_Motor_error_lights;
        Board1_Rover_Lights_Motor_Error();
      } else {
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Critical_Voltage_Occured();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Normal_lights = Board1_IN_NO_ACTIVE_CHILD;
          Board1_DW.is_Lights_manager = Boar_IN_Critical_voltage_lights;
          Board1_Rover_Lights_OFF();
        } else {
          switch (Board1_DW.is_Normal_lights) {
           case Board1_IN_Lights_AUTO:
            b = Board1_Lights_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_lights = Board1_IN_Lights_OFF;
              Board1_Rover_Lights_OFF();
            } else if (Board1_DW.sfEvent == Board1_event_STEP) {
              Board1_DW.is_Normal_lights = Board1_IN_Lights_AUTO;
              Board1_Update_Rover_Lights(false);
            }
            break;

           case Board1_IN_Lights_OFF:
            b = Board1_Lights_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_lights = Board1_IN_Lights_ON;
              Board1_Update_Rover_Lights(true);
            }
            break;

           case Board1_IN_Lights_ON:
            b = Board1_Lights_B_Pressed();
            if (b) {
              Board1_DW.is_Normal_lights = Board1_IN_Lights_AUTO;
              Board1_Update_Rover_Lights(false);
            } else if (Board1_DW.sfEvent == Board1_event_STEP) {
              Board1_DW.is_Normal_lights = Board1_IN_Lights_ON;
              Board1_Update_Rover_Lights(true);
            }
            break;
          }
        }
      }
      break;
    }
  }

  if (Board1_DW.is_active_Relay_manager != 0) {
    switch (Board1_DW.is_Relay_manager) {
     case Board_IN_Critical_voltage_relay:
      b = !Board1_Critical_Voltage_Occured();
      if (b) {
        Board1_DW.is_Relay_manager = Board1_IN_Normal_relay;
        Board1_DW.decision.relay = true;
      }
      break;

     case Board1_IN_Motor_error_relay:
      break;

     case Board1_IN_Normal_relay:
      if (Board1_DW.sfEvent == Board1_event_STEP) {
        b = Board1_Motor_Error_Occured();
      } else {
        b = false;
      }

      if (b) {
        Board1_DW.is_Relay_manager = Board1_IN_Motor_error_relay;
        Board1_DW.decision.relay = false;
      } else {
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Critical_Voltage_Occured();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Relay_manager = Board_IN_Critical_voltage_relay;
          Board1_DW.decision.relay = false;
        }
      }
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Receive_decision(void)
{
  int32_T b_previousEvent;
  boolean_T b;
  b = Board1_Is_Rx_Finished();
  if (b) {
    b = Board_Verify_Decision_Integrity();
    if (b) {
      b_previousEvent = Board1_DW.sfEvent;
      Board1_DW.sfEvent = Board1_event_STEP;
      if (Board1_DW.is_active_Board_decision != 0) {
        Board1_Board_decision();
      }

      Board1_DW.sfEvent = b_previousEvent;
      Board1_DW.is_Supervisor = Board1_IN_Decision_received;
      Board1_setSTalk();
      Board1_DW.time_comm = Board1_Get_Timestamp();
    } else {
      Board1_DW.is_Supervisor = Board1_IN_Receive_decision;
      Board1_Wait_Decision();
      Board1_resetSTalk();
      Board1_DW.time_comm = Board1_Get_Timestamp();
    }
  } else {
    b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
      Board1_DECISION_RECEIVE_TIMEOUT);
    if (b) {
      Board1_stop_motors(true);
      Board1_exit_internal_Normal();
      Board1_Abort_Communication();
      Board1_DW.is_Board_state = Board1_IN_Degraded;
      Board1_setSTalk();
      Board1_DW.is_Degraded = Board1_IN_Restarting;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Verify_Global_Integrity(void)
{
  return CRC_Check_GlobalState(&Board1_DW.receivedGlobalStatePacket);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Obs_Sonar2(void)
{
  return (Board1_DW.receivedStatePacket.state.sonar2 >= Board1_MIN_DISTANCE) &&
    (Board1_DW.receivedStatePacket.state.sonar2 <= Board1_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Check_Timeout_Ms(uint32_T b_start_time, uint32_T
  b_min_elapsed_time)
{
  return Time_Check_Elapsed_ms(b_start_time, b_min_elapsed_time);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Obs_Sonar1(void)
{
  return (Board1_DW.receivedStatePacket.state.sonar1 >= Board1_MIN_DISTANCE) &&
    (Board1_DW.receivedStatePacket.state.sonar1 <= Board1_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Obs_Sonar3(void)
{
  return (Board1_DW.receivedStatePacket.state.sonar3 >= Board1_MIN_DISTANCE) &&
    (Board1_DW.receivedStatePacket.state.sonar3 <= Board1_MAX_DISTANCE);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Low_Voltage(void)
{
  return Board1_DW.state.battery_voltage <= Board1_LOW_VOLTAGE;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_High_Temp(void)
{
  return Board1_DW.state.temperature >= Board1_HIGH_TEMPERATURE;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Button2_Pressed_Retro(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.receivedStatePacket.state.button2 &&
         (!Board1_DW.prev_button2_retro));
    Board1_DW.prev_button2_retro = Board1_DW.receivedStatePacket.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Button1_Pressed_Retro(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.receivedStatePacket.state.button1 &&
         (!Board1_DW.prev_button1_retro));
    Board1_DW.prev_button1_retro = Board1_DW.receivedStatePacket.state.button1;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Button2_Pressed_Obs(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.receivedStatePacket.state.button1 &&
         (!Board1_DW.prev_button2_obs));
    Board1_DW.prev_button2_obs = Board1_DW.receivedStatePacket.state.button1;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Button1_Pressed_Obs(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.receivedStatePacket.state.button2 &&
         (!Board1_DW.prev_button1_obs));
    Board1_DW.prev_button1_obs = Board1_DW.receivedStatePacket.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Button1_Pressed_Vel(void)
{
  boolean_T y;
  if (Board1_DW.sfEvent != Board1_event_STEP) {
    y = false;
  } else {
    y = (Board1_DW.receivedStatePacket.state.button2 &&
         (!Board1_DW.prev_button1_vel));
    Board1_DW.prev_button1_vel = Board1_DW.receivedStatePacket.state.button2;
  }

  return y;
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_L_Stick_Up(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.receivedStatePacket.state.controller_y >=
     Board1_INCLINATION_INCREASE_VEL);
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_L_Stick_Down(void)
{
  return (Board1_DW.sfEvent == Board1_event_STEP) &&
    (Board1_DW.receivedStatePacket.state.controller_y <=
     Board1_DW.INCLINATION_DECREASE_VEL);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Combo(void)
{
  boolean_T b;
  if (Board1_DW.is_active_Special_retro != 0) {
    switch (Board1_DW.is_Special_retro) {
     case Board1_IN_First_button:
      b = Board1_Button2_Pressed_Retro();
      if (b) {
        Board1_DW.is_Special_retro = Board1_IN_Special_retro_change;
        Board1_DW.special_retro = !Board1_DW.special_retro;
      } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                 Board1_Check_Timeout_Ms(Board1_DW.time_button_retro,
                  Board1_BUTTON_TIMEOUT)) {
        Board1_DW.is_Special_retro = Board1_IN_Special_retro_start;
      }
      break;

     case Board1_IN_Special_retro_change:
      Board1_DW.is_Special_retro = Board1_IN_Special_retro_start;
      break;

     case Board1_IN_Special_retro_start:
      b = Board1_Button1_Pressed_Retro();
      if (b) {
        Board1_DW.is_Special_retro = Board1_IN_First_button;
        Board1_DW.time_button_retro = Board1_Get_Timestamp();
      }
      break;
    }
  }

  if (Board1_DW.is_active_Obstacle_detection != 0) {
    switch (Board1_DW.is_Obstacle_detection) {
     case Board1_IN_First_button:
      b = Board1_Button2_Pressed_Obs();
      if (b) {
        Board1_DW.is_Obstacle_detection = Board1_IN_Special_retro_change;
        Board1_DW.obs_detection = !Board1_DW.obs_detection;
      } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                 Board1_Check_Timeout_Ms(Board1_DW.time_button_obs,
                  Board1_BUTTON_TIMEOUT)) {
        Board1_DW.is_Obstacle_detection = Board1_IN_Special_retro_start;
      }
      break;

     case Board1_IN_Special_retro_change:
      Board1_DW.is_Obstacle_detection = Board1_IN_Special_retro_start;
      break;

     case Board1_IN_Special_retro_start:
      b = Board1_Button1_Pressed_Obs();
      if (b) {
        Board1_DW.is_Obstacle_detection = Board1_IN_First_button;
        Board1_DW.time_button_obs = Board1_Get_Timestamp();
      }
      break;
    }
  }

  if (Board1_DW.is_active_Change_max_velocity != 0) {
    switch (Board1_DW.is_Change_max_velocity) {
     case Bo_IN_Change_max_velocity_start:
      b = Board1_Button1_Pressed_Vel();
      if (b) {
        Board1_DW.is_Change_max_velocity = Board1_IN_First_button_c;
        Board1_DW.time_button_vel = Board1_Get_Timestamp();
      }
      break;

     case Board1_IN_First_button_c:
      b = Board1_L_Stick_Up();
      if (b) {
        Board1_DW.is_Change_max_velocity = Board1_IN_Max_velocity_increase;
        Board1_DW.change_velocity = (int8_T)Board1_VEL_CHANGE;
      } else {
        b = Board1_L_Stick_Down();
        if (b) {
          Board1_DW.is_Change_max_velocity = Board1_IN_Max_velocity_decrease;
          Board1_DW.change_velocity = -10;
        } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                   Board1_Check_Timeout_Ms(Board1_DW.time_button_vel,
                    Board1_BUTTON_TIMEOUT)) {
          Board1_DW.is_Change_max_velocity = Bo_IN_Change_max_velocity_start;
        }
      }
      break;

     case Board1_IN_Max_velocity_decrease:
      Board1_DW.is_Change_max_velocity = Bo_IN_Change_max_velocity_start;
      break;

     case Board1_IN_Max_velocity_increase:
      Board1_DW.is_Change_max_velocity = Bo_IN_Change_max_velocity_start;
      break;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Global_state_compute(void)
{
  boolean_T b;
  if (Board1_DW.is_active_Moving_obstacle != 0) {
    if ((Board1_DW.is_active_No_obstacle != 0) && (Board1_DW.is_No_obstacle ==
         Board1_IN_No_movements) && (Board1_DW.sfEvent == Board1_event_STEP)) {
      Board1_DW.moving_from_left = false;
      Board1_DW.moving_from_right = false;
    }

    if (Board1_DW.is_active_Obstacle_from_left != 0) {
      switch (Board1_DW.is_Obstacle_from_left) {
       case Bo_IN_Moving_obstacle_from_left:
        Board1_DW.is_Obstacle_from_left = Board1_IN_Waiting;
        break;

       case Board1_IN_Obstacle_left:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Obs_Sonar2();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Obstacle_from_left = Bo_IN_Moving_obstacle_from_left;
          Board1_DW.moving_from_left = true;
        } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                   Board1_Check_Timeout_Ms(Board1_DW.time_obs_s1,
                    Board1_OBS_TIMEOUT)) {
          Board1_DW.is_Obstacle_from_left = Board1_IN_Waiting;
        }
        break;

       case Board1_IN_Waiting:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          if (Board1_Obs_Sonar1()) {
            b = !Board1_Obs_Sonar2();
          } else {
            b = false;
          }
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Obstacle_from_left = Board1_IN_Obstacle_left;
          Board1_DW.time_obs_s1 = Board1_Get_Timestamp();
        }
        break;
      }
    }

    if (Board1_DW.is_active_Obstacle_from_right != 0) {
      switch (Board1_DW.is_Obstacle_from_right) {
       case B_IN_Moving_obstacle_from_right:
        Board1_DW.is_Obstacle_from_right = Board1_IN_Waiting;
        break;

       case Board1_IN_Obstacle_right:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Obs_Sonar2();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Obstacle_from_right = B_IN_Moving_obstacle_from_right;
          Board1_DW.moving_from_right = true;
        } else if ((Board1_DW.sfEvent == Board1_event_STEP) &&
                   Board1_Check_Timeout_Ms(Board1_DW.time_obs_s3,
                    Board1_OBS_TIMEOUT)) {
          Board1_DW.is_Obstacle_from_right = Board1_IN_Waiting;
        }
        break;

       case Board1_IN_Waiting:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          if (Board1_Obs_Sonar3()) {
            b = !Board1_Obs_Sonar2();
          } else {
            b = false;
          }
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Obstacle_from_right = Board1_IN_Obstacle_right;
          Board1_DW.time_obs_s3 = Board1_Get_Timestamp();
        }
        break;
      }
    }
  }

  if (Board1_DW.is_active_Battery_temperature_m != 0) {
    if ((Board1_DW.is_active_Normal_velocity != 0) &&
        (Board1_DW.is_Normal_velocity == Board1_IN_No_limitation) &&
        (Board1_DW.sfEvent == Board1_event_STEP)) {
      Board1_DW.limit_velocity = false;
    }

    if (Board1_DW.is_active_Battery_manager != 0) {
      switch (Board1_DW.is_Battery_manager) {
       case Board1_IN_Limited:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = !Board1_Low_Voltage();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Battery_manager = Board1_IN_Normal;
        } else if (Board1_DW.sfEvent == Board1_event_STEP) {
          Board1_DW.is_Battery_manager = Board1_IN_Limited;
          Board1_DW.limit_velocity = true;
        }
        break;

       case Board1_IN_Normal:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_Low_Voltage();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Battery_manager = Board1_IN_Limited;
          Board1_DW.limit_velocity = true;
        }
        break;
      }
    }

    if (Board1_DW.is_active_Temperature_manager != 0) {
      switch (Board1_DW.is_Temperature_manager) {
       case Board1_IN_High_temperature:
        if ((Board1_DW.sfEvent == Board1_event_STEP) && Board1_Check_Timeout_Ms
            (Board1_DW.time_temp, Board1_TEMP_TIMEOUT)) {
          Board1_DW.is_Temperature_manager = Board1_IN_Limited_l;
          Board1_DW.limit_velocity = true;
        } else {
          if (Board1_DW.sfEvent == Board1_event_STEP) {
            b = !Board1_High_Temp();
          } else {
            b = false;
          }

          if (b) {
            Board1_DW.is_Temperature_manager = Board1_IN_Normal_k;
          }
        }
        break;

       case Board1_IN_Limited_l:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = !Board1_High_Temp();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Temperature_manager = Board1_IN_Normal_k;
        } else if (Board1_DW.sfEvent == Board1_event_STEP) {
          Board1_DW.is_Temperature_manager = Board1_IN_Limited_l;
          Board1_DW.limit_velocity = true;
        }
        break;

       case Board1_IN_Normal_k:
        if (Board1_DW.sfEvent == Board1_event_STEP) {
          b = Board1_High_Temp();
        } else {
          b = false;
        }

        if (b) {
          Board1_DW.is_Temperature_manager = Board1_IN_High_temperature;
          Board1_DW.time_temp = Board1_Get_Timestamp();
        }
        break;
      }
    }
  }

  if (Board1_DW.is_active_Combo != 0) {
    Board1_Combo();
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Update_Global_State(void)
{
  Board1_DW.global_state.stateB1 = Board1_DW.state;
  Board1_DW.global_state.stateB2 = Board1_DW.receivedStatePacket.state;
  if (Board1_DW.moving_from_left && Board1_DW.moving_from_right) {
    Board1_DW.global_state.mov_obs = MOVING_FROM_BOTH;
  } else if (Board1_DW.moving_from_left) {
    Board1_DW.global_state.mov_obs = MOVING_FROM_LEFT;
  } else if (Board1_DW.moving_from_right) {
    Board1_DW.global_state.mov_obs = MOVING_FROM_RIGHT;
  } else {
    Board1_DW.global_state.mov_obs = NO_OBSTACLE;
  }

  Board1_DW.global_state.spc_retro = Board1_DW.special_retro;
  Board1_DW.global_state.limit_vel = Board1_DW.limit_velocity;
  Board1_DW.global_state.change_vel = Board1_DW.change_velocity;
  Board1_DW.global_state.obs_detection = Board1_DW.obs_detection;
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Receive_global_state(void)
{
  int32_T b_previousEvent;
  boolean_T b;
  b = Board1_Is_Rx_Finished();
  if (b) {
    b = Board1_Verify_Global_Integrity();
    if (b) {
      b_previousEvent = Board1_DW.sfEvent;
      Board1_DW.sfEvent = Board1_event_STEP;
      if (Board1_DW.is_active_Global_state_compute != 0) {
        Board1_Global_state_compute();
      }

      Board1_DW.sfEvent = b_previousEvent;
      Board1_Update_Global_State();
      Board1_DW.is_Supervisor = Board1_IN_Global_state_received;
      Board1_setSTalk();
      Board1_DW.time_comm = Board1_Get_Timestamp();
    } else {
      Board1_DW.is_Supervisor = Board1_IN_Receive_global_state;
      Board1_Wait_Global_State();
      Board1_resetSTalk();
      Board1_DW.time_comm = Board1_Get_Timestamp();
    }
  } else {
    b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
      Bo_GLOBAL_STATE_RECEIVE_TIMEOUT);
    if (b) {
      Board1_stop_motors(true);
      Board1_exit_internal_Normal();
      Board1_Abort_Communication();
      Board1_DW.is_Board_state = Board1_IN_Degraded;
      Board1_setSTalk();
      Board1_DW.is_Degraded = Board1_IN_Restarting;
    }
  }
}

/* Function for Chart: '<Root>/Board1' */
static boolean_T Board1_Verify_State_Integrity(void)
{
  return CRC_Check_State(&Board1_DW.receivedStatePacket);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Wait_State(void)
{
  UART_Wait_State(&Board1_DW.receivedStatePacket);
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Update_Local_State(void)
{
  OS_Enter_Critical();

  /* Inport: '<Root>/battery_voltage' */
  Board1_DW.state.battery_voltage = Board1_U.battery_voltage;

  /* Inport: '<Root>/temperature' */
  Board1_DW.state.temperature = Board1_U.temperature;

  /* Inport: '<Root>/velocity_FA' */
  Board1_DW.state.velocity_FA = Board1_U.velocity_FA;

  /* Inport: '<Root>/velocity_FB' */
  Board1_DW.state.velocity_FB = Board1_U.velocity_FB;

  /* Inport: '<Root>/velocity_BA' */
  Board1_DW.state.velocity_BA = Board1_U.velocity_BA;

  /* Inport: '<Root>/velocity_BB' */
  Board1_DW.state.velocity_BB = Board1_U.velocity_BB;

  /* Inport: '<Root>/motorError_FA' */
  Board1_DW.state.motorError_FA = Board1_U.motorError_FA;

  /* Inport: '<Root>/motorError_FB' */
  Board1_DW.state.motorError_FB = Board1_U.motorError_FB;

  /* Inport: '<Root>/motorError_BA' */
  Board1_DW.state.motorError_BA = Board1_U.motorError_BA;

  /* Inport: '<Root>/motorError_BB' */
  Board1_DW.state.motorError_BB = Board1_U.motorError_BB;
  OS_Exit_Critical();
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Normal(void)
{
  boolean_T b;
  if (Board1_DW.is_active_Supervisor != 0) {
    switch (Board1_DW.is_Supervisor) {
     case Board1_IN_Decision_received:
      b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
      if (b) {
        Board1_stop_motors(true);
        Board1_exit_internal_Normal();
        Board1_Abort_Communication();
        Board1_DW.is_Board_state = Board1_IN_Degraded;
        Board1_setSTalk();
        Board1_DW.is_Degraded = Board1_IN_Restarting;
      } else {
        b = !Board1_isMTalkhigh();
        if (b) {
          Board1_DW.is_Supervisor = Board1_IN_Transmit_Decision;
          Board1_Send_Decision();
          Board1_DW.time_comm = Board1_Get_Timestamp();
        }
      }
      break;

     case Board1_IN_Decision_transmitted:
      Board1_Decision_transmitted();
      break;

     case IN_Global_Local_state_transmitt:
      Global_Local_state_transmitted();
      break;

     case Board1_IN_Global_state_received:
      b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
      if (b) {
        Board1_stop_motors(true);
        Board1_exit_internal_Normal();
        Board1_Abort_Communication();
        Board1_DW.is_Board_state = Board1_IN_Degraded;
        Board1_setSTalk();
        Board1_DW.is_Degraded = Board1_IN_Restarting;
      } else {
        b = !Board1_isMTalkhigh();
        if (b) {
          Board1_DW.is_Supervisor = Board1_IN_Transmit_Global_State;
          Board1_Send_Global_State();
          Board1_DW.time_comm = Board1_Get_Timestamp();
        }
      }
      break;

     case Boar_IN_Local_state_transmitted:
      b = Board1_isMTalkhigh();
      if (b) {
        Board1_DW.is_Supervisor = Board1_IN_Receive_global_state;
        Board1_Wait_Global_State();
        Board1_resetSTalk();
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
        if (b) {
          if (Board1_DW.retransmitted < Board1_MAX_RETRANSMIT) {
            Board1_DW.retransmitted = 1U;
            Board1_DW.is_Supervisor = Board1_IN_Transmit_Local_State;
            Board1_Send_Local_State();
            Board1_DW.time_comm = Board1_Get_Timestamp();
          } else {
            Board1_stop_motors(true);
            Board1_exit_internal_Normal();
            Board1_Abort_Communication();
            Board1_DW.is_Board_state = Board1_IN_Degraded;
            Board1_setSTalk();
            Board1_DW.is_Degraded = Board1_IN_Restarting;
          }
        }
      }
      break;

     case Board1_IN_Receive_decision:
      Board1_Receive_decision();
      break;

     case Board1_IN_Receive_global_state:
      Board1_Receive_global_state();
      break;

     case Board1_IN_Receive_state:
      b = Board1_Is_Rx_Finished();
      if (b) {
        b = Board1_Verify_State_Integrity();
        if (b) {
          Board1_Update_Local_State();
          Board1_DW.is_Supervisor = Board1_IN_Received_state;
          Board1_setSTalk();
          Board1_DW.time_comm = Board1_Get_Timestamp();
        } else {
          Board1_DW.is_Supervisor = Board1_IN_Receive_state;
          Board1_Wait_State();
          Board1_resetSTalk();
          Board1_DW.time_comm = Board1_Get_Timestamp();
        }
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
          Board1_STATE_RECEIVE_TIMEOUT);
        if (b) {
          Board1_stop_motors(true);
          Board1_exit_internal_Normal();
          Board1_Abort_Communication();
          Board1_DW.is_Board_state = Board1_IN_Degraded;
          Board1_setSTalk();
          Board1_DW.is_Degraded = Board1_IN_Restarting;
        }
      }
      break;

     case Board1_IN_Received_state:
      b = !Board1_isMTalkhigh();
      if (b) {
        Board1_DW.is_Supervisor = Board1_IN_Transmit_Local_State;
        Board1_Send_Local_State();
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm, Board1_WAIT_TIMEOUT);
        if (b) {
          Board1_stop_motors(true);
          Board1_exit_internal_Normal();
          Board1_Abort_Communication();
          Board1_DW.is_Board_state = Board1_IN_Degraded;
          Board1_setSTalk();
          Board1_DW.is_Degraded = Board1_IN_Restarting;
        }
      }
      break;

     case Board1_IN_Same_decision:
      Board1_DW.is_Supervisor = Board1_IN_Waiting_to_start;
      break;

     case Board1_IN_Transmit_Decision:
      b = Board1_Is_Tx_Finished();
      if (b) {
        Board1_DW.is_Supervisor = Board1_IN_Decision_transmitted;
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
          Board1_DECISION_SEND_TIMEOUT);
        if (b) {
          Board1_stop_motors(true);
          Board1_exit_internal_Normal();
          Board1_Abort_Communication();
          Board1_DW.is_Board_state = Board1_IN_Degraded;
          Board1_setSTalk();
          Board1_DW.is_Degraded = Board1_IN_Restarting;
        }
      }
      break;

     case Board1_IN_Transmit_Global_State:
      b = Board1_Is_Tx_Finished();
      if (b) {
        Board1_DW.is_Supervisor = IN_Global_Local_state_transmitt;
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
          Board_GLOBAL_STATE_SEND_TIMEOUT);
        if (b) {
          Board1_stop_motors(true);
          Board1_exit_internal_Normal();
          Board1_Abort_Communication();
          Board1_DW.is_Board_state = Board1_IN_Degraded;
          Board1_setSTalk();
          Board1_DW.is_Degraded = Board1_IN_Restarting;
        }
      }
      break;

     case Board1_IN_Transmit_Local_State:
      b = Board1_Is_Tx_Finished();
      if (b) {
        Board1_DW.is_Supervisor = Boar_IN_Local_state_transmitted;
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
          Board1_STATE_SEND_TIMEOUT);
        if (b) {
          Board1_stop_motors(true);
          Board1_exit_internal_Normal();
          Board1_Abort_Communication();
          Board1_DW.is_Board_state = Board1_IN_Degraded;
          Board1_setSTalk();
          Board1_DW.is_Degraded = Board1_IN_Restarting;
        }
      }
      break;

     case Board1_IN_Waiting_to_start:
      if (Board1_isSessionhigh() && Board1_isMTalkhigh()) {
        Board1_DW.retransmitted = 0U;
        Board1_DW.is_Supervisor = Board1_IN_Receive_state;
        Board1_Wait_State();
        Board1_resetSTalk();
        Board1_DW.time_comm = Board1_Get_Timestamp();
      } else {
        Board1_stop_motors(true);
        Board1_exit_internal_Normal();
        Board1_Abort_Communication();
        Board1_DW.is_Board_state = Board1_IN_Degraded;
        Board1_setSTalk();
        Board1_DW.is_Degraded = Board1_IN_Restarting;
      }
      break;
    }
  }

  if ((Board1_DW.is_Board_state == Board1_IN_Normal) &&
      (Board1_DW.is_active_Global_state_compute != 0)) {
    Board1_Global_state_compute();
  }
}

/* Function for Chart: '<Root>/Board1' */
static void Board1_Init_Data(void)
{
  static const StateBusB1 tmp = { 0.0F,/* battery_voltage */
    0.0F,                              /* temperature */
    0,                                 /* velocity_FA */
    0,                                 /* velocity_FB */
    0,                                 /* velocity_BA */
    0,                                 /* velocity_BB */
    false,                             /* motorError_FA */
    false,                             /* motorError_FB */
    false,                             /* motorError_BA */
    false                              /* motorError_BB */
  };

  static const GSBus tmp_0 = { { 0.0F, /* battery_voltage */
      0.0F,                            /* temperature */
      0,                               /* velocity_FA */
      0,                               /* velocity_FB */
      0,                               /* velocity_BA */
      0,                               /* velocity_BB */
      false,                           /* motorError_FA */
      false,                           /* motorError_FB */
      false,                           /* motorError_BA */
      false                            /* motorError_BB */
    },                                 /* stateB1 */

    { 0.0F,                            /* gyroYaw */
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
      0U,                              /* controller_battery */
      false,                           /* controllerError */
      false                            /* gyroError */
    },                                 /* stateB2 */
    NO_OBSTACLE,                       /* mov_obs */
    false,                             /* spc_retro */
    false,                             /* limit_vel */
    0,                                 /* change_vel */
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

  static const PacketStateB2 tmp_2 = { { 0.0F,/* gyroYaw */
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
      0U,                              /* controller_battery */
      false,                           /* controllerError */
      false                            /* gyroError */
    },                                 /* state */
    0U                                 /* crc */
  };

  static const PacketGstate tmp_3 = { { { 0.0F,/* battery_voltage */
        0.0F,                          /* temperature */
        0,                             /* velocity_FA */
        0,                             /* velocity_FB */
        0,                             /* velocity_BA */
        0,                             /* velocity_BB */
        false,                         /* motorError_FA */
        false,                         /* motorError_FB */
        false,                         /* motorError_BA */
        false                          /* motorError_BB */
      },                               /* stateB1 */

      { 0.0F,                          /* gyroYaw */
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
        0U,                            /* controller_battery */
        false,                         /* controllerError */
        false                          /* gyroError */
      },                               /* stateB2 */
      NO_OBSTACLE,                     /* mov_obs */
      false,                           /* spc_retro */
      false,                           /* limit_vel */
      0,                               /* change_vel */
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

  Board1_DW.state = tmp;
  Board1_DW.global_state = tmp_0;
  Board1_DW.decision = tmp_1;
  Board1_DW.receivedStatePacket = tmp_2;
  Board1_DW.receivedGlobalStatePacket = tmp_3;
  Board1_DW.receivedDecisionPacket = tmp_4;
  Board1_DW.special_retro = true;
  Board1_DW.obs_detection = true;
  Board1_DW.max_velocity = Board1_MAX_RPM;
}

/* Model step function */
void Board1_step(void)
{
  boolean_T b;

  /* Chart: '<Root>/Board1' */
  Board1_DW.sfEvent = Board1_CALL_EVENT;
  if (Board1_DW.is_active_c2_Board1 == 0) {
    Board1_DW.is_active_c2_Board1 = 1U;
    Board1_Init_Data();
    Board1_setSTalk();
    Board1_DW.is_c2_Board1 = Board1_IN_Supervision_task;
    Board1_DW.is_active_Board_state = 1U;
    Board1_DW.is_Board_state = Board1_IN_Normal;
    Board1_enter_internal_Normal();
    Board1_DW.is_active_Board_decision = 1U;
    Board1_DW.is_active_Working_status_manage = 1U;
    Board1_DW.is_Working_status_manager = Board1_IN_Normal_working;
    Board1_DW.working_status = NORMAL_WORKING;
    Board1_DW.is_active_Routine_manager = 1U;
    Board1_DW.is_active_Max_velocity_handler = 1U;
    Board1_DW.is_Max_velocity_handler = Board_IN_Waiting_change_max_vel;
    Board1_DW.is_active_Compute_routine = 1U;
    Board1_DW.is_Compute_routine = Board1_IN_Normal_routine;
    Board1_DW.is_Normal_routine = Board1_IN_Select_routine;
    Board1_DW.is_active_Mode_manager = 1U;
    Board1_DW.is_Mode_manager = Board1_IN_Normal_driving;
    Board1_DW.is_Normal_driving = Board1_IN_Mode_DEFAULT;
    Board1_DW.decision.mode = DEFAULT;
    Board1_DW.is_active_Lights_manager = 1U;
    Board1_DW.is_Lights_manager = Board1_IN_Normal_lights;
    Board1_DW.is_Normal_lights = Board1_IN_Lights_OFF;
    Board1_Rover_Lights_OFF();
    Board1_DW.is_active_Relay_manager = 1U;
    Board1_DW.is_Relay_manager = Board1_IN_Normal_relay;
    Board1_DW.decision.relay = true;
  } else if (Board1_DW.is_c2_Board1 == Board1_IN_Supervision_task) {
    if (Board1_DW.is_active_Board_state != 0) {
      switch (Board1_DW.is_Board_state) {
       case Board1_IN_Degraded:
        switch (Board1_DW.is_Degraded) {
         case Board1_IN_Restablish:
          switch (Board1_DW.is_Restablish) {
           case Boar_IN_Connection_restablished:
            Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
            Board1_DW.is_Degraded = Board1_IN_NO_ACTIVE_CHILD;
            Board1_DW.is_Board_state = Board1_IN_Normal;
            Board1_enter_internal_Normal();
            break;

           case Board1_IN_Ping_transmitted:
            b = !Board1_isSessionhigh();
            if (b) {
              Board1_DW.is_Restablish = Boar_IN_Connection_restablished;
            } else {
              b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
                Board1_WAIT_TIMEOUT);
              if (b) {
                if (Board1_DW.retransmitted < Board1_MAX_RETRANSMIT) {
                  Board1_DW.retransmitted = 1U;
                  Board1_DW.is_Restablish = Board1_IN_Transmit_ping;
                  Board1_Send_Ping();
                  Board1_DW.time_comm = Board1_Get_Timestamp();
                } else {
                  Board1_Abort_Communication();
                  Board1_setSTalk();
                  Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
                  Board1_DW.is_Degraded = Board1_IN_Restarting;
                }
              }
            }
            break;

           case Board1_IN_Receive_ping:
            b = Board1_Is_Rx_Finished();
            if (b) {
              b = Board1_Verify_Ping();
              if (b) {
                Board1_DW.is_Restablish = Board1_IN_Received_Ping;
                Board1_setSTalk();
                Board1_DW.time_comm = Board1_Get_Timestamp();
              } else {
                Board1_DW.is_Restablish = Board1_IN_Receive_ping;
                Board1_Wait_Ping();
                Board1_resetSTalk();
                Board1_DW.time_comm = Board1_Get_Timestamp();
              }
            } else {
              b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
                Board1_BYTE_RECEIVE_TIMEOUT);
              if (b) {
                Board1_Abort_Communication();
                Board1_setSTalk();
                Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Degraded = Board1_IN_Restarting;
              }
            }
            break;

           case Board1_IN_Received_Ping:
            b = !Board1_isMTalkhigh();
            if (b) {
              Board1_DW.is_Restablish = Board1_IN_Transmit_ping;
              Board1_Send_Ping();
              Board1_DW.time_comm = Board1_Get_Timestamp();
            } else {
              b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
                Board1_WAIT_TIMEOUT);
              if (b) {
                Board1_Abort_Communication();
                Board1_setSTalk();
                Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Degraded = Board1_IN_Restarting;
              }
            }
            break;

           case Board1_IN_Transmit_ping:
            b = Board1_Is_Tx_Finished();
            if (b) {
              Board1_DW.is_Restablish = Board1_IN_Ping_transmitted;
              Board1_DW.time_comm = Board1_Get_Timestamp();
            } else {
              b = Board1_Check_Timeout_Us(Board1_DW.time_comm,
                Board1_BYTE_SEND_TIMEOUT);
              if (b) {
                Board1_Abort_Communication();
                Board1_setSTalk();
                Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
                Board1_DW.is_Degraded = Board1_IN_Restarting;
              }
            }
            break;

           case Board1_IN_Waiting_to_restablish:
            if (Board1_isSessionhigh() && Board1_isMTalkhigh()) {
              Board1_DW.retransmitted = 0U;
              Board1_DW.is_Restablish = Board1_IN_Receive_ping;
              Board1_Wait_Ping();
              Board1_resetSTalk();
              Board1_DW.time_comm = Board1_Get_Timestamp();
            } else {
              Board1_Abort_Communication();
              Board1_setSTalk();
              Board1_DW.is_Restablish = Board1_IN_NO_ACTIVE_CHILD;
              Board1_DW.is_Degraded = Board1_IN_Restarting;
            }
            break;
          }
          break;

         case Board1_IN_Restarting:
          Board1_DW.is_Degraded = Board1_IN_Restablish;
          Board1_stop_motors(true);
          Board1_DW.is_Restablish = Board1_IN_Waiting_to_restablish;
          break;
        }
        break;

       case Board1_IN_Normal:
        Board1_Normal();
        break;

       case Board1_IN_Single_Board:
        if (Board1_DW.is_Single_Board == Board1_IN_Other_board_failure) {
          Board1_stop_motors(false);
        }
        break;
      }
    }

    if (Board1_DW.is_active_Board_decision != 0) {
      Board1_Board_decision();
    }
  }

  /* End of Chart: '<Root>/Board1' */
}

/* Model initialize function */
void Board1_initialize(void)
{
  /* SystemInitialize for Chart: '<Root>/Board1' */
  Board1_DW.sfEvent = Board1_CALL_EVENT;
  Board1_DW.INCLINATION_DECREASE_VEL = 60U;
  Board1_DW.MIN_RPM = 50U;
}

/* Model terminate function */
void Board1_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
