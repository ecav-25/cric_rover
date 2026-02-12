#include <board2_functions.h>

boolean_T UART_Is_Tx_Complete(void){}

boolean_T UART_Is_Rx_Complete(void){}

void UART_Stop_DMA(void){}

void UART_Send_Local_State(const void* s){}

void UART_Send_GlobalState(const void* gs){}

void UART_Send_Decision(const void* dec){}

void UART_Send_Ping(void){}

void UART_Wait_State(void* s){}

void UART_Wait_GlobalState(void* gs){}

void UART_Wait_Decision(void* dec){}

void UART_Wait_Ping(void* ping){}

boolean_T CRC_Check_State(const void* s_packet){}

boolean_T CRC_Check_GlobalState(const void* gs_packet){}

boolean_T CRC_Check_Decision(const void* dec_packet){}

boolean_T UART_Check_Ping(uint8_t rec_ping){}

void IO_Set_Session(void){}

void IO_Reset_Session(void){}

void IO_Set_MasterTalk(void){}

void IO_Reset_MasterTalk(void){}

boolean_T IO_Read_SlaveTalk(void){}

uint32_t Time_Get_Tick(void){}

boolean_T Time_Check_Elapsed_us(uint32_T start_time, uint32_T min_elapsed_time){}

boolean_T Time_Check_Elapsed_ms(uint32_T start_time, uint32_T min_elapsed_time){}

void OS_Enter_Critical(void){}

void OS_Exit_Critical(void){}
