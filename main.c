 #include <stdio.h>
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "delay.h"
#include "lcd.h"
#include "RTC.h"
//#include "MCP6S92.h"
//#include "AD_AN13.h"
//#include "BT_JY104.h"
//#include "dht11.h"

// ****** defines's ********************

#define DI     __disable_interrupt
#define EI     __enable_interrupt
#define HALT    __halt
#define NOP     __no_operation
#define STOP    __stop



//*********** pragma's *****************

/* Start user code for global. Do not edit comment generated here */
#pragma location = "OPTBYTE"
__root const unsigned char opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const unsigned char opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const unsigned char opbyte2 = 0xE8U;
#pragma location = "OPTBYTE"
__root const unsigned char opbyte3 = 0x84U;

/* Set security ID */
#pragma location = "SECUID"
__root const unsigned char secuid[10] =
    {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

#define TRIGGER P4_bit.no1
#define NUM_SAMPLES  10

//variables globales
extern struct RTCCounterValue *stRTC_Data;
int bHora = 0;
unsigned int ADC_value=0;
unsigned char ADC_done=0;
volatile unsigned int interval;
unsigned int distance;

volatile __saddr struct
{
unsigned char display_update : 1;
unsigned char echo_received : 1;
unsigned char trig_mode : 1;
unsigned char aux : 1;
} bits;

 long media;
 int idx;
 char buffer[25];

//******************************************************************************
void initTAU()
{
  TAU0EN = 1; // habilita a TAU0
  TPS0 = 0x0005; //    CK0=1MHz, CK1=32MHz, CK2=16MHz e CK3=125kHz
                 //TICKS   1us,0.03125us,0.0625 us,8us
  TMR00 =0x0008; //TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_ONECOUNT;
  TDR00 =     9; // intervalo inicial de 10us
  TMR04 =0x02CC;  //TAU_CK0|TAU_EDGE_RISE_FALL|TAU_TRIG_BOTH_EDGE|TAU_MD_CAPTURE_LEVEL;
  TS0L  =0x10 | 0x01;  //TAU_CH4 | TAU_CH0; 
  TMMK00 = 0; // habilita a interrupção do canal 0 da TAU0
  TMMK04 = 0; // habilita a interrupção do canal 4 da TAU0
  
}
//*****************************************************************
void borraBuffer()
{
  int i;
  for(i=0;i<25;i++)
    buffer[i]='\0';
}
//*****************************************************************
void main (void)

{
   int  i=0;
  
     DI();            //fclk 32Mhz
     PIOR = 0x00U;
     CMC =  0x10U;
     MSTOP = 1U;
     MCM0 = 0U;
     XTSTOP = 0U;
     for( i=0U; i<=100; i++ )
	{
		NOP();
	}
     OSMC = 0x00U;
     CSS = 0U;
     HIOSTOP = 0U;
     EI();	
     
      // initLCD();
       //initIR();
      // initADI3();
       //initMCP62s92();
      // initDHT11();
      // initBT();
       initTAU();
       initRTC();
       initLCD();
      
    // Configura TIMER INTERVAL ******************************

        RTCEN = 1U;	/* supply RTC clock */
        ITMC =  0x0000U;   //cuenta 30,5176
        ITMK = 1U;	// disable INTIT interrupt 
	ITIF = 0U;	//clear INTIT interrupt flag 
	// Set INTIT low priority 
	ITPR1 = 1U;
	ITPR0 = 1U;
	ITMC = 0x0CCEU;            //Tosc 1/32768 30.5us * 3278(0xCCE) = 100ms
        //Start IT  cada 100ms
        ITIF = 0U;	          // clear INTIT interrupt flag 
	ITMK = 0U;	         // enable INTIT interrupt 
	ITMC |= 0x8000U;	  // enable IT operation 
        
        PM7_bit.no7=0;
        P7_bit.no7=1;
        PM4_bit.no1 = 0;   //salida TRIGGER
        PM4_bit.no2 = 1;  //Entrada ECHO
        
        //CONFIGURA Pir Sensor
        EGP0 = 0x08;  //INTP3,P30
        PIF3 = 0;
        PMK3 = 0;    //Habilita INTP3 
        
        TRIGGER = 1;
        bits.trig_mode = 0;
        TS0L = 0x01;
     
        ponHora(21,45);
        putsLCD("\fComenzando...");
        borraBuffer();
        __enable_interrupt();
       
        idx=0;
        media=0L;
        
        delay_ms(1000);
        clrLCD();
     
      while(1)
      {
        
        
            sprintf((char *)buffer,"   Hora:    %02x:%02x:%02x",HOUR,MIN,SEC);
                 
            lcd_gotoxy(0,1);
            putsLCD((const char *)buffer);           
            borraBuffer();
        
          
           if (bits.display_update)
           {
             
           // calcula a distância em cm
              media+=interval;
              if(++idx == NUM_SAMPLES)
              {  
                interval = media / NUM_SAMPLES +1 ;
                idx=0;
                media=0L;
              }  
           
               
               distance= ((long)interval*100)/58;
               sprintf((char *)buffer,"   Distancia: %u.%02u  ",distance/100,distance%100);
              
               lcd_gotoxy(1,0);
               putsLCD((const char *)buffer);
                
                if (distance<=40000)
                {
                  ;;
                }
                
                borraBuffer();
                
                bits.display_update = 0; // apaga o bit de atualização
             
             } 
     
            
            delay_ms(500);
        
      }  //fin WHILE
 
  
}  

//***********************************************

#pragma vector = INTIT_vect
__interrupt void MD_INTIT(void)
{
  static int value=0;
  
  
  if(++value > 10)  //100ms * 10 = 1 segundo
  {
   bHora = 1; 
  // P7_bit.no7 ^= 1; 
   value=0;
  
   
  }
 
   ITIF = 0U;	
}
//****************************************************************
#pragma vector = INTTM00_vect
__interrupt void trata_TAU0_canal0(void)
{
  if (bits.trig_mode)
  {
  TRIGGER = 1;
  TDR00 = 9; // intervalo de 10us para o disparo
  TS0L = 0x01;
  bits.echo_received = 0;
 } 
  else
   {
     TRIGGER = 0;
     TDR00 = 0xFFFF;
     TS0L = 0x01;
     if (!bits.echo_received) interval=0xFFFF;
   }
  bits.trig_mode = !bits.trig_mode;
}
//*****************************************************************
#pragma vector = INTTM04_vect
__interrupt void trata_TAU0_canal4(void)
{
 
  interval = TDR04; // salva o valor capturado
 
  bits.echo_received = 1; // sinaliza que recebeu um pulso de eco
 
   bits.aux = !bits.aux;
   if (bits.aux)
     bits.display_update = 1; // atualiza o display
  
  
}
//*****************************************************************
#pragma vector = INTP3_vect
__interrupt void trata_INTP3(void)
{
   P7_bit.no7 ^= 1;   // acende led
   PIF3 = 0;
}