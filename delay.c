#include "delay.h"

#define GetInstructionClock() (32000000U)

void delay_10us(int  us)
{
	volatile unsigned long _dcnt;
  

	_dcnt = us *((unsigned long)(0.00001/(1.0/GetInstructionClock())/50));
    //    _dcnt = us  *   ((unsigned long) 12 *(0.00001/( 1.0/GetInstructionClock())));
	//while(_dcnt--){}
  
    while (--_dcnt)
    {
      
    }  
	
	
}

//*****************************************************************
void delay_ms(int ms)
{
    unsigned char i;
    while(ms--)
    {
        i=4;
        while(i--)
        {
            delay_10us(25);
        }
    }
}
//******************************************************************
void delay_us(int us)
{
  int i;
  while(--us < 0)
    for(i=0;i<32;i++)
      asm("nop");
  
  
  
} 