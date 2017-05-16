#include <RTL.h>
#include <LPC17xx.h>
#include "sys_uart.h"
#include "uart.h"
#include "uart3.h"
#include "OS_Ex.h"
#include <stdlib.h>
#include <stdio.h>



char Rx_Cmd[50];
char Rx_flag;
uint8_t En_flag = 1;
uint8_t len =0;

OS_TID tsk1;
OS_TID tsk2;
OS_TID tsk3;


__task void job1 (void);
__task void job2 (void);
__task void job3 (void);


void uart_init(uint16_t baud){
	uint32_t var_UartPclk_u32, var_Pclk_u32,var_RegValue_u32;
	LPC_SC->PCONP |= (1<<25);
	LPC_PINCON->PINSEL9|=(0xf<<24);
	LPC_SC->PCLKSEL1 |= (3<<16);
	LPC_UART3->LCR |= 3 | (1<<7);
	var_UartPclk_u32 = (LPC_SC->PCLKSEL1 >> 18) & 0x03;

	switch( var_UartPclk_u32 )
	{
	case 0x00:
			var_Pclk_u32 = SystemCoreClock/4;
			break;
	case 0x01:
			var_Pclk_u32 = SystemCoreClock;
			break; 
	case 0x02:
			var_Pclk_u32 = SystemCoreClock/2;
			break; 
	case 0x03:
			var_Pclk_u32 = SystemCoreClock/8;
			break;
	}
	
	var_RegValue_u32 = ( var_Pclk_u32 / (16 * baud )); 
	LPC_UART3->DLL =  var_RegValue_u32 & 0xFF;
	LPC_UART3->DLM = (var_RegValue_u32 >> 0x08) & 0xFF;
	LPC_UART3->FCR |= 7;
	LPC_UART3->LCR &= ~(1<<7);
	LPC_UART3->IER = 1;
}

__task void job1 (void) {
  os_tsk_prio_self (1);
  tsk1 = os_tsk_self ();
  tsk2 = os_tsk_create (job2,1);
	UART_Init(0, 9600);
	uart_init(9600);
	
	
	UART_TxString(0, "Type help for available commands\r\n");
	UART_TxString(0, "CMD_>");

  while (1) {
		if(Rx_flag == 0){
		len = UART_RxString(0, Rx_Cmd);
		Rx_flag = 1;
		UART_TxString(0, "\r\n");
		os_tsk_pass();
		}
		else {
			os_tsk_pass();
		}
	}
}

__task void job2 (void) {
  while (1) {		
		if(Rx_flag == 1){
			Extract_Cmd(Rx_Cmd);
			Rx_flag = 0;
			UART_TxString(0, "\r\nCMD_>");
			os_tsk_pass();
		}
		else{
			os_tsk_pass();
		}
  }
}

  int main (void) {
  os_sys_init (job1);
}

