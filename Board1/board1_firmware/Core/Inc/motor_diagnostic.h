/**
 * @file motor_diag.h
 * @author Gruppo 2
 * @brief Modulo di diagnostica per il monitoraggio della salute dei motori.
 *
 * Il modulo confronta il setpoint (riferimento) con la velocità misurata (encoder)
 * utilizzando un buffer circolare. Calcola l'errore cumulativo (area) per rilevare
 * discrepanze che indicano malfunzionamenti o guasti hardware.
 */

#ifndef INC_MOTOR_DIAG_H_
#define INC_MOTOR_DIAG_H_

#include <stdint.h>
#include <stdbool.h>

/** @brief Dimensione dei buffer circolari per l'analisi dei campioni. */
#ifndef M_DIAG_BUF_LEN
#define M_DIAG_BUF_LEN 48
#endif

/**
 * @brief Codici di ritorno per le funzioni di diagnostica.
 */
typedef enum {
    M_DIAG_OK = 0,    /**< Operazione eseguita con successo */
    M_DIAG_ERR = -1   /**< Errore di configurazione o puntatore nullo */
} MotorDiag_Status_t;

/**
 * @brief Rappresentazione dello stato di salute del motore.
 */
typedef enum {
    MOTOR_HEALTHY = 1, /**< Motore operativo e performante */
    MOTOR_FAILURE = 0  /**< Rilevata anomalia critica o guasto */
} Motor_Health_t;

/**
 * @brief Parametri di calibrazione per l'algoritmo di diagnostica.
 */
typedef struct {
    uint16_t delay_shift;    /**< Spostamento temporale per compensare il ritardo di risposta */
    float    max_area_limit; /**< Soglia massima di errore cumulativo prima del FAILURE */
} MotorDiag_Config_t;

/**
 * @brief Struttura di gestione per un'istanza di diagnostica motore.
 */
typedef struct {
    float buf_ref_A[M_DIAG_BUF_LEN];  /**< Buffer dei riferimenti (setpoint) */
    float buf_ref_B[M_DIAG_BUF_LEN];  /**< Buffer di supporto secondario */
    float buf_meas_A[M_DIAG_BUF_LEN]; /**< Buffer delle misure reali (encoder) */
    float buf_meas_B[M_DIAG_BUF_LEN]; /**< Buffer di supporto secondario */

    MotorDiag_Config_t config;        /**< Configurazione dei parametri di soglia */

    uint8_t  active_buf_idx;          /**< Indice del buffer in uso */
    uint16_t write_idx;               /**< Indice di scrittura corrente */
    bool     is_buffer_ready;         /**< Flag di buffer pronto */

    float          last_area_error;   /**< Ultimo valore dell'area di errore */
    Motor_Health_t health_status;     /**< Stato di salute attuale */
} MotorDiag_Handle_t;

/**
 * @brief Inizializza il modulo e azzera i buffer interni.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @param config Puntatore alla configurazione parametri.
 * @return MotorDiag_Status_t M_DIAG_OK se l'inizializzazione ha successo.
 */
MotorDiag_Status_t motor_diag_init(MotorDiag_Handle_t* hdiag, const MotorDiag_Config_t* config);

/**
 * @brief Registra un nuovo campione nei buffer interni.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @param ref_val Valore di riferimento (setpoint).
 * @param meas_val Valore misurato (encoder).
 * @return MotorDiag_Status_t M_DIAG_OK se registrato.
 */
MotorDiag_Status_t motor_diag_record(MotorDiag_Handle_t* hdiag, float ref_val, float meas_val);

/**
 * @brief Elabora i dati raccolti per determinare lo stato di salute del motore.
 * @param hdiag Puntatore all'handle della diagnostica.
 * @return MotorDiag_Status_t M_DIAG_OK se elaborazione completata.
 */
MotorDiag_Status_t motor_diag_process(MotorDiag_Handle_t* hdiag);

#endif /* INC_MOTOR_DIAG_H_ */
