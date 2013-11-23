#include "ior5f100le.h"
#include "ior5f100le_ext.h"

#include "RTC.h"

 struct RTCCounterValue *stRTC_Data;


void initRTC()
{
  
    RTCEN = 1U;     /* supply RTC clock */
    RTCE = 0U;      /* disable RTC clock operation */
    RTCMK = 1U;     /* disable INTRTC interrupt */
    RTCIF = 0U;     /* clear INTRTC interrupt flag */
    /* Set INTRTC level 1 priority */
    RTCPR1 = 0U;
    RTCPR0 = 1U;
    RTCC0 = 0x0A;  //24h ,cada segundo
    RTCMK = 1U;    //enable operacion
    RTCE = 1U;
    
   
  
}  

//********************************************************************************************
#pragma vector = INTRTC_vect
__interrupt void R_RTC_Interrupt(void)
{
    
    unsigned long i;
    
    RTCC1 |= 0x01;  //Pausa
    /* Wait 10 us */
    for( i=0U; i<RTC_WAITTIME; i++ )
    {
        asm("nop");
    }
    if (RWST == 0U)
    {
        //status = MD_BUSY1;
      ;
    }
    else
    {
        stRTC_Data->Sec = SEC;
        stRTC_Data->Min = MIN;
        stRTC_Data->Hour = HOUR;
        stRTC_Data->Week = WEEK;
        stRTC_Data->Day = DAY;
        stRTC_Data->Month = MONTH;
        stRTC_Data->Year = YEAR;
        RTCC1 &= (unsigned char)~0x01;  //counter reanuda
        /* Wait 10 us */
        for( i=0U; i<RTC_WAITTIME; i++ )
        {
           asm("nop");
        }
        if (RWST == 1U)
        {
            ;
        }
    }
    
    
    
}

//************************************************************************************************
unsigned char bin2bcd(unsigned char bin)
{
unsigned char bcd;
   
    bcd=(bin/10) << 4;
    bcd+=(bin%10);
    return (bcd);
}
//***********************************************************************************************
unsigned char bcd2bin(unsigned char BCD)
{
  unsigned char numero;
  
  numero=(BCD >> 4) * 10;
  numero+=(BCD & 0x0F);

  return numero;
}
//******************************************************************************************
void ponHora(const unsigned char hora,const unsigned char min)
{
  
  int i;
  
  RTCC1 |=0x01;   //PAUSE
  
    for( i=0U; i<RTC_WAITTIME; i++ )
    {
       asm("nop");
    }
  
  HOUR=bin2bcd(hora);
  MIN=bin2bcd(min);
  SEC=0;
  
  RTCC1 &= (unsigned char)~0x01;  //reanuda RTC
  
    for( i=0U; i<RTC_WAITTIME; i++ )
    {
       asm("nop");
    }
}
