#ifndef BOARD2_FUNCTIONS_H_
#define BOARD2_FUNCTIONS_H_

/**
 * @file board_functions.h
 * @brief Hardware Abstraction Layer (HAL) & Stub Interface per Board 2 (Master).
 *
 * Questo file definisce i prototipi per l'interazione con l'hardware a basso livello
 * (UART, GPIO, Timer) e le primitive di sincronizzazione utilizzate dal codice
 * generato da Simulink/Embedded Coder.
 */

#include <stdint.h>
#include <stddef.h>
#include "Board2_types.h" /* Definizioni dei tipi Simulink per Board 2 */

/* ========================================================================== */
/* 								   DEFINES                                    */
/* ========================================================================== */

/** @brief Byte di sincronizzazione per il protocollo di comunicazione. */
#define PING    0xAA

/* ========================================================================== */
/* 								  DATA TYPES                                  */
/* ========================================================================== */

/**
 * @brief Struttura pacchetto per lo Stato della Board 2.
 * @details Include i dati funzionali (StateBusB2) e il campo CRC per
 * l'integrità dei dati. Utilizzata per la serializzazione.
 */
typedef struct {
  StateBusB2 state; /**< Payload dati (definito in Board2_types.h) */
  uint32_T crc;     /**< Checksum CRC-32 calcolato sul payload */
} PacketStateB2;

/* ========================================================================== */
/* 							   UART FUNCTIONS                                 */
/* ========================================================================== */

/**
 * @brief Verifica se la trasmissione UART precedente è stata completata.
 * @return true se la trasmissione è stata completata, false altrimenti.
 */
boolean_T UART_Is_Tx_Complete(void);

/**
 * @brief Verifica la presenza di nuovi dati nel buffer di ricezione UART.
 * @return true la ricezione è avvenuta, false altrimenti.
 */
boolean_T UART_Is_Rx_Complete(void);

/**
 * @brief Interrompe forzatamente qualsiasi trasferimento DMA sulla UART.
 * @details Usato per reset errori di comunicazione.
 */
void UART_Stop_DMA(void);

/**
 * @brief Invia il pacchetto 'State' tramite UART (Non bloccante/DMA).
 * @param[in] s Puntatore alla struttura dati State da inviare.
 */
void UART_Send_Local_State(const void* s);

/**
 * @brief Invia il pacchetto 'GlobalState' tramite UART (Non bloccante/DMA).
 * @param[in] gs Puntatore alla struttura GlobalState.
 */
void UART_Send_GlobalState(const void* gs);

/**
 * @brief Invia il pacchetto 'Decision' tramite UART (Non bloccante/DMA).
 * @param[in] dec Puntatore alla struttura Decision.
 */
void UART_Send_Decision(const void* dec);

/**
 * @brief Invia il byte di PING per ripristinare la comunicazione (Non bloccante/DMA).
 */
void UART_Send_Ping(void);

/**
 * @brief Attende la ricezione di un pacchetto 'State' (Non Bloccante/DMA).
 * @param[out] s Puntatore al buffer dove salvare i dati ricevuti.
 */
void UART_Wait_State(void* s);

/**
 * @brief Attende la ricezione di un pacchetto 'GlobalState' (Non Bloccante/DMA).
 * @param[out] gs Puntatore al buffer di destinazione.
 */
void UART_Wait_GlobalState(void* gs);

/**
 * @brief Attende la ricezione di un pacchetto 'Decision' (Non Bloccante/DMA).
 * @param[out] dec Puntatore al buffer di destinazione.
 */
void UART_Wait_Decision(void* dec);

/**
 * @brief Attende la ricezione del byte di PING (Non Bloccante/DMA).
 * @param[out] ping Puntatore alla variabile byte dove salvare il ping ricevuto.
 */
void UART_Wait_Ping(void* ping);

/**
 * @brief Valida il byte di Ping ricevuto.
 * @param[in] rec_ping Valore del byte ricevuto.
 * @return true se rec_ping corrisponde alla costante PING (0xAA), false altrimenti.
 */
boolean_T UART_Check_Ping(uint8_t rec_ping);

/* ========================================================================== */
/* 							   CRC & VALIDATION                               */
/* ========================================================================== */

/**
 * @brief Verifica il CRC del pacchetto 'State' ricevuto.
 * @param[in] s_packet Puntatore al pacchetto completo (Dati + CRC).
 * @return true se il CRC calcolato coincide con quello ricevuto, false altrimenti.
 */
boolean_T CRC_Check_State(const void* s_packet);

/**
 * @brief Verifica il CRC del pacchetto 'GlobalState'.
 * @param[in] gs_packet Puntatore al pacchetto completo.
 * @return true se valido, false se corrotto.
 */
boolean_T CRC_Check_GlobalState(const void* gs_packet);

/**
 * @brief Verifica il CRC del pacchetto 'Decision'.
 * @param[in] dec_packet Puntatore al pacchetto completo.
 * @return true se valido, false se corrotto.
 */
boolean_T CRC_Check_Decision(const void* dec_packet);

/* ========================================================================== */
/* 							   GPIO & SIGNALS                                 */
/* ========================================================================== */

/**
 * @brief Imposta a livello ALTO il segnale SESSION.
 * @details Indica l'inizio di una sessione di comunicazione verso lo Slave.
 */
void IO_Set_Session(void);

/**
 * @brief Imposta a livello BASSO il segnale SESSION.
 * @details Termina la sessione corrente o resetta lo stato.
 */
void IO_Reset_Session(void);

/**
 * @brief Imposta a livello ALTO il segnale M_TALK (Master Talk).
 * @details Segnala allo Slave che il Master sta per trasmettere.
 */
void IO_Set_MasterTalk(void);

/**
 * @brief Imposta a livello BASSO il segnale M_TALK.
 * @details Rilascia la linea di comunicazione Master.
 */
void IO_Reset_MasterTalk(void);

/**
 * @brief Legge lo stato del pin S_TALK (Slave Talk).
 * @return true se lo Slave sta asserendo la linea.
 */
boolean_T IO_Read_SlaveTalk(void);

/* ========================================================================== */
/* 							   TIMING & OS                                    */
/* ========================================================================== */

/**
 * @brief Restituisce il tick di sistema corrente.
 * @return Valore del contatore temporale.
 */
uint32_t Time_Get_Tick(void);

/**
 * @brief Verifica se è trascorso un intervallo di tempo in microsecondi.
 * @param[in] start_time Timestamp iniziale (in us).
 * @param[in] min_elapsed_time Tempo minimo da attendere (in us).
 * @return true se (current_time - start_time) >= min_elapsed_time.
 */
boolean_T Time_Check_Elapsed_us(uint32_T start_time, uint32_T min_elapsed_time);

/**
 * @brief Verifica se è trascorso un intervallo di tempo in ms.
 * @param[in] start_time Timestamp iniziale (in us).
 * @param[in] min_elapsed_time Tempo minimo da attendere (in ms).
 * @return true se il tempo è trascorso.
 */
boolean_T Time_Check_Elapsed_ms(uint32_T start_time, uint32_T min_elapsed_time);

/**
 * @brief Entra in una Sezione Critica (Disabilita Interrupt globali).
 * @warning Deve essere breve per non bloccare il real-time system.
 * Ogni chiamata deve essere bilanciata da una exit_critical_section.
 */
void OS_Enter_Critical(void);

/**
 * @brief Esce dalla Sezione Critica (Riabilita Interrupt globali).
 */
void OS_Exit_Critical(void);

#endif /* BOARD_FUNCTIONS_H_ */
