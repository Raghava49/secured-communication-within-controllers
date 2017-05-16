#include "OS_Ex.h"
#include <uart.h>
#include <string.h>

extern uint8_t En_flag;

void uart_init(uint16_t baud);
void uart_TxChar(char ch);
char uart_RxChar(void);
void uart_TxStr(char *ch);
char* uart_RxStr(void);






void uart_TxChar(char ch)
{
    while(!(LPC_UART3->LSR & (1<<5)));
    LPC_UART3->THR=ch;
}

void uart_TxStr(char *ch)
{
	uint8_t i;
	for(i=0;ch[i] != '\0'; i++) 
		uart_TxChar(ch[i]);
}

char uart_RxChar()
{		
		char ch;
    while(!(LPC_UART3->LSR & 1));
    ch = LPC_UART3->RBR;
		return ch;
}

char* uart_RxStr()
{
	static char ch[20];
	uint8_t  j=0;
	do {
		ch[j] =	uart_RxChar();
	} while(ch[j++] != '\n');
	return ch;
}








void echo(char *data){
	UART_TxString(0, data);
	uart_TxStr(data);
	
}	

void check(char *data){
	UART_TxString(0, "Under development\r\n");
}

void Encrypt(char *data){
	UART_TxString(0, "Under development\r\n");
	if(strcmp(data, "start") == 0){
		UART_TxString(0, "Encryption Started\r\n");
		En_flag = 1;
	}
	else if(strcmp(data, "stop") == 0){
		UART_TxString(0, "Encryption Stoped\r\n");
		En_flag = 0;
	}
	else {
		UART_TxString(0, "Invalid argument\r\n");
		UART_TxString(0, data);
	}
	
}

void help(void){
	UART_TxString(0, "Under development\r\n");
	UART_TxString(0, "1.echo  2.check  3.encrypt\r\n");
}

int Extract_Cmd(char *a){
  char i, j=0, k, cmd[10] = "\0\n", data[40] = "\0\n";
	j=0;
	for(i = 0; a[i] != 32 && a[i] != '\n'; i++){
		cmd[i] = a[i];
	}
	k = i;
	for( ; a[i] != '\0'; i++){
		if(k < i)
			data[j++] = a[i];
	}
	data[--j] = '\r';
	data[++j] = '\n';
	data[++j] = '\0';
	
	if(strcmp(cmd, "send_s") == 0){
		
	}
	else if(strcmp(cmd, "echo") == 0){
		echo(data);
	}
	else if(strcmp(cmd, "check") == 0){
		check(data);
	}
	else if(strcmp(cmd, "encrypt")==0){
		Encrypt(data);
	}
	else if(strcmp(cmd, "help")==0){
		help();
	}
	else {
		UART_TxString(0, "Invalid Command\r\n");
	}
	for(j = 0 ; j < 39; j++){
		a[j] = '\0';
	}
	j = 0;
	
	return 1;
}
