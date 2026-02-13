// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file    controller.h
 * @author  Gruppo 2
 * @brief   Gestione della comunicazione con il controller remoto e telemetria.
 * @details Questo modulo definisce le strutture dati e le funzioni per interfacciare
 * il sistema con un controller esterno via I2C. Gestisce sia la ricezione dei comandi
 * di input (joystick, pulsanti) che l'invio dei dati di telemetria (stato batteria,
 * sensori, modalità operative).
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "i2c.h"
#include <stdint.h>

/** @brief Valore sentinella per indicare assenza di input o valore neutro. */
#define CONTROLLERZERO (255)

/** @brief Numero massimo di errori CRC consecutivi prima di invalidare la connessione. */
#define CONTROLLER_MAX_CRC_ERRORS   3u


/* ========================================================================== */
/* STATUS CODES                                                               */
/* ========================================================================== */

/**
 * @brief Codici di stato per le operazioni del controller.
 */
typedef enum {
    CONTROLLER_OK = 0,      /**< Operazione completata con successo. */
    CONTROLLER_ERR,         /**< Errore generico (es. parametri non validi). */
	CONTROLLER_ERR_COMM,    /**< Errore di comunicazione I2C (HAL error). */
    CONTROLLER_ERR_CRC      /**< Errore di validazione CRC sui dati ricevuti. */
} ControllerStatus_t;

/* ========================================================================== */
/* CONTROLLER DATA STRUCTURES                                                 */
/* ========================================================================== */

/**
 * @brief Struttura dati grezza ricevuta dal controller (Input utente).
 * @note  Attributo packed per garantire l'allineamento dei byte durante la trasmissione.
 */
typedef struct __attribute__((packed)) {
    uint16_t ax;     /**< Valore asse analogico X (Stick A). */
    uint16_t ay;     /**< Valore asse analogico Y (Stick A). */
    uint8_t  a_btn;  /**< Stato pulsante Stick A. */

    uint16_t bx;     /**< Valore asse analogico X (Stick B). */
    uint16_t by;     /**< Valore asse analogico Y (Stick B). */
    uint8_t  b_btn;  /**< Stato pulsante Stick B. */

    uint8_t btn1;    /**< Stato pulsante 1. */
    uint8_t btn2;    /**< Stato pulsante 2. */
    uint8_t btn3;    /**< Stato pulsante 3. */
    uint8_t btn4;    /**< Stato pulsante 4. */
} controller_data_t;

/**
 * @brief Pacchetto completo di informazioni dal controller.
 * @details Include lo stato di connessione (alive), la percentuale batteria del controller,
 * i dati di input e il checksum CRC.
 */
typedef struct __attribute__((packed)) {
    uint8_t alive;                    /**< Flag heartbeat/keep-alive. */
    uint8_t controller_percentage;    /**< Livello batteria del controller (0-100). */
    controller_data_t controller_data;/**< Dati di input (assi e pulsanti). */
    uint8_t crc_value;                /**< Checksum CRC per validazione pacchetto. */
} controller_information_t;

/**
 * @brief Handle principale per la gestione del controller remoto.
 */
typedef struct {
    I2C_HandleTypeDef* i2c;           /**< Handle I2C utilizzato per la comunicazione. */
    uint16_t address;                 /**< Indirizzo I2C slave del controller. */
    controller_information_t controller_information; /**< Ultimi dati ricevuti (buffer corrente). */
    controller_information_t last_valid_information; /**< Ultimi dati validi (fallback in caso di errore). */
	uint8_t crc_error_count;          /**< Contatore errori CRC consecutivi per monitoraggio qualità link. */
} Controller_t;

/* ========================================================================== */
/* TELEMETRY DATA STRUCTURES                                                  */
/* ========================================================================== */

/** @brief Modalità di retromarcia. */
typedef uint8_t BackwardMode;
enum {
    BW_NORMAL = 0,  /**< Retromarcia standard. */
    BW_SPECIAL      /**< Retromarcia speciale (es. limitata o assistita). */
};

/** @brief Modalità operativa del sistema. */
typedef uint8_t OperatingMode;
enum {
    OM_NORMAL = 0,      /**< Funzionamento normale (tutte le board attive). */
    OM_SINGLE_BOARD,    /**< Funzionamento a scheda singola (debug o emergenza). */
    OM_DEGRADED         /**< Modalità degradata (limitazione prestazioni per guasto). */
};

/** @brief Modalità di controllo luci. */
typedef uint8_t LightOperatingMode;
enum {
    LIGHT_OFF = 0,  /**< Luci spente. */
    LIGHT_ON,       /**< Luci accese. */
    LIGHT_AUTO      /**< Luci automatiche (sensore luce o contesto). */
};

/** @brief Profilo di guida (mappatura acceleratore/velocità). */
typedef uint8_t DrivingMode;
enum {
    DM_DEFAULT = 0, /**< Profilo standard. */
    DM_SPORT,       /**< Profilo sportivo (risposta rapida). */
    DM_ECO          /**< Profilo risparmio energetico (risposta dolce). */
};

/** @brief Modalità evitamento ostacoli (Sonars/Lidar). */
typedef uint8_t ObstacleAvoidanceMode;
enum {
    COMPLETE = 0,   /**< Evitamento completo attivo. */
    MINIMAL         /**< Assistenza minima (solo avvisi). */
};

/** @brief Stato del giroscopio. */
typedef uint8_t GyroAvailability;
enum {
	GYRO_STATUS_DEGRADED = 0, /**< Giroscopio non affidabile o non calibrato. */
    GYRO_STATUS_READY         /**< Giroscopio operativo e calibrato. */
};

/** @brief Stato di salute della trazione (motori/driver). */
typedef uint8_t Traction_Health;
enum {
	T_UNHEALTHY = 0,    /**< Guasto rilevato nel sistema di trazione. */
    T_HEALTHY           /**< Sistema di trazione nominale. */
};

/**
 * @brief Pacchetto di telemetria inviato dal veicolo al controller.
 * @note  Attributo packed per ottimizzare la banda di trasmissione.
 */
typedef struct __attribute__((packed)) {
	uint16_t sonar_l;   /**< Distanza rilevata sonar sinistro (cm/mm). */
	uint16_t sonar_c;   /**< Distanza rilevata sonar centrale. */
	uint16_t sonar_r;   /**< Distanza rilevata sonar destro. */

    int16_t rpm_FL;     /**< RPM ruota Anteriore Sinistra. */
    int16_t rpm_FR;     /**< RPM ruota Anteriore Destra. */
    int16_t rpm_RL;     /**< RPM ruota Posteriore Sinistra. */
    int16_t rpm_RR;     /**< RPM ruota Posteriore Destra. */

    uint8_t max_velocity;    /**< Limite velocità attuale impostato. */

    uint8_t battery_percent; /**< Stato di carica batteria veicolo (0-100). */
    int8_t  temperature;     /**< Temperatura sistema (°C). */

    BackwardMode backward_mode;          /**< Stato modalità retromarcia. */
    OperatingMode operating_mode;        /**< Stato modalità operativa. */
    LightOperatingMode light_mode;       /**< Stato luci. */
    DrivingMode driving_mode;            /**< Profilo di guida attivo. */
    ObstacleAvoidanceMode obstacle_mode; /**< Livello assistenza ostacoli. */
    GyroAvailability gyro_status;        /**< Stato sensore inerziale. */
    Traction_Health traction_status;     /**< Stato salute motori. */
} Telemetry_t;

/* ========================================================================== */
/* CONTROLLER API                                                             */
/* ========================================================================== */

/**
 * @brief Inizializza l'interfaccia con il controller remoto.
 * @param telecontrol Puntatore all'handle del controller.
 * @param i2c Handle I2C hardware.
 * @param address Indirizzo I2C del dispositivo remoto.
 * @return ControllerStatus_t Esito inizializzazione.
 */
ControllerStatus_t telecontrol_init(Controller_t *telecontrol, I2C_HandleTypeDef* i2c, uint16_t address);

/**
 * @brief Legge lo stato attuale degli input dal controller remoto.
 * @details Esegue una lettura I2C, verifica il CRC e aggiorna la struttura dati interna.
 * In caso di errore CRC ripetuto, mantiene l'ultimo stato valido o segnala errore.
 * @param telecontrol Puntatore all'handle del controller.
 * @return ControllerStatus_t CONTROLLER_OK se lettura valida.
 */
ControllerStatus_t telecontrol_read(Controller_t *telecontrol);

/**
 * @brief Invia il pacchetto di telemetria aggiornato al controller remoto.
 * @param telecontrol Puntatore all'handle del controller.
 * @param telemetry Puntatore alla struttura dati telemetria da inviare.
 * @return ControllerStatus_t Esito della trasmissione.
 */
ControllerStatus_t telecontrol_send_telemetry(Controller_t *telecontrol, Telemetry_t *telemetry);

/* ========================================================================== */
/* GETTERS (INLINE)                                                           */
/* ========================================================================== */

/**
 * @brief Restituisce il valore dell'asse X dello stick B.
 * @param telecontrol Puntatore all'handle.
 * @param bx Puntatore dove salvare il valore.
 * @return ControllerStatus_t OK se puntatori validi.
 */
static inline ControllerStatus_t get_telecontrol_bx(const Controller_t *telecontrol, uint16_t *bx)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (bx != NULL))
    {
        *bx = telecontrol->controller_information.controller_data.bx;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce il valore dell'asse Y dello stick A.
 * @param telecontrol Puntatore all'handle.
 * @param ay Puntatore dove salvare il valore.
 * @return ControllerStatus_t OK se puntatori validi.
 */
static inline ControllerStatus_t get_telecontrol_ay(const Controller_t *telecontrol, uint16_t *ay)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (ay != NULL))
    {
        *ay = telecontrol->controller_information.controller_data.ay;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante 1.
 * @param telecontrol Puntatore all'handle.
 * @param btn Puntatore dove salvare lo stato.
 */
static inline ControllerStatus_t get_telecontrol_button_btn1(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn1;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante 2.
 */
static inline ControllerStatus_t get_telecontrol_button_btn2(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn2;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante 3.
 */
static inline ControllerStatus_t get_telecontrol_button_btn3(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn3;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante 4.
 */
static inline ControllerStatus_t get_telecontrol_button_btn4(const Controller_t *telecontrol, uint8_t *btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (btn != NULL))
    {
        *btn = telecontrol->controller_information.controller_data.btn4;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante dello stick B.
 */
static inline ControllerStatus_t get_telecontrol_b_btn(const Controller_t *telecontrol, uint8_t *b_btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (b_btn != NULL))
    {
        *b_btn = telecontrol->controller_information.controller_data.b_btn;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce lo stato del pulsante dello stick A.
 */
static inline ControllerStatus_t get_telecontrol_a_btn(const Controller_t *telecontrol, uint8_t *a_btn)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (a_btn != NULL))
    {
        *a_btn = telecontrol->controller_information.controller_data.a_btn;
        status = CONTROLLER_OK;
    }

    return status;
}

/**
 * @brief Restituisce la percentuale batteria del controller remoto.
 */
static inline ControllerStatus_t get_telecontrol_percentage(const Controller_t *telecontrol, uint8_t *percentage)
{
    ControllerStatus_t status = CONTROLLER_ERR;

    if ((telecontrol != NULL) && (percentage != NULL))
    {
        *percentage = telecontrol->controller_information.controller_percentage;
        status = CONTROLLER_OK;
    }

    return status;
}

/* ========================================================================== */
/* TELEMETRY SETTERS API                                                      */
/* ========================================================================== */

/**
 * @brief Inizializza la struttura telemetria con valori di default sicuri.
 * @param telemetry Puntatore alla struttura.
 * @return ControllerStatus_t OK se inizializzato.
 */
ControllerStatus_t telemetry_init(Telemetry_t *telemetry);

/**
 * @brief Imposta la modalità luci nella telemetria.
 */
ControllerStatus_t telemetry_set_light_mode(Telemetry_t *telemetry, LightOperatingMode mode);

/**
 * @brief Imposta la modalità operativa generale nella telemetria.
 */
ControllerStatus_t telemetry_set_operating_mode(Telemetry_t *telemetry, OperatingMode mode);

/**
 * @brief Imposta la modalità retromarcia nella telemetria.
 */
ControllerStatus_t telemetry_set_backward_mode(Telemetry_t *telemetry, BackwardMode mode);

/**
 * @brief Imposta il profilo di guida nella telemetria.
 */
ControllerStatus_t telemetry_set_driving_mode(Telemetry_t *telemetry, DrivingMode  mode);

/**
 * @brief Imposta la modalità di evitamento ostacoli.
 */
ControllerStatus_t telemetry_set_obstacle_avoidance_mode(Telemetry_t *telemetry, ObstacleAvoidanceMode  mode);

/**
 * @brief Imposta la percentuale batteria del veicolo.
 */
ControllerStatus_t telemetry_set_battery(Telemetry_t *telemetry, uint8_t percent);

/**
 * @brief Imposta la temperatura del sistema.
 */
ControllerStatus_t telemetry_set_temperature(Telemetry_t *telemetry, int8_t temp);

/**
 * @brief Imposta i valori RPM per le quattro ruote.
 */
ControllerStatus_t telemetry_set_rpm(Telemetry_t *telemetry, int16_t fl, int16_t fr,int16_t rl, int16_t rr);

/**
 * @brief Imposta il limite di velocità massimo corrente.
 */
ControllerStatus_t telemetry_set_max_velocity(Telemetry_t *telemetry, uint8_t max_vel);

/**
 * @brief Imposta le letture dei sensori di distanza (sonar).
 */
ControllerStatus_t telemetry_set_sonars(Telemetry_t *telemetry, uint16_t left, uint16_t center, uint16_t right);

/**
 * @brief Imposta lo stato di disponibilità del giroscopio.
 */
ControllerStatus_t telemetry_set_gyro_status(Telemetry_t *telemetry, GyroAvailability gyro_status);

/**
 * @brief Imposta lo stato di salute della trazione.
 */
ControllerStatus_t telemetry_set_traction_health(Telemetry_t *telemetry, Traction_Health traction_status);

#endif /* INC_CONTROLLER_H_ */
