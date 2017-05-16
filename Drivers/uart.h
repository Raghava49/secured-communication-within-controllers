#ifndef _UART_H_
#define _UART_H_

#include <lpc17xx.h>
#include "stdutils.h"
#include <stdlib.h>


void UART_TxString(uint8_t var_uartChannel_u8, char *ptr_stringPointer_u8);
uint8_t UART_RxString(uint8_t var_uartChannel_u8, char *ptr_string);


#define C_UartZero_U8    0u
#define C_UartOne_U8     1u
#define C_UartTwo_U8     2u
#define C_UartThree_U8   3u

#define UART0_TxString(ptr) UART_TxString(C_UartZero_U8,  ptr)
#define UART1_TxString(ptr) UART_TxString(C_UartOne_U8,   ptr)
#define UART2_TxString(ptr) UART_TxString(C_UartTwo_U8,   ptr)
#define UART3_TxString(ptr) UART_TxString(C_UartThree_U8, ptr)


#define UART0_RxString(ptr) UART_RxString(C_UartZero_U8,  ptr)
#define UART1_RxString(ptr) UART_RxString(C_UartOne_U8,   ptr)
#define UART2_RxString(ptr) UART_RxString(C_UartTwo_U8,   ptr)
#define UART3_RxString(ptr) UART_RxString(C_UartThree_U8, ptr)

#define UART0_Printf(var_arg_list...) UART_Printf(C_UartZero_U8,  var_arg_list)
#define UART1_Printf(var_arg_list...) UART_Printf(C_UartOne_U8,   var_arg_list)
#define UART2_Printf(var_arg_list...) UART_Printf(C_UartTwo_U8,   var_arg_list)
#define UART3_Printf(var_arg_list...) UART_Printf(C_UartThree_U8, var_arg_list)


#endif



