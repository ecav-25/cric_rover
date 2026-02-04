// ============================================================================
// types.h - STRUTTURE DATI 
// ============================================================================

#ifndef TYPES_H
#define TYPES_H

#include "config.h"

// ============================================================================
// CONTROLLER DATA
// ============================================================================

typedef struct __attribute__((packed)) {
    uint16_t ax, ay;
    uint8_t a_btn;
    uint16_t bx, by;
    uint8_t b_btn;
    uint8_t btn1, btn2, btn3, btn4;
} ControllerData;

typedef struct __attribute__((packed)) {
    uint8_t alive;
    uint8_t battery;
    ControllerData data;
} ControllerStatus;

// ============================================================================
// ROVER TELEMETRY
// ============================================================================

typedef uint8_t BackwardMode;
enum {
    BW_NORMAL = 0,
    BW_SPECIAL
};

typedef uint8_t OperatingMode;
enum {
    OM_NORMAL = 0,
    OM_SINGLE_BOARD,
    OM_DEGRADED
};

typedef uint8_t LightOperatingMode;
enum {
    LIGHT_OFF = 0,
    LIGHT_ON,
    LIGHT_AUTO
};

typedef uint8_t DrivingMode;
enum {
    DM_DEFAULT = 0,
    DM_SPORT,
    DM_ECO
};

typedef uint8_t ObstacleAvoidanceMode;
enum {
    COMPLETE = 0,
    MINIMAL
};


typedef struct __attribute__((packed)) {
	uint16_t sonar_l;
	uint16_t sonar_c;
	uint16_t sonar_r;

    int16_t rpm_fl;
    int16_t rpm_fr;
    int16_t rpm_rl;
    int16_t rpm_rr;

    int16_t target_fl;
    int16_t target_fr;
    int16_t target_rl;
    int16_t target_rr;

    uint8_t max_velocity;

    uint8_t battery_percent;
    int8_t  temperature;

    BackwardMode backward_mode;
    OperatingMode operating_mode;
    LightOperatingMode light_mode;
    DrivingMode driving_mode;
    ObstacleAvoidanceMode obstacle_mode;
} RoverTelemetry;


// ============================================================================
// AI TELEMETRY
// ============================================================================

typedef struct __attribute__((packed)) {
    float mse;           
    uint8_t alarm;       
} AITelemetry;

// ============================================================================
// FULL TELEMETRY PACKET 
// ============================================================================

typedef struct __attribute__((packed)) {
    RoverTelemetry rover;
    AITelemetry ai;
} FullTelemetry;

// ============================================================================
// ESP-NOW PACKET
// ============================================================================

typedef struct __attribute__((packed)) {
    ControllerData controller;
    uint8_t battery;
} ESPNowPacket;

#endif // TYPES_H