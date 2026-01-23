#ifndef INC_ENCODER_H_
#include "tim.h"

#define INC_ENCODER_H_

#define PULSES_PER_REVOLUTION (24)   // Numero di impulsi per giro dell’encoder (su un solo fronte su entrambi i canali)
#define GEAR_RATIO (51)              // Rapporto di riduzione del motore
/*
 * Struttura per la gestione dell’encoder incrementale.
 * Contiene le informazioni sul timer associato, il conteggio precedente,
 * il numero di impulsi per giro, il rapporto di riduzione, il periodo di lettura
 * e il valore di autoreload del timer.
 */
typedef struct {
    TIM_HandleTypeDef* htim;          // Handle del timer configurato in modalità encoder
    uint32_t prev_count;              // Conteggio precedente del timer per il calcolo della differenza
    uint8_t pulse_per_revolution;     // Numero di impulsi per giro (su un solo fronte di un canale)
    float gear_motor_ratio;           // Rapporto di riduzione tra inner revolution e outer revolution
    float reading_period;             // Periodo di aggiornamento della misura (in secondi)
    uint32_t arr;                     // Valore di autoreload del timer
    float velocity;
} EncoderHandle;

/*
 * Inizializza l’encoder:
 *  - associa il timer e memorizza il conteggio iniziale,
 *  - avvia il timer in modalità encoder,
 *  - imposta i parametri caratteristici (impulsi per giro, rapporto di riduzione, periodo di campionamento, valore di autoreload).
 */
void encoder_init(EncoderHandle* enc, TIM_HandleTypeDef* htim, uint8_t pulse_per_revolution, float gear_motor_ratio, float reading_period);

/*
 * Calcola la velocità in RPM dell’encoder.
 *  - Legge il valore corrente del contatore e confronta con quello precedente per ottenere la differenza.
 *  - Gestisce i casi di overflow/underflow del timer.
 *  - Determina la direzione di rotazione in base al flag di conteggio (up/down).
 *  - Converte la differenza di impulsi in giri al minuto considerando il numero di impulsi per giro,
 *    		il periodo di lettura e il rapporto di riduzione del motoriduttore.
 *  - Se il timer è configurato in modalità X4 (conteggio su entrambi i canali), divide la velocità per due.
 *  - Aggiorna il conteggio precedente per il ciclo successivo.
 */
void encoder_readRPM(EncoderHandle* enc);

#endif /* INC_ENCODER_H_ */
