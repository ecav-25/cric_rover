/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board1.h
 *
 * Code generated for Simulink model 'Board1'.
 *
 * Model version                  : 1.2472
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Mon Feb  9 18:11:06 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Board1_h_
#define Board1_h_
#ifndef Board1_COMMON_INCLUDES_
#define Board1_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#include "board1_functions.h"
#endif                                 /* Board1_COMMON_INCLUDES_ */

#include "Board1_types.h"


/* --- Stateflow States --- */
#define B_IN_Control_controller_routine ((uint8_T)1U)
#define B_IN_Moving_obstacle_from_right ((uint8_T)1U)
#define Bo_IN_Moving_obstacle_from_left ((uint8_T)1U)
#define Bo_IN_Turn_moving_right_no_gyro ((uint8_T)2U)
#define Boa_IN_Critical_voltage_driving ((uint8_T)1U)
#define Boa_IN_Critical_voltage_routine ((uint8_T)1U)
#define Boa_IN_Critical_voltage_working ((uint8_T)1U)
#define Boa_IN_Emergency_button_routine ((uint8_T)2U)
#define Boa_IN_Turn_moving_left_no_gyro ((uint8_T)2U)
#define Boar_IN_Connection_restablished ((uint8_T)1U)
#define Boar_IN_Control_from_controller ((uint8_T)1U)
#define Boar_IN_Critical_voltage_lights ((uint8_T)1U)
#define Boar_IN_Emergency_sonar_routine ((uint8_T)3U)
#define Boar_IN_Local_state_transmitted ((uint8_T)5U)
#define Board1_IN_Button1_pressed_combo ((uint8_T)1U)
#define Board1_IN_Button2_pressed_combo ((uint8_T)2U)
#define Board1_IN_Control_battery_stop ((uint8_T)1U)
#define Board1_IN_Decision_received    ((uint8_T)1U)
#define Board1_IN_Decision_transmitted ((uint8_T)2U)
#define Board1_IN_Degraded             ((uint8_T)1U)
#define Board1_IN_Degraded_mux         ((uint8_T)1U)
#define Board1_IN_Emergency_button     ((uint8_T)1U)
#define Board1_IN_Emergency_sonar      ((uint8_T)1U)
#define Board1_IN_Global_state_received ((uint8_T)4U)
#define Board1_IN_High_temperature     ((uint8_T)1U)
#define Board1_IN_Lights_AUTO          ((uint8_T)1U)
#define Board1_IN_Lights_OFF           ((uint8_T)2U)
#define Board1_IN_Lights_ON            ((uint8_T)3U)
#define Board1_IN_Limited              ((uint8_T)1U)
#define Board1_IN_Limited_l            ((uint8_T)2U)
#define Board1_IN_Max_velocity_decrease ((uint8_T)3U)
#define Board1_IN_Max_velocity_increase ((uint8_T)4U)
#define Board1_IN_Mode_DEFAULT         ((uint8_T)1U)
#define Board1_IN_Mode_ECO             ((uint8_T)2U)
#define Board1_IN_Mode_SPORT           ((uint8_T)3U)
#define Board1_IN_Motor_error_driving  ((uint8_T)2U)
#define Board1_IN_Motor_error_lights   ((uint8_T)2U)
#define Board1_IN_Motor_error_relay    ((uint8_T)2U)
#define Board1_IN_Motor_error_routine  ((uint8_T)2U)
#define Board1_IN_Motor_error_working  ((uint8_T)2U)
#define Board1_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board1_IN_No_limitation        ((uint8_T)1U)
#define Board1_IN_No_movements         ((uint8_T)1U)
#define Board1_IN_Normal               ((uint8_T)2U)
#define Board1_IN_Normal_driving       ((uint8_T)3U)
#define Board1_IN_Normal_k             ((uint8_T)3U)
#define Board1_IN_Normal_lights        ((uint8_T)3U)
#define Board1_IN_Normal_mux           ((uint8_T)2U)
#define Board1_IN_Normal_relay         ((uint8_T)3U)
#define Board1_IN_Normal_routine       ((uint8_T)3U)
#define Board1_IN_Normal_working       ((uint8_T)3U)
#define Board1_IN_Not_moving           ((uint8_T)1U)
#define Board1_IN_Not_moving_routine   ((uint8_T)7U)
#define Board1_IN_Obs_detection_change ((uint8_T)5U)
#define Board1_IN_Obstacle_left        ((uint8_T)2U)
#define Board1_IN_Obstacle_right       ((uint8_T)2U)
#define Board1_IN_Other_board_failure  ((uint8_T)1U)
#define Board1_IN_Ping_transmitted     ((uint8_T)2U)
#define Board1_IN_Receive_decision     ((uint8_T)6U)
#define Board1_IN_Receive_global_state ((uint8_T)7U)
#define Board1_IN_Receive_ping         ((uint8_T)3U)
#define Board1_IN_Receive_state        ((uint8_T)8U)
#define Board1_IN_Received_Ping        ((uint8_T)4U)
#define Board1_IN_Received_state       ((uint8_T)9U)
#define Board1_IN_Restablish           ((uint8_T)1U)
#define Board1_IN_Restarting           ((uint8_T)2U)
#define Board1_IN_Routine_state_normal ((uint8_T)1U)
#define Board1_IN_Same_decision        ((uint8_T)10U)
#define Board1_IN_Select_routine       ((uint8_T)8U)
#define Board1_IN_Single_Board         ((uint8_T)3U)
#define Board1_IN_Special_retro_change ((uint8_T)6U)
#define Board1_IN_Special_retro_routine ((uint8_T)9U)
#define Board1_IN_Stop_back_rotation   ((uint8_T)1U)
#define Board1_IN_Stop_left_rotation   ((uint8_T)2U)
#define Board1_IN_Stop_right_rotation  ((uint8_T)3U)
#define Board1_IN_Stop_slow            ((uint8_T)1U)
#define Board1_IN_Stop_slow_routine    ((uint8_T)10U)
#define Board1_IN_Supervision_task     ((uint8_T)1U)
#define Board1_IN_Transmit_Decision    ((uint8_T)11U)
#define Board1_IN_Transmit_Global_State ((uint8_T)12U)
#define Board1_IN_Transmit_Local_State ((uint8_T)13U)
#define Board1_IN_Transmit_ping        ((uint8_T)5U)
#define Board1_IN_Turn_back_gyro       ((uint8_T)2U)
#define Board1_IN_Turn_back_no_gyro    ((uint8_T)3U)
#define Board1_IN_Turn_left            ((uint8_T)4U)
#define Board1_IN_Turn_left_no_gyro    ((uint8_T)5U)
#define Board1_IN_Turn_moving_left_gyro ((uint8_T)1U)
#define Board1_IN_Turn_right_gyro      ((uint8_T)6U)
#define Board1_IN_Turn_right_no_gyro   ((uint8_T)7U)
#define Board1_IN_Waiting              ((uint8_T)3U)
#define Board1_IN_Waiting_combo        ((uint8_T)7U)
#define Board1_IN_Waiting_to_restablish ((uint8_T)6U)
#define Board1_IN_Waiting_to_start     ((uint8_T)14U)
#define Board_IN_Critical_voltage_relay ((uint8_T)1U)
#define Board_IN_Turn_moving_right_gyro ((uint8_T)1U)
#define Board_IN_Waiting_change_max_vel ((uint8_T)1U)
#define IN_Global_Local_state_transmitt ((uint8_T)3U)
#define IN_Low_controller_battery_routi ((uint8_T)4U)
#define IN_Moving_obstacle_from_left_ro ((uint8_T)5U)
#define IN_Moving_obstacle_from_right_r ((uint8_T)6U)
/* ------------------------------------------ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */
#include "board1_functions.h"

/* Block states (default storage) for system '<Root>' */
typedef struct {
  PacketGstate receivedGlobalStatePacket;/* '<Root>/Board1' */
  GSBus global_state;                  /* '<Root>/Board1' */
  PacketDecision receivedDecisionPacket;/* '<Root>/Board1' */
  DecBus decision;                     /* '<Root>/Board1' */
  PacketStateB2 receivedStatePacket;   /* '<Root>/Board1' */
  StateBusB1 state;                    /* '<Root>/Board1' */
  real32_T angle;                      /* '<Root>/Board1' */
  real32_T prevYaw;                    /* '<Root>/Board1' */
  int32_T sfEvent;                     /* '<Root>/Board1' */
  uint32_T time_obs_s1;                /* '<Root>/Board1' */
  uint32_T time_temp;                  /* '<Root>/Board1' */
  uint32_T time_comm;                  /* '<Root>/Board1' */
  uint32_T time_obs_s3;                /* '<Root>/Board1' */
  uint32_T time_button;                /* '<Root>/Board1' */
  WORKING_STATUS_TYPE working_status;  /* '<Root>/Board1' */
  uint16_T distance_threshold;         /* '<Root>/Board1' */
  int8_T change_velocity;              /* '<Root>/Board1' */
  uint8_T retransmitted;               /* '<Root>/Board1' */
  uint8_T receivedPing;                /* '<Root>/Board1' */
  uint8_T MIN_RPM;                     /* '<Root>/Board1' */
  uint8_T max_velocity;                /* '<Root>/Board1' */
  uint8_T turn_counter;                /* '<Root>/Board1' */
  uint8_T is_active_c2_Board1;         /* '<Root>/Board1' */
  uint8_T is_c2_Board1;                /* '<Root>/Board1' */
  uint8_T is_active_Board_state;       /* '<Root>/Board1' */
  uint8_T is_Board_state;              /* '<Root>/Board1' */
  uint8_T is_Degraded;                 /* '<Root>/Board1' */
  uint8_T is_Restablish;               /* '<Root>/Board1' */
  uint8_T is_active_Supervisor;        /* '<Root>/Board1' */
  uint8_T is_Supervisor;               /* '<Root>/Board1' */
  uint8_T is_active_Global_state_compute;/* '<Root>/Board1' */
  uint8_T is_active_Moving_obstacle;   /* '<Root>/Board1' */
  uint8_T is_active_No_obstacle;       /* '<Root>/Board1' */
  uint8_T is_No_obstacle;              /* '<Root>/Board1' */
  uint8_T is_active_Obstacle_from_left;/* '<Root>/Board1' */
  uint8_T is_Obstacle_from_left;       /* '<Root>/Board1' */
  uint8_T is_active_Obstacle_from_right;/* '<Root>/Board1' */
  uint8_T is_Obstacle_from_right;      /* '<Root>/Board1' */
  uint8_T is_active_Battery_temperature_m;/* '<Root>/Board1' */
  uint8_T is_active_Normal_velocity;   /* '<Root>/Board1' */
  uint8_T is_Normal_velocity;          /* '<Root>/Board1' */
  uint8_T is_active_Battery_manager;   /* '<Root>/Board1' */
  uint8_T is_Battery_manager;          /* '<Root>/Board1' */
  uint8_T is_active_Temperature_manager;/* '<Root>/Board1' */
  uint8_T is_Temperature_manager;      /* '<Root>/Board1' */
  uint8_T is_active_Combo;             /* '<Root>/Board1' */
  uint8_T is_Combo;                    /* '<Root>/Board1' */
  uint8_T is_Single_Board;             /* '<Root>/Board1' */
  uint8_T is_active_Board_decision;    /* '<Root>/Board1' */
  uint8_T is_active_Working_status_manage;/* '<Root>/Board1' */
  uint8_T is_Working_status_manager;   /* '<Root>/Board1' */
  uint8_T is_active_Relay_manager;     /* '<Root>/Board1' */
  uint8_T is_Relay_manager;            /* '<Root>/Board1' */
  uint8_T is_active_Mux_manager;       /* '<Root>/Board1' */
  uint8_T is_Mux_manager;              /* '<Root>/Board1' */
  uint8_T is_active_Routine_manager;   /* '<Root>/Board1' */
  uint8_T is_active_Max_velocity_handler;/* '<Root>/Board1' */
  uint8_T is_Max_velocity_handler;     /* '<Root>/Board1' */
  uint8_T is_active_Compute_routine;   /* '<Root>/Board1' */
  uint8_T is_Compute_routine;          /* '<Root>/Board1' */
  uint8_T is_Routine_state_normal;     /* '<Root>/Board1' */
  uint8_T is_Normal_routine;           /* '<Root>/Board1' */
  uint8_T is_Control_controller_routine;/* '<Root>/Board1' */
  uint8_T is_Emergency_button_routine; /* '<Root>/Board1' */
  uint8_T is_Emergency_sonar_routine;  /* '<Root>/Board1' */
  uint8_T is_Low_controller_battery_routi;/* '<Root>/Board1' */
  uint8_T is_Moving_obstacle_from_left_ro;/* '<Root>/Board1' */
  uint8_T is_Moving_obstacle_from_right_r;/* '<Root>/Board1' */
  uint8_T is_Not_moving_routine;       /* '<Root>/Board1' */
  uint8_T is_Special_retro_routine;    /* '<Root>/Board1' */
  uint8_T is_Stop_slow_routine;        /* '<Root>/Board1' */
  uint8_T is_active_Mode_manager;      /* '<Root>/Board1' */
  uint8_T is_Mode_manager;             /* '<Root>/Board1' */
  uint8_T is_Normal_driving;           /* '<Root>/Board1' */
  uint8_T is_active_Lights_manager;    /* '<Root>/Board1' */
  uint8_T is_Lights_manager;           /* '<Root>/Board1' */
  uint8_T is_Normal_lights;            /* '<Root>/Board1' */
  boolean_T special_retro;             /* '<Root>/Board1' */
  boolean_T limit_velocity;            /* '<Root>/Board1' */
  boolean_T obs_detection;             /* '<Root>/Board1' */
  boolean_T special_retro_rotating;    /* '<Root>/Board1' */
  boolean_T moving_from_left;          /* '<Root>/Board1' */
  boolean_T moving_from_right;         /* '<Root>/Board1' */
  boolean_T open_loop;                 /* '<Root>/Board1' */
  boolean_T prev_button1;              /* '<Root>/Board1' */
  boolean_T prev_button2;              /* '<Root>/Board1' */
  boolean_T prev_limit_state;          /* '<Root>/Board1' */
  boolean_T prev_l_stick_button;       /* '<Root>/Board1' */
  boolean_T prev_r_stick_button;       /* '<Root>/Board1' */
} DW_Board1_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T temperature;                /* '<Root>/temperature' */
  real32_T battery_voltage;            /* '<Root>/battery_voltage' */
  int16_T velocity_BA;                 /* '<Root>/velocity_BA' */
  int16_T velocity_BB;                 /* '<Root>/velocity_BB' */
  int16_T velocity_FA;                 /* '<Root>/velocity_FA' */
  int16_T velocity_FB;                 /* '<Root>/velocity_FB' */
  boolean_T motorError_FA;             /* '<Root>/motorError_FA' */
  boolean_T motorError_FB;             /* '<Root>/motorError_FB' */
  boolean_T motorError_BA;             /* '<Root>/motorError_BA' */
  boolean_T motorError_BB;             /* '<Root>/motorError_BB' */
} ExtU_Board1_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  DecBus output;                       /* '<Root>/output' */
} ExtY_Board1_T;

/* Real-time Model Data Structure */
struct tag_RTM_Board1_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_Board1_T Board1_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Board1_T Board1_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Board1_T Board1_Y;

/* Model entry point functions */
extern void Board1_initialize(void);
extern void Board1_step(void);
extern void Board1_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Board1_T *const Board1_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('Board1_supervision_Copia/Board1')    - opens subsystem Board1_supervision_Copia/Board1
 * hilite_system('Board1_supervision_Copia/Board1/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Board1_supervision_Copia'
 * '<S1>'   : 'Board1_supervision_Copia/Board1'
 */
#endif                                 /* Board1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
