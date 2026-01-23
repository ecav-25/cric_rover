#ifndef BOARD_FUNCTIONS_H_
#define BOARD_FUNCTIONS_H_

#include <stdint.h>
#include <stddef.h>
#include "Board2_types.h"

#define PING    0xAA

typedef struct {
  StateBusB2 state;
  uint32_T crc;
} PacketStateB2;

boolean_T UART_Data_Sended(void);

boolean_T UART_Data_Received(void);

void UART_DMA_Stop(void);

void UART_Send_State(const void*s);

void UART_Send_GlobalState(const void* gs);

void UART_Send_Decision(const void* dec);

void UART_Send_Ping(void);

void UART_Wait_State(void* s);

void UART_Wait_GlobalState(void* gs);

void UART_Wait_Decision(void* dec);

void UART_Wait_Ping(void* ping);

boolean_T Correct_CRC_State(const void* s_packet);

boolean_T Correct_CRC_GlobalState(const void* gs_packet);

boolean_T Correct_CRC_Decision(const void* dec_packet);

boolean_T Correct_Ping(uint8_t rec_ping);

void set_Session(void);

void reset_Session(void);

void set_MTALK(void);

void reset_MTALK(void);

boolean_T is_STALK_high(void);

void set_driver_control(void);

void reset_driver_control(void);

uint32_t get_tick(void);

boolean_T check_elapsed_time_us(uint32_T start_time, uint32_T min_elapsed_time);

boolean_T check_elapsed_time(uint32_T start_time, uint32_T min_elapsed_time);

void enter_critical_section(void);

void exit_critical_section(void);

#endif

