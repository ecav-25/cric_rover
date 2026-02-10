/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Board2_types.h
 *
 * Code generated for Simulink model 'Board2'.
 *
 * Model version                  : 1.2532
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Tue Feb 10 12:44:45 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef Board2_types_h_
#define Board2_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_BRAKING_TYPE_
#define DEFINED_TYPEDEF_FOR_BRAKING_TYPE_

typedef enum {
  NONE = 0,                            /* Default value */
  NORMAL,
  EMERGENCY
} BRAKING_TYPE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_LED_TYPE_
#define DEFINED_TYPEDEF_FOR_LED_TYPE_

typedef enum {
  OFF = 0,                             /* Default value */
  WHITE,
  RED,
  BLINKING_WHITE,
  BLINKING_RED,
  BLINKING_RED_SLOW
} LED_TYPE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_REAR_LED_TYPE_
#define DEFINED_TYPEDEF_FOR_REAR_LED_TYPE_

typedef enum {
  IDLE = 0,                            /* Default value */
  BACKLIGHTS,
  BRAKING_LIGHTS,
  BACKWARD_LIGHTS,
  ARROW_LEFT,
  ARROW_RIGHT,
  SPECIAL_LIGHTS,
  EMERGENCY_LIGHTS,
  DEGRADED_LIGHTS
} REAR_LED_TYPE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_REAR_SIGN_TYPE_
#define DEFINED_TYPEDEF_FOR_REAR_SIGN_TYPE_

typedef enum {
  SIGN_OFF = 0,                        /* Default value */
  SIGN_WHITE,
  SIGN_GREEN,
  SIGN_ORANGE,
  SIGN_RED,
  SIGN_YELLOW
} REAR_SIGN_TYPE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ROVER_MODE_
#define DEFINED_TYPEDEF_FOR_ROVER_MODE_

typedef enum {
  DEFAULT = 0,                         /* Default value */
  SPORT,
  ECO
} ROVER_MODE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_DecBus_
#define DEFINED_TYPEDEF_FOR_DecBus_

typedef struct {
  real32_T rif_FA;
  real32_T rif_FB;
  real32_T rif_BA;
  real32_T rif_BB;
  BRAKING_TYPE brk_mode;
  LED_TYPE led_A;
  LED_TYPE led_B;
  REAR_LED_TYPE rear_led;
  REAR_SIGN_TYPE rear_sign;
  ROVER_MODE mode;
  boolean_T relay;
  boolean_T mux;
} DecBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_StateBusB1_
#define DEFINED_TYPEDEF_FOR_StateBusB1_

typedef struct {
  real32_T battery_voltage;
  real32_T temperature;
  int16_T velocity_FA;
  int16_T velocity_FB;
  int16_T velocity_BA;
  int16_T velocity_BB;
  boolean_T motorError_FA;
  boolean_T motorError_FB;
  boolean_T motorError_BA;
  boolean_T motorError_BB;
} StateBusB1;

#endif

#ifndef DEFINED_TYPEDEF_FOR_StateBusB2_
#define DEFINED_TYPEDEF_FOR_StateBusB2_

typedef struct {
  real32_T gyroYaw;
  uint16_T sonar1;
  uint16_T sonar2;
  uint16_T sonar3;
  uint16_T controller_y;
  uint16_T controller_x;
  boolean_T button1;
  boolean_T button2;
  boolean_T button3;
  boolean_T button4;
  boolean_T r_stick_button;
  boolean_T l_stick_button;
  uint8_T controller_battery;
  boolean_T controllerError;
  boolean_T gyroError;
} StateBusB2;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MOVING_OBSTACLE_TYPE_
#define DEFINED_TYPEDEF_FOR_MOVING_OBSTACLE_TYPE_

typedef enum {
  NO_OBSTACLE = 0,                     /* Default value */
  MOVING_FROM_LEFT,
  MOVING_FROM_RIGHT,
  MOVING_FROM_BOTH
} MOVING_OBSTACLE_TYPE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_GSBus_
#define DEFINED_TYPEDEF_FOR_GSBus_

typedef struct {
  StateBusB1 stateB1;
  StateBusB2 stateB2;
  MOVING_OBSTACLE_TYPE mov_obs;
  boolean_T spc_retro;
  boolean_T limit_vel;
  int8_T change_vel;
  boolean_T obs_detection;
} GSBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PacketDecision_
#define DEFINED_TYPEDEF_FOR_PacketDecision_

typedef struct {
  DecBus decision;
  uint32_T crc;
} PacketDecision;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PacketGstate_
#define DEFINED_TYPEDEF_FOR_PacketGstate_

typedef struct {
  GSBus global_state;
  uint32_T crc;
} PacketGstate;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PacketStateB1_
#define DEFINED_TYPEDEF_FOR_PacketStateB1_

typedef struct {
  StateBusB1 state;
  uint32_T crc;
} PacketStateB1;

#endif

#ifndef DEFINED_TYPEDEF_FOR_WORKING_STATUS_TYPE_
#define DEFINED_TYPEDEF_FOR_WORKING_STATUS_TYPE_

typedef enum {
  NORMAL_WORKING = 0,                  /* Default value */
  CRITICAL_VOLTAGE_WORKING,
  MOTOR_ERROR_WORKING
} WORKING_STATUS_TYPE;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Board2_T RT_MODEL_Board2_T;

#endif                                 /* Board2_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
