/*
 * motor_diagnostic.c
 */

#include "motor_diagnostic.h"
#include <math.h>
#include <string.h>
#include <stddef.h>

/* -------------------- Helpers -------------------- */

static float calculate_iae_shifted(const float* ref, const float* meas, uint16_t len, uint16_t shift) {
    float area = 0.0f;
    float diff = 0.0f;
    uint16_t check_len;

    if (shift >= len) return 0.0f;

    check_len = len - shift;

    for (uint16_t i = 0; i < check_len; i++) {
        diff = ref[i] - meas[i + shift];
        if (diff < 0.0f) diff = -diff;
        area += diff;
    }
    return area;
}

/* -------------------- API -------------------- */

MotorDiag_Status_t motor_diag_init(MotorDiag_Handle_t* hdiag, const MotorDiag_Config_t* config) {
    if ((hdiag == NULL) || (config == NULL)) {
        return M_DIAG_ERR;
    }


    hdiag->config = *config;

    memset(hdiag->buf_ref_A, 0, sizeof(hdiag->buf_ref_A));
    memset(hdiag->buf_ref_B, 0, sizeof(hdiag->buf_ref_B));
    memset(hdiag->buf_meas_A, 0, sizeof(hdiag->buf_meas_A));
    memset(hdiag->buf_meas_B, 0, sizeof(hdiag->buf_meas_B));

    hdiag->active_buf_idx = 0;
    hdiag->write_idx = 0;
    hdiag->is_buffer_ready = false;
    hdiag->last_area_error = 0.0f;
    hdiag->health_status = MOTOR_HEALTHY;

    return M_DIAG_OK;
}

MotorDiag_Status_t motor_diag_record(MotorDiag_Handle_t* hdiag, float ref_val, float meas_val) {
    uint16_t pos = hdiag->write_idx;

    if (hdiag->active_buf_idx == 0) {
        hdiag->buf_ref_A[pos] = ref_val;
        hdiag->buf_meas_A[pos] = meas_val;
    } else {
        hdiag->buf_ref_B[pos] = ref_val;
        hdiag->buf_meas_B[pos] = meas_val;
    }

    hdiag->write_idx++;

    if (hdiag->write_idx >= M_DIAG_BUF_LEN) {
        hdiag->write_idx = 0;
        hdiag->is_buffer_ready = true;
        hdiag->active_buf_idx = !hdiag->active_buf_idx;
    }

    return M_DIAG_OK;
}

MotorDiag_Status_t motor_diag_process(MotorDiag_Handle_t* hdiag) {
    if ((hdiag == NULL) || (!hdiag->is_buffer_ready)) {
        return M_DIAG_OK;
    }

    uint8_t analyze_idx = !hdiag->active_buf_idx;
    float* p_ref;
    float* p_meas;

    if (analyze_idx == 0) {
        p_ref = hdiag->buf_ref_A;
        p_meas = hdiag->buf_meas_A;
    } else {
        p_ref = hdiag->buf_ref_B;
        p_meas = hdiag->buf_meas_B;
    }

    hdiag->last_area_error = calculate_iae_shifted(
        p_ref,
        p_meas,
        M_DIAG_BUF_LEN,
        hdiag->config.delay_shift
    );

    if (hdiag->last_area_error > hdiag->config.max_area_limit) {
        hdiag->health_status = MOTOR_FAILURE;
    } else {
        hdiag->health_status = MOTOR_HEALTHY;
    }

    hdiag->is_buffer_ready = false;

    return M_DIAG_OK;
}
