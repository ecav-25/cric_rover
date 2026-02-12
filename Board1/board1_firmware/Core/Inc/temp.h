/**
 * @file temp.h
 * @author Gruppo 2
 * @brief Driver per la lettura del sensore di temperatura interno dell'STM32G4.
 * * Il modulo utilizza l'ADC interno per misurare la tensione generata dal sensore
 * integrato nel chip e la converte in gradi Celsius utilizzando i parametri
 * caratteristici (V25 e Slope) tipici della famiglia STM32G4.
 */

#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include <stdint.h>

/* ================== RETURN CODES ================== */

/**
 * @brief Codici di ritorno per le funzioni del sensore di temperatura.
 */
typedef enum {
    TEMP_OK = 0,      /**< Operazione completata correttamente */
    TEMP_ERR,         /**< Errore generico di configurazione */
    TEMP_ERR_COMM     /**< Errore di timeout o comunicazione con l'ADC */
} Temp_Status_t;

/* ================== SENSOR CONSTANTS ================== */

#define TEMP_ADC_VREF_MV        (3300.0f)           /**< Tensione di riferimento ADC in millivolt */
#define TEMP_ADC_MAX_CNT        (4095.0f)           /**< Risoluzione massima ADC (12-bit) */

#define TEMP_V25_MV             (760.0f)            /**< Tensione del sensore a 25°C */
#define TEMP_SLOPE_MV_PER_C     (2.5f)              /**< Pendenza della variazione (mV/°C) */

#define TEMP_DEFAULT_SMPL_TIME  (ADC_SAMPLETIME_247CYCLES_5) /**< Tempo di campionamento minimo consigliato */
#define TEMP_DEFAULT_TIMEOUT_MS (10U)               /**< Timeout di default per l'acquisizione polling */

/* ================== OBJECT ================== */

/**
 * @brief Struttura di gestione per il sensore di temperatura.
 */
typedef struct
{
    ADC_HandleTypeDef *hadc;            /**< Puntatore all'handle dell'ADC STM32 */
    ADC_ChannelConfTypeDef channel_cfg; /**< Configurazione del canale ADC specifico */
    uint32_t timeout_ms;                /**< Timeout per le operazioni di lettura */

    /* ultimi valori acquisiti */
    uint32_t raw_last;                  /**< Ultimo valore grezzo letto dall'ADC */
    float    temp_c_last;               /**< Ultima temperatura calcolata in gradi Celsius */

} temp_t;

/* ================== API ================== */

/**
 * @brief Inizializza l'oggetto sensore di temperatura.
 * * @param t Puntatore alla struttura temp_t da inizializzare.
 * @param hadc Puntatore all'handle ADC hardware.
 * @param channel_cfg Configurazione del canale ADC associato al sensore interno.
 * @param timeout_ms Timeout massimo per le operazioni di polling ADC.
 * @return Temp_Status_t TEMP_OK se l'inizializzazione ha successo.
 */
Temp_Status_t temp_init(temp_t *t, ADC_HandleTypeDef *hadc, const ADC_ChannelConfTypeDef* channel_cfg, uint32_t timeout_ms);

/**
 * @brief Esegue una lettura ADC grezza singola.
 * * @param t Puntatore all'oggetto temperatura.
 * @param raw Puntatore alla variabile dove salvare il valore a 12 bit letto.
 * @return Temp_Status_t TEMP_OK se la lettura è avvenuta correttamente.
 */
Temp_Status_t temp_read_raw_once(temp_t *t, uint32_t *raw);

/**
 * @brief Acquisisce la temperatura attuale convertita in gradi Celsius.
 * * La funzione esegue una lettura ADC ed esegue la conversione
 * @param temp_c Puntatore alla variabile float dove salvare il valore in °C.
 * @return Temp_Status_t TEMP_OK se l'acquisizione e il calcolo sono riusciti.
 */
Temp_Status_t temp_get_celsius_once(temp_t *t, float *temp_c);

#ifdef __cplusplus
}
#endif

#endif /* INC_TEMP_H_ */
