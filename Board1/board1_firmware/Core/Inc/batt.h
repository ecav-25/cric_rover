/**
 * @file batt.h
 * @author Gruppo 2
 * @brief Gestione della lettura batteria tramite ADC per STM32G4.
 * * Questo modulo fornisce le astrazioni per leggere il valore grezzo dell'ADC
 * e convertirlo in tensione reale tenendo conto del partitore resistivo.
 */

#ifndef INC_BATT_H_
#define INC_BATT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* ================== RETURN CODES ================== */

/**
 * @brief Codici di ritorno per le funzioni del modulo batteria.
 */
typedef enum {
    BATT_OK  = 0,    /**< Operazione completata con successo */
    BATT_ERR,        /**< Errore generico */
    BATT_ERR_COMM    /**< Errore di comunicazione con l'ADC */
} Batt_Status_t;

/* ================== ADC / BATTERY CONSTANTS ================== */

#define BATT_ADC_VREF_MV        (3300.0f)     /**< Tensione di riferimento ADC in millivolt */
#define BATT_ADC_MAX_CNT        (4095.0f)     /**< Risoluzione massima ADC (12-bit) */
#define BATT_R1_OHM             (100000.0f)   /**< Resistenza lato alto del partitore (Ohm) */
#define BATT_R2_OHM             (27000.0f)    /**< Resistenza lato basso del partitore (Ohm) */
#define BATT_DEFAULT_TIMEOUT_MS (10U)         /**< Timeout di default per le operazioni ADC */

/* ================== OBJECT ================== */

/**
 * @brief Struttura di gestione del modulo batteria.
 */
typedef struct
{
    ADC_HandleTypeDef *hadc;             /**< Puntatore all'handle dell'ADC di STM32 HAL */
    ADC_ChannelConfTypeDef channel_cfg;  /**< Configurazione del canale ADC specifico */
    uint32_t timeout_ms;                 /**< Timeout per la lettura */
    uint32_t raw_last;                   /**< Ultimo valore grezzo letto */
    float    volt_last;                  /**< Ultimo valore di tensione calcolato */
} batt_t;

/* ================== API ================== */

/**
 * @brief Inizializza l'oggetto batteria.
 * * @param b Puntatore alla struttura della batteria.
 * @param hadc Puntatore all'handle ADC HAL.
 * @param channel_cfg Configurazione del canale ADC.
 * @param timeout_ms Timeout per le operazioni di polling.
 * @return Batt_Status_t BATT_OK se l'inizializzazione ha successo.
 */
Batt_Status_t batt_init(batt_t *b, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef *channel_cfg, uint32_t timeout_ms);

/**
 * @brief Esegue una lettura ADC grezza singola.
 * * @param b Puntatore alla struttura della batteria.
 * @param raw Puntatore dove verrà salvato il valore grezzo (0-4095).
 * @return Batt_Status_t BATT_OK se la lettura è corretta, BATT_ERR_COMM in caso di timeout.
 */
Batt_Status_t batt_read_raw_once(batt_t *b, uint32_t *raw);

/**
 * @brief Calcola la tensione della batteria (Volt) tramite una lettura singola.
 * * @param b Puntatore alla struttura della batteria.
 * @param volt Puntatore dove verrà salvato il valore in Volt calcolato.
 * @return Batt_Status_t BATT_OK se il calcolo è avvenuto con successo.
 */
Batt_Status_t batt_get_voltage_once(batt_t *b, float *volt);


#ifdef __cplusplus
}
#endif

#endif /* INC_BATT_H_ */
