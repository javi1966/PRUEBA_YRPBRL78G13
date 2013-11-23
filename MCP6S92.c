#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "MCP6S92.h"

volatile unsigned char  *gpCsi01RxAddress;   /* csi01 receive buffer address */
volatile unsigned short gCsi01RxLen;      /* csi01 receive data length */
volatile unsigned short gCsi01RxCnt;      /* csi01 receive data count */
volatile unsigned char  *gpCsi01TxAddress;   /* csi01 send buffer address */
volatile unsigned short gCsi01TxLen;      /* csi01 send data length */
volatile unsigned short gCsi01TxCnt;      /* csi01 send data count */

//**************************************************************************

void initMCP62s92()
{
  
   SAU0EN = 1U;   /* supply SAU0 clock */
   asm("nop");
   asm("nop");
   asm("nop");
   asm("nop");
   SPS0 = 0x0007;  //4Mhz
   
   ST0 |=0x0002;   /* disable CSI01 */
   CSIMK01 = 1U;   /* disable INTCSI01 interrupt */
   CSIIF01 = 0U;   /* clear INTCSI01 interrupt flag */
   /* Set INTCSI01 low priority */
   CSIPR101 = 1U;
   CSIPR001 = 1U;
   
   SIR01 = 0x0007;  //borra flags framminf,parity,overrun
   SMR01 = 0x0020;  //modo SPI 1
 //  SCR01 = 0x8087;  //TX,tipo 1,LSB first,8 bits
   SCR01 = 0x8007;  //TX,tipo 1,MSB first,8 bits
   SDR01 = 0x8000;
   SO0 |= 0x0200;   /* CSI01 clock initial level */
   SO0 &= ~0x0002;   /* CSI01 SO initial level */
   SOE0 |= 0x0002;   /* enable CSI01 output */
   /* Set SO01 pin P73 */
   P7 |= 0x08U;
   PM7 &= 0xF7U;
   /* Set SCK01 pin P75*/
   P7 |= 0x20U;
   PM7 &= 0xDFU;
   
   PM_CS_MCP6S22 = 0 ; //CS salida
   CS_MCP6S22 = 1;     //CS High
   CSIIF01 = 0U;   /* clear INTCSI01 interrupt flag */
   CSIMK01 = 0U;   /* enable INTCSI01 */
   SS0 |= 0x0002;  //start SAU01
  
} 

//*************************************************************************
int MCP6s92Send(unsigned char *txbuf, unsigned short txnum)
{
   int status = 0;

   if (txnum < 1U)
   {
      status = 1;
   }
   else
   {
      CS_MCP6S22 = 0;
      gCsi01TxCnt = txnum;   /* send data count */
      gpCsi01TxAddress = txbuf;   /* send buffer pointer */
      CSIMK01 = 1U;   /* disable INTCSI01 interrupt */
      SIO01 = *gpCsi01TxAddress;   /* started by writing data to SDR[7:0] */
      gpCsi01TxAddress++;
      gCsi01TxCnt--;
      CSIMK01 = 0U;   /* enable INTCSI01 interrupt */
   }

   return (status);
}

//***************************************************************************
 void setChanMCP6s92(unsigned char chan)
  {
    unsigned char buffer[2];
    buffer[0]=0x41;
    buffer[1]=chan;
    
     CS_MCP6S22 = 0;
     CSIMK10 = 1U;
     SIO01 = 0x41;
     
  //   MCP6s92Send((unsigned char*)&buffer[0], 2);
   //  while(SSR01 & 0x0040);   //_SAU_UNDER_EXECUTE
     
     SIO01 = chan;
     while(SSR01 & 0x0040);   //_SAU_UNDER_EXECUTE
     
     CS_MCP6S22 = 1;
    
  }

//***************************************************************************

  int setGainMCP6S92(unsigned char gain)
  {
    int code = 0;
    unsigned char buffer[2];
    switch (gain)
    {
      
      case 1:  code = 0;  break;
      case 2:  code = 1;  break;
      case 4:  code = 2;  break;
      case 5:  code = 3;  break;
      case 8:  code = 4;  break;
      case 10: code = 5;  break;
      case 16: code = 6;  break;
      case 32: code = 7;  break;
      default: return -1;
    }
    
    buffer[0]=0x40;
    buffer[1]=code;
    
     CS_MCP6S22 = 0;
     CSIMK10 = 1U;
     SIO01 = 0x40;
     
  //   MCP6s92Send((unsigned char*)&buffer[0], 2);
    // while(SSR01 & 0x0040);   //_SAU_UNDER_EXECUTE
     
     SIO01 = code;
     while(SSR01 & 0x0040);   //_SAU_UNDER_EXECUTE
     
     CS_MCP6S22 = 1;
    //MCP6s92Send((unsigned char*)&buffer[0], 2);
    return  0;
  }
//**********************************************************************
#pragma vector = INTCSI01_vect
__interrupt void MD_INTCSI01(void)
{
   unsigned char err_type;

   err_type = (unsigned char)(SSR01 & 0x0001);
   SIR01 = (unsigned short)err_type;
   if (err_type != 1U)
   {
      if (gCsi01TxCnt > 0U)
      {
         SIO01 = *gpCsi01TxAddress;
         gpCsi01TxAddress++;
         gCsi01TxCnt--;
      }
      else
      {
        CS_MCP6S22 = 1;//CSI01_SendEndCallback();   /* complete send */
      }
   }
}