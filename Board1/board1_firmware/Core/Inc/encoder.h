#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "tim.h"
#include <stdint.h>

typedef enum {
    ENCODER_OK = 0,
    ENCODER_ERR = -1
} Encoder_Status_t;

typedef struct {
    uint16_t pulses_per_rev;
    float    gear_ratio;
} Encoder_Calibration_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t           prev_count;
    uint32_t           arr;
    float              reading_period;
    Encoder_Calibration_t calib;
    float              velocity;
} EncoderHandle;

Encoder_Status_t encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, const Encoder_Calibration_t* calib, float reading_period_ms);

Encoder_Status_t encoder_readRPM(EncoderHandle* enc);

#endif
