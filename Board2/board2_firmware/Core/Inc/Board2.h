/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2.h
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 1.2419
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Feb  6 14:06:18 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Board2_h_
#define Board2_h_
#ifndef Board2_COMMON_INCLUDES_
#define Board2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#include "board2_functions.h"
#endif                                 /* Board2_COMMON_INCLUDES_ */

#include "Board2_types.h"


/* --- Stateflow States --- */
#define B_IN_Control_controller_routine ((uint8_T)1U)
#define B_IN_Moving_obstacle_from_right ((uint8_T)1U)
#define Bo_IN_Change_max_velocity_start ((uint8_T)1U)
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
#define Boar_IN_Local_state_transmitted ((uint8_T)4U)
#define Board2_IN_Control_battery_stop ((uint8_T)1U)
#define Board2_IN_Decision_transmitted ((uint8_T)1U)
#define Board2_IN_Degraded             ((uint8_T)1U)
#define Board2_IN_Emergency_button     ((uint8_T)1U)
#define Board2_IN_Emergency_sonar      ((uint8_T)1U)
#define Board2_IN_First_button         ((uint8_T)1U)
#define Board2_IN_First_button_d       ((uint8_T)2U)
#define Board2_IN_Global_state_received ((uint8_T)3U)
#define Board2_IN_High_temperature     ((uint8_T)1U)
#define Board2_IN_Lights_AUTO          ((uint8_T)1U)
#define Board2_IN_Lights_OFF           ((uint8_T)2U)
#define Board2_IN_Lights_ON            ((uint8_T)3U)
#define Board2_IN_Limited              ((uint8_T)1U)
#define Board2_IN_Limited_l            ((uint8_T)2U)
#define Board2_IN_Max_velocity_decrease ((uint8_T)3U)
#define Board2_IN_Max_velocity_increase ((uint8_T)4U)
#define Board2_IN_Mode_DEFAULT         ((uint8_T)1U)
#define Board2_IN_Mode_ECO             ((uint8_T)2U)
#define Board2_IN_Mode_SPORT           ((uint8_T)3U)
#define Board2_IN_Motor_error_driving  ((uint8_T)2U)
#define Board2_IN_Motor_error_lights   ((uint8_T)2U)
#define Board2_IN_Motor_error_relay    ((uint8_T)2U)
#define Board2_IN_Motor_error_routine  ((uint8_T)2U)
#define Board2_IN_Motor_error_working  ((uint8_T)2U)
#define Board2_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board2_IN_No_limitation        ((uint8_T)1U)
#define Board2_IN_No_movements         ((uint8_T)1U)
#define Board2_IN_Normal               ((uint8_T)2U)
#define Board2_IN_Normal_d             ((uint8_T)3U)
#define Board2_IN_Normal_driving       ((uint8_T)3U)
#define Board2_IN_Normal_lights        ((uint8_T)3U)
#define Board2_IN_Normal_relay         ((uint8_T)3U)
#define Board2_IN_Normal_routine       ((uint8_T)3U)
#define Board2_IN_Normal_working       ((uint8_T)3U)
#define Board2_IN_Not_moving           ((uint8_T)1U)
#define Board2_IN_Not_moving_routine   ((uint8_T)7U)
#define Board2_IN_Obstacle_left        ((uint8_T)2U)
#define Board2_IN_Obstacle_right       ((uint8_T)2U)
#define Board2_IN_Other_board_failure  ((uint8_T)1U)
#define Board2_IN_Ping_transmitted     ((uint8_T)2U)
#define Board2_IN_Receive_decision     ((uint8_T)5U)
#define Board2_IN_Receive_global_state ((uint8_T)6U)
#define Board2_IN_Receive_ping         ((uint8_T)3U)
#define Board2_IN_Receive_state        ((uint8_T)7U)
#define Board2_IN_Restablish           ((uint8_T)1U)
#define Board2_IN_Restarting           ((uint8_T)2U)
#define Board2_IN_Same_decision        ((uint8_T)8U)
#define Board2_IN_Select_routine       ((uint8_T)8U)
#define Board2_IN_Single_Board         ((uint8_T)3U)
#define Board2_IN_Special_retro_change ((uint8_T)2U)
#define Board2_IN_Special_retro_routine ((uint8_T)9U)
#define Board2_IN_Special_retro_start  ((uint8_T)3U)
#define Board2_IN_State_received       ((uint8_T)9U)
#define Board2_IN_Stop_back_rotation   ((uint8_T)1U)
#define Board2_IN_Stop_left_rotation   ((uint8_T)2U)
#define Board2_IN_Stop_right_rotation  ((uint8_T)3U)
#define Board2_IN_Stop_slow            ((uint8_T)1U)
#define Board2_IN_Stop_slow_routine    ((uint8_T)10U)
#define Board2_IN_Supervision_task     ((uint8_T)1U)
#define Board2_IN_Transmit_Decision    ((uint8_T)10U)
#define Board2_IN_Transmit_Global_State ((uint8_T)11U)
#define Board2_IN_Transmit_Local_State ((uint8_T)12U)
#define Board2_IN_Transmit_ping        ((uint8_T)5U)
#define Board2_IN_Turn_back_gyro       ((uint8_T)2U)
#define Board2_IN_Turn_back_no_gyro    ((uint8_T)3U)
#define Board2_IN_Turn_left            ((uint8_T)4U)
#define Board2_IN_Turn_left_no_gyro    ((uint8_T)5U)
#define Board2_IN_Turn_moving_left_gyro ((uint8_T)1U)
#define Board2_IN_Turn_right_gyro      ((uint8_T)6U)
#define Board2_IN_Turn_right_no_gyro   ((uint8_T)7U)
#define Board2_IN_Waiting              ((uint8_T)3U)
#define Board2_IN_Waiting_comunication ((uint8_T)13U)
#define Board_IN_Critical_voltage_relay ((uint8_T)1U)
#define Board_IN_Starting_to_restablish ((uint8_T)4U)
#define Board_IN_Turn_moving_right_gyro ((uint8_T)1U)
#define Board_IN_Waiting_change_max_vel ((uint8_T)1U)
#define IN_Global_Local_state_transmitt ((uint8_T)2U)
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
#include "board2_functions.h"

/* Block states (default storage) for system '<Root>' */
typedef struct {
  PacketGstate receivedGlobalStatePacket;/* '<Root>/Board2' */
  GSBus global_state;                  /* '<Root>/Board2' */
  PacketDecision receivedDecisionPacket;/* '<Root>/Board2' */
  DecBus decision;                     /* '<Root>/Board2' */
  PacketStateB1 receivedStatePacket;   /* '<Root>/Board2' */
  StateBusB2 state;                    /* '<Root>/Board2' */
  real32_T angle;                      /* '<Root>/Board2' */
  real32_T prevYaw;                    /* '<Root>/Board2' */
  real32_T TURN_THRESHOLD;             /* '<Root>/Board2' */
  int32_T sfEvent;                     /* '<Root>/Board2' */
  uint32_T time_temp;                  /* '<Root>/Board2' */
  uint32_T time_obs_s1;                /* '<Root>/Board2' */
  uint32_T time_button_retro;          /* '<Root>/Board2' */
  uint32_T time_comm;                  /* '<Root>/Board2' */
  uint32_T time_obs_s3;                /* '<Root>/Board2' */
  uint32_T time_button_obs;            /* '<Root>/Board2' */
  uint32_T time_button_vel;            /* '<Root>/Board2' */
  uint32_T turn_counter;               /* '<Root>/Board2' */
  WORKING_STATUS_TYPE working_status;  /* '<Root>/Board2' */
  uint16_T distance_threshold;         /* '<Root>/Board2' */
  int8_T change_velocity;              /* '<Root>/Board2' */
  uint8_T retransmitted;               /* '<Root>/Board2' */
  uint8_T receivedPing;                /* '<Root>/Board2' */
  uint8_T max_velocity;                /* '<Root>/Board2' */
  uint8_T is_active_c1_Board2;         /* '<Root>/Board2' */
  uint8_T is_c1_Board2;                /* '<Root>/Board2' */
  uint8_T is_active_Board_state;       /* '<Root>/Board2' */
  uint8_T is_Board_state;              /* '<Root>/Board2' */
  uint8_T is_Degraded;                 /* '<Root>/Board2' */
  uint8_T is_Restablish;               /* '<Root>/Board2' */
  uint8_T is_active_Supervisor;        /* '<Root>/Board2' */
  uint8_T is_Supervisor;               /* '<Root>/Board2' */
  uint8_T is_active_Global_state_compute;/* '<Root>/Board2' */
  uint8_T is_active_Moving_obstacle;   /* '<Root>/Board2' */
  uint8_T is_active_No_obstacle;       /* '<Root>/Board2' */
  uint8_T is_No_obstacle;              /* '<Root>/Board2' */
  uint8_T is_active_Obstacle_from_left;/* '<Root>/Board2' */
  uint8_T is_Obstacle_from_left;       /* '<Root>/Board2' */
  uint8_T is_active_Obstacle_from_right;/* '<Root>/Board2' */
  uint8_T is_Obstacle_from_right;      /* '<Root>/Board2' */
  uint8_T is_active_Battery_temperature_m;/* '<Root>/Board2' */
  uint8_T is_active_Normal_velocity;   /* '<Root>/Board2' */
  uint8_T is_Normal_velocity;          /* '<Root>/Board2' */
  uint8_T is_active_Battery_manager;   /* '<Root>/Board2' */
  uint8_T is_Battery_manager;          /* '<Root>/Board2' */
  uint8_T is_active_Temperature_manager;/* '<Root>/Board2' */
  uint8_T is_Temperature_manager;      /* '<Root>/Board2' */
  uint8_T is_active_Combo;             /* '<Root>/Board2' */
  uint8_T is_active_Special_retro;     /* '<Root>/Board2' */
  uint8_T is_Special_retro;            /* '<Root>/Board2' */
  uint8_T is_active_Obstacle_detection;/* '<Root>/Board2' */
  uint8_T is_Obstacle_detection;       /* '<Root>/Board2' */
  uint8_T is_active_Change_max_velocity;/* '<Root>/Board2' */
  uint8_T is_Change_max_velocity;      /* '<Root>/Board2' */
  uint8_T is_Single_Board;             /* '<Root>/Board2' */
  uint8_T is_active_Board_decision;    /* '<Root>/Board2' */
  uint8_T is_active_Working_status_manage;/* '<Root>/Board2' */
  uint8_T is_Working_status_manager;   /* '<Root>/Board2' */
  uint8_T is_active_Routine_manager;   /* '<Root>/Board2' */
  uint8_T is_active_Max_velocity_handler;/* '<Root>/Board2' */
  uint8_T is_Max_velocity_handler;     /* '<Root>/Board2' */
  uint8_T is_active_Compute_routine;   /* '<Root>/Board2' */
  uint8_T is_Compute_routine;          /* '<Root>/Board2' */
  uint8_T is_Normal_routine;           /* '<Root>/Board2' */
  uint8_T is_Control_controller_routine;/* '<Root>/Board2' */
  uint8_T is_Emergency_button_routine; /* '<Root>/Board2' */
  uint8_T is_Emergency_sonar_routine;  /* '<Root>/Board2' */
  uint8_T is_Low_controller_battery_routi;/* '<Root>/Board2' */
  uint8_T is_Moving_obstacle_from_left_ro;/* '<Root>/Board2' */
  uint8_T is_Moving_obstacle_from_right_r;/* '<Root>/Board2' */
  uint8_T is_Not_moving_routine;       /* '<Root>/Board2' */
  uint8_T is_Special_retro_routine;    /* '<Root>/Board2' */
  uint8_T is_Stop_slow_routine;        /* '<Root>/Board2' */
  uint8_T is_active_Mode_manager;      /* '<Root>/Board2' */
  uint8_T is_Mode_manager;             /* '<Root>/Board2' */
  uint8_T is_Normal_driving;           /* '<Root>/Board2' */
  uint8_T is_active_Lights_manager;    /* '<Root>/Board2' */
  uint8_T is_Lights_manager;           /* '<Root>/Board2' */
  uint8_T is_Normal_lights;            /* '<Root>/Board2' */
  uint8_T is_active_Relay_manager;     /* '<Root>/Board2' */
  uint8_T is_Relay_manager;            /* '<Root>/Board2' */
  boolean_T special_retro;             /* '<Root>/Board2' */
  boolean_T limit_velocity;            /* '<Root>/Board2' */
  boolean_T obs_detection;             /* '<Root>/Board2' */
  boolean_T special_retro_rotating;    /* '<Root>/Board2' */
  boolean_T moving_from_left;          /* '<Root>/Board2' */
  boolean_T moving_from_right;         /* '<Root>/Board2' */
  boolean_T prev_button1_retro;        /* '<Root>/Board2' */
  boolean_T prev_button2_retro;        /* '<Root>/Board2' */
  boolean_T prev_button1_obs;          /* '<Root>/Board2' */
  boolean_T prev_button2_obs;          /* '<Root>/Board2' */
  boolean_T prev_button1_vel;          /* '<Root>/Board2' */
  boolean_T prev_limit_state;          /* '<Root>/Board2' */
  boolean_T prev_r_stick_button;       /* '<Root>/Board2' */
  boolean_T prev_l_stick_button;       /* '<Root>/Board2' */
} DW_Board2_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  uint16_T sonar1;                     /* '<Root>/sonar1' */
  uint16_T sonar2;                     /* '<Root>/sonar2' */
  uint16_T sonar3;                     /* '<Root>/sonar3' */
  uint16_T controller_x;               /* '<Root>/controller_x' */
  uint16_T controller_y;               /* '<Root>/controller_y' */
  boolean_T B1;                        /* '<Root>/B1' */
  boolean_T B2;                        /* '<Root>/B2' */
  real32_T gyroYaw;                    /* '<Root>/gyroYaw' */
  boolean_T B3;                        /* '<Root>/B3' */
  boolean_T B4;                        /* '<Root>/B4' */
  boolean_T B_r_stick;                 /* '<Root>/B_r_stick' */
  uint8_T controller_battery;          /* '<Root>/controller_battery' */
  boolean_T B_l_stick;                 /* '<Root>/B_l_stick' */
  boolean_T controllerError;           /* '<Root>/controllerError' */
  boolean_T gyroError;                 /* '<Root>/gyroError' */
} ExtU_Board2_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  DecBus output;                       /* '<Root>/output' */
} ExtY_Board2_T;

/* Real-time Model Data Structure */
struct tag_RTM_Board2_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_Board2_T Board2_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Board2_T Board2_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Board2_T Board2_Y;

/* Model entry point functions */
extern void Board2_initialize(void);
extern void Board2_step(void);
extern void Board2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Board2_T *const Board2_M;

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
 * hilite_system('Board2_supervision/Board2')    - opens subsystem Board2_supervision/Board2
 * hilite_system('Board2_supervision/Board2/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Board2_supervision'
 * '<S1>'   : 'Board2_supervision/Board2'
 */
#endif                                 /* Board2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
