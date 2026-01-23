#include <board_functions.h>
#include "main.h"
#include "string.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart3;
extern CRC_HandleTypeDef hcrc;
extern osMutexId_t inputMutexHandle;
extern osMutexId_t outputMutexHandle;
extern TIM_HandleTypeDef htim5;

boolean_T tx_complete = 0;
boolean_T rx_complete = 0;

// Copia StateBusB1 campo per campo
static void Copy_StateBusB1(StateBusB1* dest, const StateBusB1* src) {
    dest->battery_voltage = src->battery_voltage;
    dest->temperature = src->temperature;
    dest->velocity_FA = src->velocity_FA;
    dest->velocity_FB = src->velocity_FB;
    dest->velocity_BA = src->velocity_BA;
    dest->velocity_BB = src->velocity_BB;
}

// Copia StateBusB2 campo per campo
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

// Copia GSBus campo per campo
static void Copy_GSBus(GSBus* dest, const GSBus* src) {
    Copy_StateBusB1(&dest->stateB1, &src->stateB1);
    Copy_StateBusB2(&dest->stateB2, &src->stateB2);
    dest->mov_obs = src->mov_obs;
    dest->spc_retro = src->spc_retro;
    dest->limit_vel = src->limit_vel;
    dest->obs_detection = src->obs_detection;
}

// Copia DecBus campo per campo
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

boolean_T UART_Data_Received(void)
{
    if (rx_complete) {
        rx_complete = 0;
        return 1;
    }
    return 0;
}

boolean_T UART_Data_Sended(void)
{
    if (tx_complete) {
        tx_complete = 0;
        return 1;
    }
    return 0;
}

void UART_DMA_Stop(void)
{
    HAL_UART_DMAStop(&huart3);
}

void UART_Send_State(const void* s)
{
    static PacketStateB2 packet;

    memset(&packet, 0, sizeof(packet));
    Copy_StateBusB2(&packet.state, (const StateBusB2*)s);

    __HAL_CRC_DR_RESET(&hcrc);
    packet.crc = HAL_CRC_Calculate(&hcrc,
                                  (uint32_t*)&packet.state,
                                  sizeof(packet.state));

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t*)&packet, sizeof(packet));
}

void UART_Send_GlobalState(const void* gs)
{
    static PacketGstate packet;

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
    static PacketDecision packet;

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
    static uint8_t ping = PING;

    HAL_HalfDuplex_EnableTransmitter(&huart3);
    HAL_UART_Transmit_DMA(&huart3, &ping, sizeof(ping));
}

void UART_Wait_State(void* s)
{
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

boolean_T Correct_CRC_State(const void* s_packet)
{
    const PacketStateB1* packet = (const PacketStateB1*)s_packet;

    StateBusB1 clean_state;
    memset(&clean_state, 0, sizeof(clean_state));
    Copy_StateBusB1(&clean_state, &packet->state);

    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t crc_calc =
        HAL_CRC_Calculate(&hcrc, (uint32_t*)&clean_state, sizeof(clean_state));

    return (crc_calc == packet->crc);
}

boolean_T Correct_CRC_GlobalState(const void* gs_packet)
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

boolean_T Correct_CRC_Decision(const void* dec_packet)
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

boolean_T Correct_Ping(uint8_t rec_ping)
{
    return (rec_ping == PING);
}

void set_Session(void)
{
    HAL_GPIO_WritePin(SESSION_GPIO_Port, SESSION_Pin, GPIO_PIN_SET);
}

void reset_Session(void)
{
    HAL_GPIO_WritePin(SESSION_GPIO_Port, SESSION_Pin, GPIO_PIN_RESET);
}

void set_MTALK(void)
{
    HAL_GPIO_WritePin(MTALK_GPIO_Port, MTALK_Pin, GPIO_PIN_SET);
}

void reset_MTALK(void)
{
    HAL_GPIO_WritePin(MTALK_GPIO_Port, MTALK_Pin, GPIO_PIN_RESET);
}

boolean_T is_STALK_high(void)
{
    return (HAL_GPIO_ReadPin(STALK_GPIO_Port, STALK_Pin) == GPIO_PIN_SET);
}

void set_driver_control(void)
{
    HAL_GPIO_WritePin(SELECT_GPIO_Port, SELECT_Pin, GPIO_PIN_SET);
}

void reset_driver_control(void)
{
    HAL_GPIO_WritePin(SELECT_GPIO_Port, SELECT_Pin, GPIO_PIN_RESET);
}

uint32_t get_tick(void)
{
    return __HAL_TIM_GET_COUNTER(&htim5);
}

boolean_T check_elapsed_time_us(uint32_T start_time_us,
                                uint32_T min_elapsed_us)
{
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim5);
    return ((current_us - start_time_us) >= min_elapsed_us);
}

boolean_T check_elapsed_time(uint32_T start_time,
                             uint32_T min_elapsed_time_ms)
{
    uint32_t current_us = __HAL_TIM_GET_COUNTER(&htim5);
    uint32_t elapsed_us = current_us - start_time;

    return (elapsed_us >= (min_elapsed_time_ms * 1000U));
}

void enter_critical_section(void)
{
    taskENTER_CRITICAL();
}

void exit_critical_section(void)
{
    taskEXIT_CRITICAL();
}

