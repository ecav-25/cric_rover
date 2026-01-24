/**
 * @file board_functions.c
 * @brief Implementazione del Hardware Abstraction Layer (HAL) per Board 2 (Master).
 *
 * Include l'implementazione delle funzioni di comunicazione UART via DMA,
 * calcolo CRC-32, gestione GPIO (Master Control) e primitive di sistema.
 */

#include "board_functions.h"
#include "main.h"
#include "string.h"
#include "cmsis_os.h"

/* ========================================================================== */
/* 							   EXTERNAL HANDLES                               */
/* ========================================================================== */

extern UART_HandleTypeDef huart3; /**< Handle UART per la comunicazione Inter-Board */
extern CRC_HandleTypeDef hcrc;    /**< Handle unità di calcolo CRC hardware */
extern TIM_HandleTypeDef htim5;   /**< Handle Timer ad alta risoluzione (us) */

/* ========================================================================== */
/* 							   GLOBAL VARIABLES                               */
/* ========================================================================== */

/** @brief Flag di completamento trasmissione (gestito da ISR callback). */
boolean_T tx_complete = 0;

/** @brief Flag di completamento ricezione (gestito da ISR callback). */
boolean_T rx_complete = 0;

/* ========================================================================== */
/* 					    STATIC HELPER FUNCTIONS (DATA COPY)                   */
/* ========================================================================== */

/**
 * @brief Copia sicura campo per campo di StateBusB1.
 * @details Utilizzata quando si riceve lo stato dallo Slave.
 */
static void Copy_StateBusB1(StateBusB1* dest, const StateBusB1* src) {
    dest->battery_voltage = src->battery_voltage;
    dest->temperature = src->temperature;
    dest->velocity_FA = src->velocity_FA;
    dest->velocity_FB = src->velocity_FB;
    dest->velocity_BA = src->velocity_BA;
    dest->velocity_BB = src->velocity_BB;
}

/**
 * @brief Copia sicura campo per campo di StateBusB2.
 * @details Necessaria per serializzare lo stato del Master prima dell'invio.
 */
static void Copy_StateBusB2(StateBusB2* dest, const StateBusB2* src) {
    dest->acceleration_y = src->acceleration_y;
    dest->acceleration_x = src->acceleration_x;
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
}

/**
 * @brief Copia sicura per la struttura Global State.
 */
static void Copy_GSBus(GSBus* dest, const GSBus* src) {
    Copy_StateBusB1(&dest->stateB1, &src->stateB1);
    Copy_StateBusB2(&dest->stateB2, &src->stateB2);
    dest->mov_obs = src->mov_obs;
    dest->spc_retro = src->spc_retro;
    dest->limit_vel = src->limit_vel;
    dest->obs_detection = src->obs_detection;
}

/**
 * @brief Copia sicura per la struttura Decision.
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
}

/* ========================================================================== */
/* 							   UART FUNCTIONS                                 */
/* ========================================================================== */

boolean_T UART_Is_Tx_Complete(void)
{
    if (tx_complete) {
        tx_complete = 0; /* Reset flag dopo la lettura */
        return 1;
    }
    return 0;
}

boolean_T UART_Is_Rx_Complete(void)
{
    if (rx_complete) {
        rx_complete = 0; /* Reset flag dopo la lettura */
        return 1;
    }
    return 0;
}

void UART_Stop_DMA(void)
{
    HAL_UART_DMAStop(&huart3);
}

void UART_Send_Local_State(const void* s)
{
    /* * NOTA: 'packet' deve essere static per persistenza DMA. */
    static PacketStateB2 packet;

    /* 1. Pulizia memoria */
    memset(&packet, 0, sizeof(packet));

    /* 2. Copia dati (Invia stato Board 2) */
    Copy_StateBusB2(&packet.state, (const StateBusB2*)s);

    /* 3. Calcolo CRC */
    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc,
                                  (uint32_t*)&packet.state,
                                  sizeof(packet.state));

    /* 4. Trasmissione DMA */
    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

void UART_Send_GlobalState(const void* gs)
{
    static PacketGstate packet; /* Static per persistenza DMA */

    memset(&packet, 0, sizeof(packet));
    Copy_GSBus(&packet.global_state, (const GSBus*)gs);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc,
                                  (uint32_t*)&packet.global_state,
                                  sizeof(packet.global_state));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

void UART_Send_Decision(const void* dec)
{
    static PacketDecision packet; /* Static per persistenza DMA */

    memset(&packet, 0, sizeof(packet));
    Copy_DecBus(&packet.decision, (const DecBus*)dec);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc,
                                  (uint32_t*)&packet.decision,
                                  sizeof(packet.decision));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

void UART_Send_Ping(void)
{
    static uint8_t ping = PING; /* Static per persistenza DMA */

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, &ping, sizeof(ping));
}

void UART_Wait_State(void* s)
{
    /* Master attende lo stato della Board 1 (Slave) */
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)s, sizeof(PacketStateB1));
}

void UART_Wait_GlobalState(void* gs)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)gs, sizeof(PacketGstate));
}

void UART_Wait_Decision(void* dec)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)dec, sizeof(PacketDecision));
}

void UART_Wait_Ping(void* ping)
{
    HAL_HalfDuplex_EnableReceiver(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)ping, sizeof(uint8_t));
}

/* ========================================================================== */
/* 							   CRC & VALIDATION                               */
/* ========================================================================== */

boolean_T CRC_Check_State(const void* s_packet)
{
    /* Verifica CRC su pacchetto Board 1 ricevuto */
    const PacketStateB1* packet = (const PacketStateB1*)s_packet;

    StateBusB1 clean_state;
    memset(&clean_state, 0, sizeof(clean_state));
    Copy_StateBusB1(&clean_state, &packet->state);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc =
        HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_state, sizeof(clean_state));

    return (crc_calc == packet->crc);
}

boolean_T CRC_Check_GlobalState(const void* gs_packet)
{
    const PacketGstate* packet = (const PacketGstate*)gs_packet;

    GSBus clean_state;
    memset(&clean_state, 0, sizeof(clean_state));
    Copy_GSBus(&clean_state, &packet->global_state);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc =
        HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_state, sizeof(clean_state));

    return (crc_calc == packet->crc);
}

boolean_T CRC_Check_Decision(const void* dec_packet)
{
    const PacketDecision* packet = (const PacketDecision*)dec_packet;

    DecBus clean_decision;
    memset(&clean_decision, 0, sizeof(clean_decision));
    Copy_DecBus(&clean_decision, &packet->decision);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc =
        HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_decision,
                          sizeof(clean_decision));

    return (crc_calc == packet->crc);
}

boolean_T UART_Check_Ping(uint8_t rec_ping)
{
    return (rec_ping == PING);
}

/* ========================================================================== */
/* 							   GPIO & SIGNALS                                 */
/* ========================================================================== */

void IO_Set_Session(void)
{
    /* Output: Master attiva la sessione */
    HAL_GPIO_WritePin(SESSION_GPIO_Port, SESSION_Pin, GPIO_PIN_SET);
}

void IO_Reset_Session(void)
{
    HAL_GPIO_WritePin(SESSION_GPIO_Port, SESSION_Pin, GPIO_PIN_RESET);
}

void IO_Set_MasterTalk(void)
{
    /* Output: Master richiede la linea */
    HAL_GPIO_WritePin(MTALK_GPIO_Port, MTALK_Pin, GPIO_PIN_SET);
}

void IO_Reset_MasterTalk(void)
{
    HAL_GPIO_WritePin(MTALK_GPIO_Port, MTALK_Pin, GPIO_PIN_RESET);
}

boolean_T IO_Read_SlaveTalk(void)
{
    /* Input: Legge lo stato dello Slave */
    return (HAL_GPIO_ReadPin(STALK_GPIO_Port, STALK_Pin) == GPIO_PIN_SET);
}

void IO_Set_DriverControl(void)
{
    /* Abilita driver motori */
    HAL_GPIO_WritePin(SELECT_GPIO_Port, SELECT_Pin, GPIO_PIN_SET);
}

void IO_Reset_DriverControl(void)
{
    /* Disabilita driver */
    HAL_GPIO_WritePin(SELECT_GPIO_Port, SELECT_Pin, GPIO_PIN_RESET);
}

/* ========================================================================== */
/* 							   TIMING & OS                                    */
/* ========================================================================== */

uint32_t Time_Get_Tick(void)
{
    /* Restituisce il valore corrente del Timer 5 */
    return __HAL_TIM_GET_COUNTER(&htim5);
}

boolean_T Time_Check_Elapsed_us(uint32_T start_time_us,
                                uint32_T min_elapsed_us)
{
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim5);
    return ((current_us - start_time_us) >= min_elapsed_us);
}

boolean_T Time_Check_Elapsed_ms(uint32_T start_time,
                                uint32_T min_elapsed_time_ms)
{
    /* Conversione ms -> us per confronto con Timer 5 */
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim5);
    uint32_t elapsed_us = current_us - start_time;

    return (elapsed_us >= (min_elapsed_time_ms * 1000U));
}

void OS_Enter_Critical(void)
{
    /* Disabilita interrupt/context switch */
    taskENTER_CRITICAL();
}

void OS_Exit_Critical(void)
{
    /* Riabilita interrupt */
    taskEXIT_CRITICAL();
}
