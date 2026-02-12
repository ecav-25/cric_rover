/*
 * motor_diagnostic.c
 */

#include "motor_diagnostic.h"
#include <math.h>
#include <string.h>
#include <stddef.h>

/* -------------------- Helpers -------------------- */

static float calculate_iae_shifted(const float* ref, const float* meas, uint16_t len, uint16_t shift)
{
    float area = 0.0f;
    float diff = 0.0f;
    uint16_t check_len;

    if (shift < len)
    {
        check_len = (uint16_t)(len - shift);

        for (uint16_t i = 0U; i < check_len; i++)
        {
            diff = ref[i] - meas[i + shift];

            if (diff < 0.0f)
            {
                diff = -diff;
            }

            area += diff;
        }
    }

    return area;
}

/* -------------------- API -------------------- */

MotorDiag_Status_t motor_diag_init(MotorDiag_Handle_t* hdiag, const MotorDiag_Config_t* config)
{
    MotorDiag_Status_t status = M_DIAG_ERR;

    if ((hdiag != NULL) && (config != NULL))
    {
        hdiag->config = *config;

        (void)memset(hdiag->buf_ref_A, 0, sizeof(hdiag->buf_ref_A));
        (void)memset(hdiag->buf_ref_B, 0, sizeof(hdiag->buf_ref_B));
        (void)memset(hdiag->buf_meas_A, 0, sizeof(hdiag->buf_meas_A));
        (void)memset(hdiag->buf_meas_B, 0, sizeof(hdiag->buf_meas_B));

        hdiag->active_buf_idx = 0U;
        hdiag->write_idx = 0U;
        hdiag->is_buffer_ready = false;
        hdiag->last_area_error = 0.0f;
        hdiag->health_status = MOTOR_HEALTHY;

        status = M_DIAG_OK;
    }

    return status;
}

MotorDiag_Status_t motor_diag_record(MotorDiag_Handle_t* hdiag, float ref_val, float meas_val)
{
    MotorDiag_Status_t status = M_DIAG_ERR;

    if (hdiag != NULL)
    {
        uint16_t pos = hdiag->write_idx;

        if (hdiag->active_buf_idx == 0U)
        {
            hdiag->buf_ref_A[pos] = ref_val;
            hdiag->buf_meas_A[pos] = meas_val;
        }
        else
        {
            hdiag->buf_ref_B[pos] = ref_val;
            hdiag->buf_meas_B[pos] = meas_val;
        }

        hdiag->write_idx++;

        if (hdiag->write_idx >= (uint16_t)M_DIAG_BUF_LEN)
        {
            hdiag->write_idx = 0U;
            hdiag->is_buffer_ready = true;
            hdiag->active_buf_idx = (hdiag->active_buf_idx == 0U) ? 1U : 0U;
        }

        status = M_DIAG_OK;
    }
    return status;
}



MotorDiag_Status_t motor_diag_process(MotorDiag_Handle_t* hdiag)
{
    MotorDiag_Status_t status = M_DIAG_ERR;

    if (hdiag != NULL)
    {
        if (hdiag->is_buffer_ready)
        {
            uint8_t analyze_idx = (hdiag->active_buf_idx == 0U) ? 1U : 0U;
            const float* p_ref;
            const float* p_meas;

            if (analyze_idx == 0U)
            {
                p_ref = hdiag->buf_ref_A;
                p_meas = hdiag->buf_meas_A;
            }
            else
            {
                p_ref = hdiag->buf_ref_B;
                p_meas = hdiag->buf_meas_B;
            }

            hdiag->last_area_error = calculate_iae_shifted(
                p_ref,
                p_meas,
                (uint16_t)M_DIAG_BUF_LEN,
                hdiag->config.delay_shift
            );

            if (hdiag->last_area_error > hdiag->config.max_area_limit)
            {
                hdiag->health_status = MOTOR_FAILURE;
            }
            else
            {
                hdiag->health_status = MOTOR_HEALTHY;
            }

            hdiag->is_buffer_ready = false;
        }

        status = M_DIAG_OK;
    }

    return status;
}
