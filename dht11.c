
#include "dht11.h"
#include "delay.h" 
 
 unsigned short TOUT = 0;
 volatile long widthPulso;
 long PulseWidth[45];		/* Store pulse width */
 int g_Times = 0;						/* Measurement times counter */
 unsigned char temperatura = 0;
 unsigned char humedad     = 0;
 extern unsigned char bCaptura =0;
 unsigned char estado=START;
 unsigned char dato[5];
 unsigned char cntBits =7;
 unsigned char idx =0;
 unsigned char i =0;
//****************************************************************************************
void initDHT11()
{
  /* PM14_bit.no6 = 1;   //entrada
   P14_bit.no6 = 1;
    //init TMR0
   TAU0EN = 1U;		// supplies input clock 
   TPS0 =0x0000; // _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;
      // Stop all channels 
   TT0 = 0x0AFF;//_0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON | _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON | _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0800_TAU_CH3_H8_STOP_TRG_ON;
   TMR00 = 0x0000; //_0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE | _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
   TDR00 = 0x0C7F; // _0C7F_TAU_TDR00_VALUE;
   TO0 &= ~0x0001; //_0001_TAU_CH0_OUTPUT_VALUE_1;
   TOE0 &= ~0x0001; //_0001_TAU_CH0_OUTPUT_ENABLE;
   TMIF00 = 0U;	    // clear INTTM00 interrupt flag 
   TMMK00 = 0U;	    // enable INTTM00 interrupt 
  // TS0 |= 0x0001;  //_0001_TAU_CH0_START_TRG_ON;
 //  TT0 |= 0x0001; //_0001_TAU_CH0_STOP_TRG_ON;*/
  
  TAU0EN = 1U;		// supplies input clock 
  TPS0 = 0x0002;
  TT0 = 0x0A00;
  TMMK01 = 1U;	// disable INTTM00 interrupt 
  TMIF01 = 0U;	// clear INTTM00 interrupt flag 
  TMPR101 = 1U;
  TMPR001 = 1U;
  
  TMR01 = 0x0104;//0x0144;
  TOM0 &= ~0x0002;
  TOL0 &= ~0x0002;
  
  TO0  &= ~0x0002;
  TOE0 &= ~0x0002;
  NFEN1 &= (unsigned char)~0x02;
  PM1 |= 0x40U;
 } 

//****************************************************************************************
unsigned short CheckResponse()
{
  //START SIGNAL
  
  PM14_bit.no6  = 0;     // Data port is output
  P14_bit.no6   = 0;
  delay_ms(25);    // Low for at least 18us
  P14_bit.no6      = 1;
  delay_us(30);    // High for 20-40 us
  PM14_bit.no6 = 1;     // Data port is input
  
  TOUT = 0;
  //TDR00 =0x0000;
  TS0 |= 0x0001;  //_0001_TAU_CH0_START_TRG_ON;
  while(!P14_bit.no6 && !TOUT); // If there's no response within 256us, the Timer2 overflows
  if (TOUT) return 0;    // and exit
  else {
   //TDR00 =0x0000;
   while(P14_bit.no6 && !TOUT);
   if (TOUT) return 0;
   else {
    TT0 |= 0x0001;
    return 1;
   }
  }
}
//***************************************************************************************
unsigned short ReadByte()
{
  unsigned short num = 0, t;
  int i;
  
  PM14_bit.no6 = 1;
  for (i=0; i<8; i++){
   while(!P14_bit.no6);
  //TCR00 = 0;
   TS0 |= 0x0001;  // Start TMR2 from 0 when a low to high data pulse
   while(P14_bit.no6);       // is detected, and wait until it falls low again.
   TT0 |= 0x0001;  // Stop the TMR2 when the data pulse falls low.
   if(TCR00 > 40) num |= 1<<(7-i);  // If time > 40us, Data is 1
  }
  return num;
}
//*****************************************************************

//****************************************************************************************
unsigned char readTemperatura(void)
{
  
    return temperatura;
  
} 

//***************************************************************************************
unsigned char readHumedad(void)
{
  
  return  humedad;
  
} 
//******************************************************************
unsigned char getCaptura()
{
  
    return bCaptura;
  
}
//*****************************************************************************************
unsigned char setCaptura(unsigned char cp)
{
  
  bCaptura = cp;
} 
//*****************************************************************************************
void startDHT11()
{
  
  
        //START SIGNAL
       
        PM1_bit.no6  = 0;     // Data port is output
        P1_bit.no6  = 0;
        delay_ms(25);    // Low for at least 18us
        P1_bit.no6      = 1;
        delay_us(30);    // High for 20-40 us
        PM1 |= 0x40U;     // Data port is input
        TMIF01 = 0U;	// clear INTTM01 interrupt flag 
	TMMK01 = 0U;	// enable INTTM01 interrupt 
	TS0 |= 0x0002;
        
       
        //PM0 |= 0x01U;
}
//*****************************************************************************************
void stopDHT11(){
  
        TT0 |= 0x0002;
	// Mask channel 0 interrupt 
	TMMK01 = 1U;	// disable INTTM00 interrupt 
	TMIF01 = 0U;	// clear INTTM00 interrupt flag 
        PM1 |= 0x40U;
  
}

//****************************************************************************************

#pragma vector = INTTM01_vect
__interrupt void MD_INTTM01(void)
{
	
        if ((TSR01 & 0x0001) != 0U)	/* overflow occurs */
	{			
		widthPulso = (long)TDR01 + 0x10000U;
	}
	else
	{
		widthPulso = (long)TDR01;
	}
        
       
        
        
   /*    
  
  switch (estado) {
    
  case START:
               if(widthPulso >  MIN_PULSE_START && widthPulso < MAX_PULSE_START)
                    estado = DATO;
               break;
  case DATO:               
               if(widthPulso >  MIN_PULSE_CERO && widthPulso < MAX_PULSE_CERO )
               {
                 
                 estado = DATO;
               }  
                 
               else if(  widthPulso >  MIN_PULSE_UNO && widthPulso < MAX_PULSE_UNO)
               {
                 dato[idx] |= (1 << cntBits);
                 estado = DATO;
                 
               }
               else
                 
                   estado = START;
               
               if(cntBits == 0)
               {
                 cntBits = 7;
                 idx++;
                 
               } 
               else
                  --cntBits;
               
               if(++i > 40)
               {  
                  estado=START;
                  break;
               }  
                  
               break;
  case FIN:
               estado = START;
               bCaptura = 1;  
               stopDHT11();	
               break;
  default:
               break;
  }           
       
        
       
        */  
  
	
	PulseWidth[g_Times++] = (widthPulso+1);	

	if (g_Times > 45)	
	{
          g_Times=0;
          humedad =0;
          temperatura =0;
            for ( i=2; i <= 9 ;i++)
                {  
                  
                  if(PulseWidth[i] >MIN_PULSE_UNO && PulseWidth[i] < MAX_PULSE_UNO)
                  { 
                     
                      humedad |= 1 <<(9-i);
                  }    
                } 
            for ( i=18; i <= 25 ;i++)
                {  
                  if(PulseWidth[i] > MIN_PULSE_UNO && PulseWidth[i] < MAX_PULSE_UNO)
                  { 
                     
                      temperatura |= 1 <<(25-i);
                  }    
                } 
               
            
          bCaptura = 1;  
	  stopDHT11();		//Stops timer counter 
	}
	
}

