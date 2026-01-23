/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board1.h
 *
 * Code generated for Simulink model 'Board1'.
 *
 * Model version                  : 1.1786
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Jan 22 17:38:41 2026
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
#include "board_functions.h"
#endif                                 /* Board1_COMMON_INCLUDES_ */

#include "Board1_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */
#include "board_functions.h"

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
  uint32_T time_button;                /* '<Root>/Board1' */
  uint32_T time_obs;                   /* '<Root>/Board1' */
  uint32_T time_temp;                  /* '<Root>/Board1' */
  uint32_T time_comm;                  /* '<Root>/Board1' */
  MOVING_OBSTACLE_TYPE moving_obstacle;/* '<Root>/Board1' */
  uint8_T retransmitted;               /* '<Root>/Board1' */
  uint8_T receivedPing;                /* '<Root>/Board1' */
  uint8_T is_active_c3_Board1;         /* '<Root>/Board1' */
  uint8_T is_c3_Board1;                /* '<Root>/Board1' */
  uint8_T is_Supervision_task;         /* '<Root>/Board1' */
  uint8_T is_Degraded;                 /* '<Root>/Board1' */
  uint8_T is_Restablish;               /* '<Root>/Board1' */
  uint8_T is_active_Supervisor;        /* '<Root>/Board1' */
  uint8_T is_Supervisor;               /* '<Root>/Board1' */
  uint8_T is_active_Moving_obstacle;   /* '<Root>/Board1' */
  uint8_T is_active_No_obstacle;       /* '<Root>/Board1' */
  uint8_T is_No_obstacle;              /* '<Root>/Board1' */
  uint8_T is_active_Obstacle_from_left;/* '<Root>/Board1' */
  uint8_T is_Obstacle_from_left;       /* '<Root>/Board1' */
  uint8_T is_active_Obstacle_from_right;/* '<Root>/Board1' */
  uint8_T is_Obstacle_from_right;      /* '<Root>/Board1' */
  uint8_T is_active_Combo;             /* '<Root>/Board1' */
  uint8_T is_active_Special_retro;     /* '<Root>/Board1' */
  uint8_T is_Special_retro;            /* '<Root>/Board1' */
  uint8_T is_active_Obstacle_detection;/* '<Root>/Board1' */
  uint8_T is_Obstacle_detection;       /* '<Root>/Board1' */
  uint8_T is_active_Battery_temperature_m;/* '<Root>/Board1' */
  uint8_T is_active_Normal_velocity;   /* '<Root>/Board1' */
  uint8_T is_Normal_velocity;          /* '<Root>/Board1' */
  uint8_T is_active_Battery_manager;   /* '<Root>/Board1' */
  uint8_T is_Battery_manager;          /* '<Root>/Board1' */
  uint8_T is_active_Temperature_manager;/* '<Root>/Board1' */
  uint8_T is_Temperature_manager;      /* '<Root>/Board1' */
  uint8_T is_active_Actions;           /* '<Root>/Board1' */
  uint8_T is_active_Routine_manager;   /* '<Root>/Board1' */
  uint8_T is_Routine_manager;          /* '<Root>/Board1' */
  uint8_T is_Normal_voltage;           /* '<Root>/Board1' */
  uint8_T is_Control_controller_routine;/* '<Root>/Board1' */
  uint8_T is_Emergency_button_routine; /* '<Root>/Board1' */
  uint8_T is_Emergency_sonar_routine;  /* '<Root>/Board1' */
  uint8_T is_Low_controller_battery_routi;/* '<Root>/Board1' */
  uint8_T is_Moving_obstacle_from_left_ro;/* '<Root>/Board1' */
  uint8_T is_Moving_obstacle_from_right_r;/* '<Root>/Board1' */
  uint8_T is_Not_moving_routine;       /* '<Root>/Board1' */
  uint8_T is_Special_retro_routine;    /* '<Root>/Board1' */
  uint8_T is_Stop_slow_routine;        /* '<Root>/Board1' */
  uint8_T is_active_Lights_manager;    /* '<Root>/Board1' */
  uint8_T is_Lights_manager;           /* '<Root>/Board1' */
  uint8_T is_Normal_voltage_g;         /* '<Root>/Board1' */
  uint8_T is_active_Mode_manager;      /* '<Root>/Board1' */
  uint8_T is_Mode_manager;             /* '<Root>/Board1' */
  uint8_T is_Normal_voltage_n;         /* '<Root>/Board1' */
  uint8_T is_active_Relay_manager;     /* '<Root>/Board1' */
  uint8_T is_Relay_manager;            /* '<Root>/Board1' */
  uint8_T is_Single_Board;             /* '<Root>/Board1' */
  boolean_T special_retro;             /* '<Root>/Board1' */
  boolean_T limit_velocity;            /* '<Root>/Board1' */
  boolean_T obs_detection;             /* '<Root>/Board1' */
  boolean_T prev_button2_retro;        /* '<Root>/Board1' */
  boolean_T prev_button1_retro;        /* '<Root>/Board1' */
  boolean_T prev_button1_obs;          /* '<Root>/Board1' */
  boolean_T prev_button2_obs;          /* '<Root>/Board1' */
  boolean_T prev_r_stick_button;       /* '<Root>/Board1' */
  boolean_T prev_l_stick_button;       /* '<Root>/Board1' */
} DW_Board1_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T temperature;                /* '<Root>/temperature' */
  real32_T battery_voltage;            /* '<Root>/battery_voltage' */
  int16_T velocity_BA;                 /* '<Root>/velocity_BA' */
  int16_T velocity_BB;                 /* '<Root>/velocity_BB' */
  int16_T velocity_FA;                 /* '<Root>/velocity_FA' */
  int16_T velocity_FB;                 /* '<Root>/velocity_FB' */
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
 * hilite_system('diagram_acitions_sequencial/Board1')    - opens subsystem diagram_acitions_sequencial/Board1
 * hilite_system('diagram_acitions_sequencial/Board1/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'diagram_acitions_sequencial'
 * '<S1>'   : 'diagram_acitions_sequencial/Board1'
 */

/* Named constants for Chart: '<Root>/Board1' */
#define B_IN_Control_controller_routine ((uint8_T)1U)
#define B_IN_Moving_obstacle_from_right ((uint8_T)1U)
#define Bo_GLOBAL_STATE_RECEIVE_TIMEOUT (5500U)
#define Bo_IN_Moving_obstacle_from_left ((uint8_T)1U)
#define Boa_IN_Emergency_button_routine ((uint8_T)2U)
#define Boar_IN_Connection_restablished ((uint8_T)1U)
#define Boar_IN_Control_from_controller ((uint8_T)1U)
#define Boar_IN_Emergency_sonar_routine ((uint8_T)3U)
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
#define Board1_IN_Control_battery_stop ((uint8_T)1U)
#define Board1_IN_Critical_voltage     ((uint8_T)1U)
#define Board1_IN_Decision_received    ((uint8_T)1U)
#define Board1_IN_Decision_sended      ((uint8_T)2U)
#define Board1_IN_Degraded             ((uint8_T)1U)
#define Board1_IN_Emergency_button     ((uint8_T)1U)
#define Board1_IN_Emergency_sonar      ((uint8_T)1U)
#define Board1_IN_First_button         ((uint8_T)1U)
#define Board1_IN_Global_state_received ((uint8_T)3U)
#define Board1_IN_Global_state_sended  ((uint8_T)4U)
#define Board1_IN_High_temp            ((uint8_T)1U)
#define Board1_IN_Lights_AUTO          ((uint8_T)1U)
#define Board1_IN_Lights_OFF           ((uint8_T)2U)
#define Board1_IN_Lights_ON            ((uint8_T)3U)
#define Board1_IN_Limited              ((uint8_T)1U)
#define Board1_IN_Limited_o            ((uint8_T)2U)
#define Board1_IN_Mode_DEFAULT         ((uint8_T)1U)
#define Board1_IN_Mode_ECO             ((uint8_T)2U)
#define Board1_IN_Mode_SPORT           ((uint8_T)3U)
#define Board1_IN_NO_ACTIVE_CHILD      ((uint8_T)0U)
#define Board1_IN_No_limitation        ((uint8_T)1U)
#define Board1_IN_No_movements         ((uint8_T)1U)
#define Board1_IN_Normal               ((uint8_T)2U)
#define Board1_IN_Normal_g             ((uint8_T)3U)
#define Board1_IN_Normal_voltage       ((uint8_T)2U)
#define Board1_IN_Not_moving           ((uint8_T)1U)
#define Board1_IN_Not_moving_routine   ((uint8_T)7U)
#define Board1_IN_Obstacle_left        ((uint8_T)2U)
#define Board1_IN_Obstacle_right       ((uint8_T)2U)
#define Board1_IN_Other_board_failure  ((uint8_T)1U)
#define Board1_IN_Ping_sended          ((uint8_T)2U)
#define Board1_IN_Receive_decision     ((uint8_T)5U)
#define Board1_IN_Receive_global_state ((uint8_T)6U)
#define Board1_IN_Receive_ping         ((uint8_T)3U)
#define Board1_IN_Receive_state        ((uint8_T)7U)
#define Board1_IN_Received_Ping        ((uint8_T)4U)
#define Board1_IN_Received_state       ((uint8_T)8U)
#define Board1_IN_Restablish           ((uint8_T)1U)
#define Board1_IN_Restarting           ((uint8_T)2U)
#define Board1_IN_Same_decision        ((uint8_T)9U)
#define Board1_IN_Select_routine       ((uint8_T)8U)
#define Board1_IN_Send_decision        ((uint8_T)10U)
#define Board1_IN_Send_global_state    ((uint8_T)11U)
#define Board1_IN_Send_ping            ((uint8_T)5U)
#define Board1_IN_Send_state           ((uint8_T)12U)
#define Board1_IN_Single_Board         ((uint8_T)3U)
#define Board1_IN_Special_retro_change ((uint8_T)2U)
#define Board1_IN_Special_retro_routine ((uint8_T)9U)
#define Board1_IN_Special_retro_start  ((uint8_T)3U)
#define Board1_IN_State_sended         ((uint8_T)13U)
#define Board1_IN_Stop_back_rotation   ((uint8_T)1U)
#define Board1_IN_Stop_left_rotation   ((uint8_T)2U)
#define Board1_IN_Stop_left_rotation_l ((uint8_T)1U)
#define Board1_IN_Stop_right_rotation  ((uint8_T)3U)
#define Board1_IN_Stop_right_rotation_g ((uint8_T)1U)
#define Board1_IN_Stop_slow            ((uint8_T)1U)
#define Board1_IN_Stop_slow_routine    ((uint8_T)10U)
#define Board1_IN_Stop_to_turn_left    ((uint8_T)2U)
#define Board1_IN_Stop_to_turn_right   ((uint8_T)2U)
#define Board1_IN_Supervision_task     ((uint8_T)1U)
#define Board1_IN_Turn_back            ((uint8_T)2U)
#define Board1_IN_Turn_left            ((uint8_T)4U)
#define Board1_IN_Turn_left_h          ((uint8_T)3U)
#define Board1_IN_Turn_right           ((uint8_T)5U)
#define Board1_IN_Turn_right_m         ((uint8_T)3U)
#define Board1_IN_Waiting              ((uint8_T)3U)
#define Board1_IN_Waiting_to_restablish ((uint8_T)6U)
#define Board1_IN_Waiting_to_start     ((uint8_T)14U)
#define Board1_LIMITED_RPM             (80.0F)
#define Board1_LOW_CONTROLLER_BATTERY  ((uint8_T)5U)
#define Board1_LOW_VOLTAGE             (9.83F)
#define Board1_MAX_DISTANCE            ((uint16_T)300U)
#define Board1_MAX_RETRANSMIT          ((uint8_T)1U)
#define Board1_MAX_RPM                 (150.0F)
#define Board1_MIN_DISTANCE            ((uint16_T)150U)
#define Board1_OBS_TIMEOUT             (1000U)
#define Board1_PERIOD                  (0.06F)
#define Board1_PURE_TURN_EPS           (0.01F)
#define Board1_STATE_RECEIVE_TIMEOUT   (3800U)
#define Board1_STATE_SEND_TIMEOUT      (2200U)
#define Board1_TEMP_TIMEOUT            (15000U)
#define Board1_TURN_ANGLE              (45.0F)
#define Board1_TURN_BACK_ANGLE         (180.0F)
#define Board1_TURN_BACK_RPM           (40.0F)
#define Board1_TURN_RATIO              (0.35F)
#define Board1_TURN_RPM                (20.0F)
#define Board1_TURN_THRESHOLD          (5.0F)
#define Board1_WAIT_TIMEOUT            (500U)
#define Board1_event_STEP              (1483)
#define Board_GLOBAL_STATE_SEND_TIMEOUT (5000U)
#define IN_Low_controller_battery_routi ((uint8_T)4U)
#define IN_Moving_obstacle_from_left_ro ((uint8_T)5U)
#define IN_Moving_obstacle_from_right_r ((uint8_T)6U)

#endif                                 /* Board1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
