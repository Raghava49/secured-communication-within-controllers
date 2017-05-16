#include <stdlib.h>
#include "sys_uart.h"
#include "uart.h"
#include "sys_uart.h"
#include "delay.h"


extern uint8_t En_flag;
char dat[5];
void UART_TxString(uint8_t v_uartChannel_u8, char *ptr) {
	uint8_t i, key1 = 0, key2 = 0, En_data =0, count = 0;
	char Tx_string[9] = "\r\n";
	srand(9);
	if(En_flag == 1){
		for(i=0; ptr[i] != '\0'; i++) {
			do
			{
				En_data = key2 = key1 = 0;
				En_data  = ptr[i];
				key1     = 2;
				key2     = 4;
				En_data ^= key1;
				En_data ^= key2;
				key1    ^= key2;
				key2    ^= 0x2F;

				if ((key2 & (0x01 << 4)) == 0)
				{
						Tx_string[count++] = En_data;
						Tx_string[count++] = key1;
						Tx_string[count++] = key2;
				}
				else
				{
						Tx_string[count++] = key1;
						Tx_string[count++] = En_data;
						Tx_string[count++] = key2;
				}
				Tx_string[count++] = '\r';
				Tx_string[count++] = '\n';
				Tx_string[count++] = '\0';
				count = 0;
			}
			while ((Tx_string[0] == 13) || (Tx_string[1] == 13) || (Tx_string[2] == 13) ||
				(Tx_string[0] == 10) || (Tx_string[1] == 10) || (Tx_string[2] == 10));			
			
			sys_UART_TxString(v_uartChannel_u8, Tx_string);
			DELAY_ms(800);
			count = 0;
		}
	}
	else{
		sys_UART_TxString(v_uartChannel_u8, ptr);
	}
}


uint8_t UART_RxString(uint8_t var_uartChannel_u8, char *ptr_string){
	uint8_t Rx_key1 = 0, Rx_key2 = 0, Rx_data = 0, count = 0;
	if(En_flag == 1){
		while(Rx_data != 0x0D){
			sys_UART_RxString(var_uartChannel_u8, dat);
			Rx_key2 = dat[2];
			if ((Rx_key2 & (0x01 << 4)) == 0)
			{
					Rx_data = dat[0];
					Rx_key1 = dat[1];
			}
			else
			{
					Rx_data = dat[1];
					Rx_key1 = dat[0];
			}
			Rx_key2 ^= 0x2F;
			Rx_key1 ^= Rx_key2;
			Rx_data ^= Rx_key2;
			Rx_data ^= Rx_key1;
			ptr_string[count++] = Rx_data;
			//UART_TxChar(0, Rx_data);
		}
		return 0;
	}
	else{
		return sys_UART_RxString(var_uartChannel_u8, ptr_string);
	}
}
