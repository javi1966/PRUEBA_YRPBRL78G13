#include <string.h>
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "IR.h"


volatile unsigned long gTau0Ch3Width;
unsigned char rx_index = 0;         /* Index of bit to receive */
unsigned char rx_fronthalf = 1;     /* If high, process first 32 cycles. If low, second 32 cycles */
unsigned char rx_packet[14];
extern  unsigned char got_packet = 0;
unsigned int pulses_seen = 0;

 unsigned char packet[14];
 
 unsigned char it_timeouts;
 
long PulseWidth[70];		/* Store pulse width */
int irPtr=0;	

//******************************************************************************************
void startIR()
{
     memset(PulseWidth,0x00,sizeof(PulseWidth));
     irPtr=0;
     got_packet = 0;
  
}
//********************************************************************************************
void initIR()
{
        TAU0EN = 1U;      //supply reloj=
        TPS0   = 0x0075;
        TMR03 = 0x0204;  //modo captura,con flanco bajada
        //TMR03 = 0x12CC;
	/*TOM0 &= ~0x0008;  //TI03 input
	TOL0 &= ~0x0008;
	TO0 &= ~ 0x0008;
	TOE0 &= ~0x0008;*/
	NFEN1 |= 0x08 ;      //_TAU_CH3_NOISE_ON;// previene ruido canal 3
        PM3_bit.no1=1;                  //PM3 |= 0x02U; //P31 input
        TMPR103 = 1U;        //low prioridad
        TMPR003 = 1U;
        TMIF03 = 0U;	                // clear INTTM03 interrupt flag 
	TMMK03 = 0U;	                // enable INTTM03 interrupt 
	TS0 |= 0x0008;                  //TRIGGER ON.
         
        
        //inicia timeout 100ms
         /* Channel 4 used as interval timer */
         TMR04 = 0x0000;   //_0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE | _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
         TDR04 = 0xffff;   //_TAU_TDR04_VALUE;
         TOM0 &= ~0x0010;  // _TAU_CH4_OUTPUT_COMBIN;
         TOL0 &= ~0x0010;   //_TAU_CH4_OUTPUT_LEVEL_L;
         TO0 &= ~0x0010;    //_TAU_CH4_OUTPUT_VALUE_1;
         TOE0 &= ~0x0010;   //_TAU_CH4_OUTPUT_ENABLE;
         TMPR104 = 1U;      //low priority
         TMPR004 = 1U;
         TMIF04 = 0U;       // clear INTTM04 interrupt flag 
         TMMK04 = 1U;      // disable INTTM04 interrupt 
        // TS0 |=0x0010;     //_TAU_CH4_START_TRG_ON;
        /*
        //inicia Ttimer interval para Timeout
        ITMC = 0x0000;	// disable ITMC operation 
	ITMK = 1U;	// disable INTIT interrupt 
	ITIF = 0U;	// clear INTIT interrupt flag 
	// Set INTIT low priority 
	ITPR1 = 1U;
	ITPR0 = 1U;
	ITMC = 0x007;  //valor de IT  7 * 1/32768 Khz
        //Start
        ITIF = 0U;	// clear INTIT interrupt flag 
	ITMK = 1U;	// enable INTIT interrupt 
	ITMC |= 0x8000;	// enable IT operation 
        */
        startIR();
          
} 
//******************************************************************************************
unsigned char decodeIR()
{
  
  
  static unsigned char dato=0;
  unsigned char startPulse=0;
  int i;
  unsigned long lDatoNEC=0;
  
    TMR04start();
    
    while(!startPulse)
    {
      TMR04set(0);
      while( P3_bit.no1);
      if(TMR04get() > 8192 && TMR04get() < 10240) 
        startPulse=1;
        
      if(P3_bit.no1 == 0)  
            startPulse=0;
      
      TMR04set(0);
      while(P3_bit.no1 && TMR04get() < 10000);
      if(TMR04get() >3584 && TMR04get() < 5120) 
         startPulse=1;
    } 
             return 1;
      
      
} 

//******************************************************************************************

#pragma vector = INTTM03_vect
__interrupt void MD_INTTM03(void)
{

 
 
        /* Allow interrupts to nest inside this one - preserve 25% duty cycle output */
        // __enable_interrupt;
        
	if ((TSR03 & 0x0001) != 0U)	/* overflow occurs */
	{			
		gTau0Ch3Width = (unsigned long)TDR03 + 0x10000U;
	}
	else
	{
		gTau0Ch3Width = (unsigned long)TDR03;
	}
        
        PulseWidth[irPtr++] = gTau0Ch3Width;
        if (irPtr == 70 - 1) 
            irPtr = 0;  
       
       
       if ((PulseWidth[0] > 70000 && PulseWidth[0] < 130000) && 
           (PulseWidth[1] > 120 && PulseWidth[1] < 300))
       {
         
         
         got_packet = 1;
         
         
         
       } 
       
        //Start timeout
       // TMIF04 = 0U;   /* clear INTTM04 interrupt flag */
      //  TMMK04 = 0U;   /* enable INTTM04 interrupt */
       // TS0 |= 0x0010; //_TAU_CH4_START_TRG_ON;
        
        /*
        
          if (rx_fronthalf) {
            
            // If we've seen many pulses when we're expecting the first 32 cycles 
            if (pulses_seen > 28) {
                
                if ((pulses_seen > 56) || (rx_packet[rx_index - 1] == 0)) {
                    pulses_seen = 0;
                    
                    //The current bit is zero if we have many pulses back to back (1 to 0 transition)
                     // or if we have a handful of pulses and the previous bit was zero (0 to 0 transition)
                     
                    rx_packet[rx_index] = 0;
                    
                    // Start looking for the next 32 cycles in this bit 
                    rx_fronthalf = 0;
                }
            }
        
        // Are we receiving the second 32 cycles of an incoming pulse train? 
        } else {
            if (rx_packet[rx_index] == 1) {
                if (pulses_seen > 28) {
                    rx_index++;
                    
                    // If the current bit was detected as a 1 already, then we can start looking for
                   // the front half of the next bit after we receive a bunch of high cycles
                    
                    rx_fronthalf = 1;
                    
                    // If we completed reception of the last bit, reset the bit counter for the 
                    // rx_packet and prepare to display the command byte on screen
                  
                    if (rx_index > 13) {
                        rx_index = 0;
                        got_packet = 1;
                    }
                }
            }
        }
        
        // Count pulses seen by this interrupt 
        if (pulses_seen < 100)
            pulses_seen++;
        
        // Reset the interval timeout counter 
        it_timeouts = 0;
        */
        // Restart the interval timer 
        //TT0 |= 0x0008;
        
        
	// Mask channel 3 interrupt 
	//TMMK03 = 1U;	// disable INTTM03 interrupt 
	//TMIF03 = 0U;	// clear INTTM03 interrupt flag 
       // TMIF03 = 0U;	// clear INTTM03 interrupt flag 
	//TMMK03 = 0U;	// enable INTTM03 interrupt 
	//TS0 |= 0x0008;
        
       
        
} //fin INT   

//*****************************************************************************
#pragma vector = INTTM04_vect
__interrupt void MD_INTTM04(void){
  
  
        irPtr=0;
       
        //stop timeout
        TT0 |= 0x0010;   //_TAU_CH4_STOP_TRG_ON;
        TMMK04 = 1U;   /* disable INTTM04 interrupt */
        TMIF04 = 0U;   /* clear INTTM04 interrupt flag */
  
  
}
//****************************************************
void TMR04start(void)
{
   TMIF04 = 0U;   /* clear INTTM04 interrupt flag */
   TMMK04 = 1U;   /* enable INTTM04 interrupt */
   TS0 |= 0x0010;
}
//****************************************************
void TMR04stop(void)
{
   TT0 |= 0x0010;
   /* Mask channel 4 interrupt */
   TMMK04 = 1U;   /* disable INTTM04 interrupt */
   TMIF04 = 0U;   /* clear INTTM04 interrupt flag */
}
//*********************************************************
void TMR04set(unsigned int regvalue)
{
   TDR04 = regvalue;
}
//****************************************************
int TMR04get()
{
  return TDR04;
}  

//*********************************************************************************************
/*
#pragma vector = INTIT_vect
__interrupt void MD_INTIT(void)
{
	// Start user code. Do not edit comment generated here 
    // Allow interrupts to nest inside this one 
   //__enable_interrupt;
    
   // Reset to look for the first bit if we time out too much while looking for the back half 
    if (it_timeouts > 8) {
        rx_fronthalf = 1;
        rx_index = 0;
    }
    
    //If we're looking for the front half of the bit and we time out, we get a 1 if we time out more than
   // once (first 32 cycles are low) or if the previous bit is 1 and we only get one timeout.
    
    if (rx_fronthalf) {
        if ((it_timeouts > 1) || (rx_packet[rx_index-1] == 1)) {
            rx_packet[rx_index] = 1;
            rx_fronthalf = 0;
        }
    
    // If we're looking for the back half of the current bit and we get a timeout, 
    // we confirm that the current bit is a zero (because the first 32 cycles were high).
    // Increment the index and if we've filled the buffer, prepare to disply it on screen.
    
    } else {
        if (rx_packet[rx_index] == 0) {
            rx_index++;
            rx_fronthalf = 1;
            if (rx_index > 13) {
                rx_index = 0;
                got_packet = 1;
            }
        }
    }
    
    // Count the number of consecutive timeouts not interrupted by an IR pulse 
    if (it_timeouts < 100)
        it_timeouts++;
    
   // Set the consecutive IR pulses seen to zero 
    pulses_seen = 0;
    
    // Start the interval timer over 
        ITIF = 0U;	// clear INTIT interrupt flag 
	ITMK = 0U;	// enable INTIT interrupt 
	ITMC |= 0x8000;	// enable IT operation 

	// End user code. Do not edit comment generated here
}
*/