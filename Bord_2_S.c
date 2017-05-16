#include<lpc17xx.h>
//#include "task.h"
#include "sys_uart.h"
#include "delay.h"

unsigned char a;
void idat(unsigned char val);
void dta(unsigned char date,unsigned char cmd);
unsigned char h[11]="LCD_INITED" ;
char s[14], sp[] = "          ";

 unsigned char i;
 void delay(unsigned int y)
 {
 unsigned char i,j;
 for(i=0;i<y;i++)
 {
 for(j=0;j<50;j++);
 }
 }


void stop()
  {
  LPC_I2C1->I2CONSET=(1<<4);
  LPC_I2C1->I2CONCLR=0xff;
  }

void ini()
{
LPC_PINCON->PINSEL1=(0xf<<6);
LPC_PINCON->PINMODE1=(2<<6)|(2<<8);	 
LPC_PINCON->PINMODE_OD0=(3<<19); 
LPC_SC->PCLKSEL1=(2<<6);								 //to select 30Mhz frequency
LPC_I2C1->I2SCLL=38;									    // load frequency value
LPC_I2C1->I2SCLH=37;
}

void start()
{
	LPC_I2C1->I2CONSET=(1<<6) | (1<<5);   //set en start				
	 while(LPC_I2C1->I2STAT !=(0X08)); 			   
	 LPC_I2C1->I2CONCLR=(1<<3) | (1<<5);    			
}

void martc(unsigned char dat,unsigned char cmd)
{
unsigned d,c;
start();
 LPC_I2C1->I2DAT=0x4e;
 while(LPC_I2C1->I2STAT!=0x18);  //slave address					
 LPC_I2C1->I2CONCLR=(1<<3);  	
 d=dat&0xf0;
 c=((dat&0x0f)<<4);
 dta(d,cmd);
 dta(c,cmd);
 stop();
}

void dta(unsigned char date,unsigned char cmd)
{
switch(cmd)
		{
		case 0:
			
					idat(date|0x0c);
					 idat(date|0x0c);
				
				idat(date|0x08);break;

		case 1:
					idat(date|0x0d);
					idat(date|0x0d);					
					
				idat(date|0x09);break;
		}
		}
void idat(unsigned char val)
{
 LPC_I2C1->I2DAT=val;
  while(LPC_I2C1->I2STAT!=0x28);  // data address
 LPC_I2C1->I2CONCLR=(1<<3);

 
}
int main()
{
SystemInit();
	UART_Init(0, 9600);

	
ini();

martc(0x28,0);
martc(0x0e,0);
 martc(0x01,0);
	martc(0x0C,0);
 martc(0x80,0);
for(i=0;h[i] != '\0';i++)
{
	DELAY_ms(70);
martc(h[i],1);
 }
DELAY_sec(2);
 
 while(1){
	 sys_UART_RxString(0, s);
	 martc(0xC0, 0);
	 for(i=0; s[i+1] != '\0'; i++){
	 DELAY_sec(2);
	 martc(s[i], 1);
	 }
	 for(i=0; i<10; i++){
	 DELAY_sec(2);
	 martc(sp[i], 1);
	 }
	 
	 for(i=0; i<14; i++)
		s[i] = '\0';
}

}