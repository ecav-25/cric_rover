/*
 * motor_diagnostic.h
 */

#ifndef INC_MOTOR_DIAG_H_
#define INC_MOTOR_DIAG_H_

#include <stdint.h>
#include <stdbool.h>

/* --- CONFIGURAZIONE DIMENSIONE BUFFER --- */
#ifndef M_DIAG_BUF_LEN
#define M_DIAG_BUF_LEN      200
#endif

/* -------------------- Return status -------------------- */
typedef enum {
    M_DIAG_OK = 0,
    M_DIAG_ERR = -1
} MotorDiag_Status_t;

/* -------------------- Stati Motore -------------------- */
typedef enum {
    MOTOR_HEALTHY = 1,
    MOTOR_FAILURE = 0
} Motor_Health_t;

/* -------------------- Configurazione Parametri -------------------- */

typedef struct {
    uint16_t delay_shift;
    float    max_area_limit;
} MotorDiag_Config_t;

/* -------------------- Handle Diagnostica -------------------- */
typedef struct {
    /* --- MEMORIA DATI  --- */
    float buf_ref_A[M_DIAG_BUF_LEN];
    float buf_ref_B[M_DIAG_BUF_LEN];
    float buf_meas_A[M_DIAG_BUF_LEN];
    float buf_meas_B[M_DIAG_BUF_LEN];

    /* --- CONFIGURAZIONE --- */
    MotorDiag_Config_t config;

    /* --- STATO RUNTIME --- */
    uint8_t  active_buf_idx;
    uint16_t write_idx;
    bool     is_buffer_ready;

    /* --- OUTPUT --- */
    float          last_area_error;
    Motor_Health_t health_status;

} MotorDiag_Handle_t;

/* -------------------- API -------------------- */

/**
 * @brief Inizializza il modulo e azzera i buffer interni.
 */
MotorDiag_Status_t motor_diag_init(MotorDiag_Handle_t* hdiag, const MotorDiag_Config_t* config);

/**
 * @brief Registra un campione nei buffer interni.
 */
MotorDiag_Status_t motor_diag_record(MotorDiag_Handle_t* hdiag, float ref_val, float meas_val);

/**
 * @brief Elabora i dati interni.
 */
MotorDiag_Status_t motor_diag_process(MotorDiag_Handle_t* hdiag);

#endif /* INC_MOTOR_DIAG_H_ */
