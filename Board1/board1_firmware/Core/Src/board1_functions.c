// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file board1_functions.c
 * @author Gruppo 2
 * @brief Implementazione del Hardware Abstraction Layer (HAL) per Board 1.
 * * Include l'implementazione delle funzioni di comunicazione UART via DMA,
 * calcolo CRC-32 hardware, gestione GPIO e primitive di temporizzazione.
 * Il driver è progettato per operare in un ambiente multitasking FreeRTOS.
 *
 * @copyright Copyright (c) 2026 Gruppo 2.
 * Rilasciato sotto licenza GPLv3 (consultare il file LICENSE per i dettagli).
 */

#include "board1_functions.h"
#include "main.h"
#include "string.h"
#include "cmsis_os.h"
#include "pid_law.h"

/* ========================================================================== */
/* EXTERNAL HANDLES                               */
/* ========================================================================== */

extern UART_HandleTypeDef huart3; /**< Handle UART per la comunicazione Inter-Board */
extern CRC_HandleTypeDef hcrc;    /**< Handle unità di calcolo CRC hardware */
extern TIM_HandleTypeDef htim2;   /**< Handle Timer per il cronometraggio (us/ms) */

extern PID_Law_t pid_FA; /**< Istanza PID Motore Front-A */
extern PID_Law_t pid_FB; /**< Istanza PID Motore Front-B */
extern PID_Law_t pid_BA; /**< Istanza PID Motore Back-A */
extern PID_Law_t pid_BB; /**< Istanza PID Motore Back-B */

/* ========================================================================== */
/* GLOBAL VARIABLES                               */
/* ========================================================================== */

/** @brief Flag di completamento trasmissione (gestito da ISR callback). */
boolean_T tx_complete = 0;

/** @brief Flag di completamento ricezione (gestito da ISR callback). */
boolean_T rx_complete = 0;

/* ========================================================================== */
/* STATIC HELPER FUNCTIONS (DATA COPY)                   */
/* ========================================================================== */

/**
 * @brief Copia sicura campo per campo di StateBusB1.
 * @details Previene problemi di allineamento e garantisce che il calcolo del CRC
 * avvenga esclusivamente sui dati utili, ignorando eventuali padding bytes.
 * @param dest Puntatore alla struttura di destinazione.
 * @param src Puntatore alla struttura sorgente.
 */
static void Copy_StateBusB1(StateBusB1* dest, const StateBusB1* src) {
    dest->battery_voltage = src->battery_voltage;
    dest->temperature = src->temperature;
    dest->velocity_FA = src->velocity_FA;
    dest->velocity_FB = src->velocity_FB;
    dest->velocity_BA = src->velocity_BA;
    dest->velocity_BB = src->velocity_BB;
    dest->motorError_FA = src->motorError_FA;
    dest->motorError_FB = src->motorError_FB;
    dest->motorError_BA = src->motorError_BA;
    dest->motorError_BB = src->motorError_BB;
}

/**
 * @brief Copia sicura campo per campo di StateBusB2.
 * @param dest Puntatore alla struttura di destinazione.
 * @param src Puntatore alla struttura sorgente.
 */
static void Copy_StateBusB2(StateBusB2* dest, const StateBusB2* src) {
    dest->gyroYaw = src->gyroYaw;
    dest->sonar1 = src->sonar1;
    dest->sonar2 = src->sonar2;
    dest->sonar3 = src->sonar3;
    dest->controller_y = src->controller_y;
    dest->controller_x = src->controller_x;
    dest->button1 = src->button1;
    dest->button2 = src->button2;
    dest->button3 = src->button3;
    dest->button4 = src->button4;
    dest->r_stick_button = src->r_stick_button;
    dest->l_stick_button = src->l_stick_button;
    dest->controller_battery = src->controller_battery;
    dest->controllerError = src->controllerError;
    dest->gyroError = src->gyroError;
}

/**
 * @brief Copia ricorsiva sicura per la struttura Global State Bus.
 * @param dest Puntatore alla struttura di destinazione.
 * @param src Puntatore alla struttura sorgente.
 */
static void Copy_GSBus(GSBus* dest, const GSBus* src) {
    Copy_StateBusB1(&dest->stateB1, &src->stateB1);
    Copy_StateBusB2(&dest->stateB2, &src->stateB2);
    dest->mov_obs = src->mov_obs;
    dest->spc_retro = src->spc_retro;
    dest->limit_vel = src->limit_vel;
    dest->change_vel = src->change_vel;
    dest->obs_detection = src->obs_detection;
}

/**
 * @brief Copia sicura per la struttura Decision Bus.
 * @param dest Puntatore alla struttura di destinazione.
 * @param src Puntatore alla struttura sorgente.
 */
static void Copy_DecBus(DecBus* dest, const DecBus* src) {
    dest->rif_FA = src->rif_FA;
    dest->rif_FB = src->rif_FB;
    dest->rif_BA = src->rif_BA;
    dest->rif_BB = src->rif_BB;
    dest->brk_mode = src->brk_mode;
    dest->led_A = src->led_A;
    dest->led_B = src->led_B;
    dest->rear_led = src->rear_led;
    dest->rear_sign = src->rear_sign;
    dest->mode = src->mode;
    dest->relay = src->relay;
    dest->mux = src->mux;
}

/* ========================================================================== */
/* UART FUNCTIONS                                 */
/* ========================================================================== */

/**
 * @brief Verifica se la trasmissione UART DMA è stata completata.
 * @return boolean_T 1 se completata (e resetta il flag), 0 altrimenti.
 */
boolean_T UART_Is_Tx_Complete(void)
{
    if (tx_complete) {
        tx_complete = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se la ricezione UART DMA è stata completata.
 * @return boolean_T 1 se completata (e resetta il flag), 0 altrimenti.
 */
boolean_T UART_Is_Rx_Complete(void)
{
    if (rx_complete) {
        rx_complete = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief Arresta immediatamente le operazioni DMA sulla UART3.
 */
void UART_Stop_DMA(void)
{
    HAL_UART_DMAStop(&huart3);
}

/**
 * @brief Invia lo stato locale (Board 1) tramite UART DMA in Half-Duplex.
 * @note Utilizza un buffer statico per garantire l'integrità dei dati durante il trasferimento DMA.
 * @param s Puntatore alla struttura StateBusB1 da inviare.
 */
void UART_Send_Local_State(const void* s)
{
    static PacketStateB1 packet;

    (void)memset(&packet, 0, sizeof(packet));
    Copy_StateBusB1(&packet.state, (const StateBusB1*)s);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&packet.state, sizeof(packet.state));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

/**
 * @brief Invia lo stato globale (GSBus) tramite UART DMA.
 * @param gs Puntatore alla struttura GSBus.
 */
void UART_Send_GlobalState(const void* gs)
{
    static PacketGstate packet;

    (void)memset(&packet, 0, sizeof(packet));
    Copy_GSBus(&packet.global_state, (const GSBus*)gs);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&packet.global_state, sizeof(packet.global_state));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

/**
 * @brief Invia il pacchetto decisionale tramite UART DMA.
 * @param dec Puntatore alla struttura DecBus.
 */
void UART_Send_Decision(const void* dec)
{
    static PacketDecision packet;

    (void)memset(&packet, 0, sizeof(packet));
    Copy_DecBus(&packet.decision, (const DecBus*)dec);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&packet.decision, sizeof(packet.decision));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

/**
 * @brief Invia un segnale di PING (byte singolo) per sincronizzazione.
 */
void UART_Send_Ping(void)
{
    static uint8_t ping = PING;
    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, &ping, sizeof(ping));
}

/**
 * @brief Configura la UART in ricezione DMA per lo stato di Board 2.
 * @param s Puntatore al buffer di destinazione (PacketStateB2).
 */
void UART_Wait_State(void* s)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)s, sizeof(PacketStateB2));
}

/**
 * @brief Configura la UART in ricezione DMA per lo stato globale.
 * @param gs Puntatore al buffer di destinazione (PacketGstate).
 */
void UART_Wait_GlobalState(void* gs)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)gs, sizeof(PacketGstate));
}

/**
 * @brief Configura la UART in ricezione DMA per il pacchetto decisionale.
 * @param dec Puntatore al buffer di destinazione (PacketDecision).
 */
void UART_Wait_Decision(void* dec)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)dec, sizeof(PacketDecision));
}

/**
 * @brief Configura la UART in ricezione DMA per un singolo byte di PING.
 * @param ping Puntatore alla variabile di destinazione.
 */
void UART_Wait_Ping(void* ping)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)ping, sizeof(uint8_t));
}

/**
 * @brief Verifica la validità del byte di PING ricevuto.
 * @param rec_ping Byte ricevuto.
 * @return boolean_T 1 se valido, 0 altrimenti.
 */
boolean_T UART_Check_Ping(uint8_t rec_ping)
{
    return (rec_ping == PING);
}

/* ========================================================================== */
/* CRC FUNCTIONS                                  */
/* ========================================================================== */

/**
 * @brief Esegue la validazione CRC di un pacchetto StateBusB2.
 * @param s_packet Puntatore al pacchetto ricevuto.
 * @return boolean_T 1 se il CRC calcolato corrisponde a quello ricevuto.
 */
boolean_T CRC_Check_State(const void* s_packet)
{
    const PacketStateB2* packet = (const PacketStateB2*)s_packet;
    StateBusB2 clean_state;

    (void)memset(&clean_state, 0, sizeof(clean_state));
    Copy_StateBusB2(&clean_state, &packet->state);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_state, sizeof(clean_state));

    return (crc_calc == packet->crc);
}

/**
 * @brief Esegue la validazione CRC di un pacchetto GSBus.
 * @param gs_packet Puntatore al pacchetto ricevuto.
 * @return boolean_T 1 se valido.
 */
boolean_T CRC_Check_GlobalState(const void* gs_packet)
{
    const PacketGstate* packet = (const PacketGstate*)gs_packet;
    GSBus clean_state;

    (void)memset(&clean_state, 0, sizeof(clean_state));
    Copy_GSBus(&clean_state, &packet->global_state);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_state, sizeof(clean_state));

    return (crc_calc == packet->crc);
}

/**
 * @brief Esegue la validazione CRC di un pacchetto Decision.
 * @param dec_packet Puntatore al pacchetto ricevuto.
 * @return boolean_T 1 se valido.
 */
boolean_T CRC_Check_Decision(const void* dec_packet)
{
    const PacketDecision* packet = (const PacketDecision*)dec_packet;
    DecBus clean_decision;

    (void)memset(&clean_decision, 0, sizeof(clean_decision));
    Copy_DecBus(&clean_decision, &packet->decision);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_decision, sizeof(clean_decision));

    return (crc_calc == packet->crc);
}

/* ========================================================================== */
/* GPIO & SIGNALS                                 */
/* ========================================================================== */

/**
 * @brief Legge lo stato del pin SESSION per determinare la modalità operativa.
 * @return boolean_T 1 se il pin è SET.
 */
boolean_T IO_Read_Session(void)
{
    return (HAL_GPIO_ReadPin(SESSION_GPIO_Port, SESSION_Pin) == GPIO_PIN_SET);
}

/**
 * @brief Attiva il segnale SlaveTalk verso l'altra board.
 */
void IO_Set_SlaveTalk(void)
{
    HAL_GPIO_WritePin(STALK_GPIO_Port, STALK_Pin, GPIO_PIN_SET);
}

/**
 * @brief Disattiva il segnale SlaveTalk.
 */
void IO_Reset_SlaveTalk(void)
{
    HAL_GPIO_WritePin(STALK_GPIO_Port, STALK_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Legge lo stato del segnale MasterTalk proveniente dall'altra board.
 * @return boolean_T 1 se MasterTalk è attivo.
 */
boolean_T IO_Read_MasterTalk(void)
{
    return (HAL_GPIO_ReadPin(MTALK_GPIO_Port, MTALK_Pin) == GPIO_PIN_SET);
}

/* ========================================================================== */
/* PID                                            */
/* ========================================================================== */

/**
 * @brief Esegue il reset completo dello stato interno (integrale e derivativo)
 * di tutti i regolatori PID dei motori.
 */
void PID_Reset(void)
{
	(void)PID_Law_reset(&pid_FA);
	(void)PID_Law_reset(&pid_FB);
	(void)PID_Law_reset(&pid_BA);
	(void)PID_Law_reset(&pid_BB);
}


/* ========================================================================== */
/* TIMING & OS                                    */
/* ========================================================================== */

/**
 * @brief Legge il valore corrente del contatore hardware (microsecondi).
 * @return uint32_t Tempo corrente in us.
 */
uint32_t Time_Get_Tick(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

/**
 * @brief Verifica se è trascorso un intervallo in microsecondi.
 * @param start_time_us Tempo di inizio (us).
 * @param min_elapsed_us Intervallo minimo richiesto (us).
 * @return boolean_T 1 se l'intervallo è trascorso.
 */
boolean_T Time_Check_Elapsed_us(uint32_T start_time_us, uint32_T min_elapsed_us)
{
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim2);
    return ((current_us - start_time_us) >= min_elapsed_us);
}

/**
 * @brief Verifica se è trascorso un intervallo in millisecondi.
 * @details Converte internamente i millisecondi in microsecondi per il confronto.
 * @param start_time Tempo di inizio in microsecondi.
 * @param min_elapsed_time_ms Intervallo minimo in millisecondi.
 * @return boolean_T 1 se l'intervallo è trascorso.
 */
boolean_T Time_Check_Elapsed_ms(uint32_T start_time, uint32_T min_elapsed_time_ms)
{
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t elapsed_us = current_us - start_time;

    return (elapsed_us >= (min_elapsed_time_ms * 1000U));
}

/**
 * @brief Entra in una sezione critica di FreeRTOS (disabilita interrupt).
 */
void OS_Enter_Critical(void)
{
    taskENTER_CRITICAL();
}

/**
 * @brief Esce dalla sezione critica di FreeRTOS.
 */
void OS_Exit_Critical(void)
{
    taskEXIT_CRITICAL();
}
