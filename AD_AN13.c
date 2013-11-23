#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "AD_AN13.h"

extern unsigned int ADC_value;
extern unsigned char ADC_done;

void initADI3()
{
   ADCEN = 1U;    /* supply AD clock */
   ADM0 = 0x00;   /* disable AD conversion and clear ADM0 register */
   ADMK = 1U;     /* disable INTAD interrupt */
   ADIF = 0U;     /* clear INTAD interrupt flag */
   /* Set INTAD low priority */
   ADPR1 = 1U;
   ADPR0 = 0U;
   ADPC = 0x05;  //ANI0..3
                     /* Set ANI0 - ANI3 pin as analog input */
   PM2 |= 0x0FU;
   ADM0 = 0x00;
   ADM1 = 0x00;    //Secuencial
   ADM2 = 0x00;   //10 bits ,Vref->Vdd,Vss
   ADUL = 0xFF;
   ADLL = 0x00;
   ADS  = 0x03;  //ANI3,P23
   ADIF = 0U;   /* clear INTAD interrupt flag */
   ADMK = 0U;   /* enable INTAD interrupt */
   ADCS = 1U;   /* enable AD conversion */ 
   ADCE = 1;    //Enable comparator
} 

//***************************************************************************
void AD_ANI3read(unsigned short *buffer)
{
   *buffer = (unsigned short)(ADCR >> 6U);
}
//*****************************************************************************
#pragma vector = INTAD_vect
__interrupt void int_ANI3(void)
{
   /* Start user code. Do not edit comment generated here */
        ADC_value = ADCR;               /* Read ADC result (10bits) */
        ADC_value = ADC_value >> 5;     /* Scale ADC value. Range is 0 -> 2048 */
        ADC_done  =1;             /* Set ADC result updated flag */
        ADIF = 0;
   /* End user code. Do not edit comment generated here */
}