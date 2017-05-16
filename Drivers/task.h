#include<LPC17xx.h>
unsigned char a;
void tx(unsigned char b);
void idat(unsigned char val);
void dta(unsigned char date,unsigned char cmd);
 void delay(unsigned int y)
 {
 unsigned char i,j;
 for(i=0;i<y;i++)
 {
 for(j=0;j<100;j++);
 }
 }


void stop()
  {
  LPC_I2C1->I2CONSET=(1<<4);
  LPC_I2C1->I2CONCLR=0xff;
  tx('5');
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
void uin()
{
LPC_PINCON->PINSEL0=(1<<4)|(1<<6);
LPC_UART0->LCR=(1<<7)|(3<<0);
LPC_UART0->DLL=97;								
LPC_UART0->DLM=0;
LPC_UART0->LCR&=~(1<<7);
 }
void tx(unsigned char b)
{
	while((LPC_UART0->LSR&(1<<5))!=(1<<5));
	LPC_UART0->THR=b;
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
 tx('7');	
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
						 delay(1);
				idat(date|0x08);break;

		case 1:
					idat(date|0x0d);
					delay(1);
				idat(date|0x09);break;
		}
		}
void idat(unsigned char val)
{
 LPC_I2C1->I2DAT=val;
  while(LPC_I2C1->I2STAT!=0x28);  // data address
 LPC_I2C1->I2CONCLR=(1<<3);
 tx('3');
 
}
