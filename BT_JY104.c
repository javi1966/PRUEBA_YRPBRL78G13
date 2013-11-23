/*  Funciones para manejo Modulo Bluetooth JY_MCU v1.04*/
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "BT_JY104.h"

// Variables GLOBALES
volatile unsigned char  *ptr_bufferTX;	/* uart2 transmit buffer address */
volatile unsigned short cnt_bufferTX;		/* uart2 transmit data number */
volatile unsigned char  *ptr_bufferRX;	/* uart2 receive buffer address */
volatile unsigned short cnt_bufferRX;		/* uart2 receive data number */
volatile unsigned short len_bufferRX;		/* uart2 receive data length */
volatile unsigned short gUart2RxErr;
unsigned char TxBuff[32]= {0};


//*****************************************************************************
void initBT()
{
  
        SAU1EN = 1U;	/* supply SAU1 clock */
	 asm("nop");
	 asm("nop");
	 asm("nop");
	 asm("nop");
	 SPS1 = 0x0044; //_0002_SAU_CK00_FCLK_2 | _0020_SAU_CK01_FCLK_2;
        
        //Inicia UART2 
        ST1 |= 0x0003; //_0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;	/* disable UART2 receive and transmit */
	STMK2 = 1U;	/* disable INTST2 interrupt */
	STIF2 = 0U;	/* clear INTST2 interrupt flag */
	SRMK2 = 1U;	/* disable INTSR2 interrupt */
	SRIF2 = 0U;	/* clear INTSR2 interrupt flag */
	SREMK2 = 1U;	/* disable INTSRE2 interrupt */
	SREIF2 = 0U;	/* clear INTSRE2 interrupt flag */
	/* Set INTST2 low priority */
	STPR12 = 1U;
	STPR02 = 1U;
	/* Set INTSR2 low priority */
	SRPR12 = 1U;
	SRPR02 = 1U;
	/* Set INTSRE2 low priority */
	SREPR12 = 1U;
	SREPR02 = 1U;
	SMR10 = 0x0022;  //_0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
	SCR10 = 0x8097;  //_8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
	SDR10 = 0xCE00U; //_CE00_UART2_TRANSMIT_DIVISOR;
	NFEN0 |= 0x10;   //_10_SAU_RXD2_FILTER_ON;
	SIR11 =  0x0007; // _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;	/* clear error flag */
	SMR11 = 0x0122; //_0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
	SCR11 = 0x4497; //_4000_SAU_RECEPTION | _0400_SAU_INTSRE_ENABLE | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
	SDR11 = 0xCE00U; // _CE00_UART2_RECEIVE_DIVISOR;
	SO1 |= 0x0001;  //_0001_SAU_CH0_DATA_OUTPUT_1;
	SOL1 |= 0x0000; //_0000_SAU_CHANNEL0_NORMAL;	/* output level normal */
	SOE1 |= 0x0001; //_0001_SAU_CH0_OUTPUT_ENABLE;	/* enable UART2 output */
	/* Set RxD2 pin */
	PM1 |= 0x10U;
	/* Set TxD2 pin */
	P1 |= 0x08U;
	PM1 &= 0xF7U;
  
  
  
}
//*******************************************************************************
void startBT()
{
        STIF2 = 0U;	/* clear INTST2 interrupt flag */
	STMK2 = 0U;	/* enable INTST2 interrupt */
	SRIF2 = 0U;	/* clear INTSR2 interrupt flag */
	SRMK2 = 0U;	/* enable INTSR2 interrupt */
	SREIF2 = 0U;	/* clear INTSRE2 interrupt flag */
	SREMK2 = 0U;	/* enable INTSRE2 interrupt */
	SO1 |= 0x0001;  //_0001_SAU_CH0_DATA_OUTPUT_1;	/* output level normal */
	SOE1 |= 0x0001; //_0001_SAU_CH0_OUTPUT_ENABLE;	/* enable UART2 output */
	SS1 |= 0x0003;  //_0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;	/* enable UART2 receive and transmit */
  
  
}

//******************************************************************************
void stopBT()
{

  
        ST1 |= 0x0003; //_0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;	/* disable UART2 receive and transmit */
	SOE1 &= ~0x0001; //_0001_SAU_CH0_OUTPUT_ENABLE;	/* disable UART2 output */
	STMK2 = 1U;	/* disable INTST2 interrupt */
	STIF2 = 0U;	/* clear INTST2 interrupt flag */
	SRMK2 = 1U;	/* disable INTSR2 interrupt */
	SRIF2 = 0U;	/* clear INTSR2 interrupt flag */
	SREMK2 = 1U;	/* disable INTSRE2 interrupt */
	SREIF2 = 0U;	/* clear INTSRE2 interrupt flag */
    
}
//*********************************************************************************
int BT_ReceiveData(unsigned char *rxbuf, unsigned short rxnum)
{
   int status = 1;

	if (rxnum < 1U)
	{
		status = 0;
	}
	else
	{
		cnt_bufferRX = 0U;
		len_bufferRX = rxnum;
		ptr_bufferRX = rxbuf;
	}

	return (status);
  
  
}
//************************************************************************************
int BT_SendData(unsigned char* txbuf, unsigned short txnum)
{
   int status = 1;

	       
        
        if (txnum < 1U)
	{
		status = 0;
	}
	else
	{
                int i;
		/*ptr_bufferTX = txbuf;
		cnt_bufferTX = txnum;
		STMK2 = 1U;	// disable INTST0 interrupt 
		TXD2 = *ptr_bufferTX++;
		//ptr_bufferTX++;
		cnt_bufferTX--;
		STMK2 = 0U;	// enable INTST0 interrupt 
                */
                ptr_bufferTX = txbuf;
                cnt_bufferTX = txnum;
                while (cnt_bufferTX)
                {
		  TXD2 = *ptr_bufferTX;
                  for (i=0; i < 3000; i++) {
                   asm("nop");
                  }
		  ptr_bufferTX++;
		  cnt_bufferTX--;
                }
                
                
                
	}

	return (status);
}
//****************************************************************************************
#pragma vector = INTSR2_vect
__interrupt void MD_INTSR2(void)
{
  /*
     //   RxBufferPtr->wrPtr++;
       unsigned char rx_data;

	rx_data = RXD2;
	if (len_bufferRX > cnt_bufferRX)
	{
		*ptr_bufferRX = rx_data;
		ptr_bufferRX++;
		cnt_bufferRX++;
		if (len_bufferRX == cnt_bufferRX)
		{
			gUart2RxErr = 0U;
		}
	}
        
        */
  
  RxBufferPtr->buffer[(RxBufferPtr->wrPtr) & (RX_BUFF_SIZE-1)] = RXD2;    // Store received data in Rx buffer
  RxBufferPtr->wrPtr++;
  
  
  
}
//******************************************************************************************
#pragma vector = INTST2_vect
__interrupt void MD_INTST2(void)
{
  /*
        if (cnt_bufferTX > 0U)
	{
		TXD2 = *ptr_bufferTX++;
		//ptr_bufferTX++;
		cnt_bufferTX--;
	}
	else
	{
		gUart2RxErr = 0U;
	}
  
  */
}
//******************************************************************************************
#pragma vector = INTSRE2_vect
__interrupt void MD_INTSRE2(void)
{
	unsigned char err_type;

        RxBufferPtr->buffer[(RxBufferPtr->rdPtr) & (RX_BUFF_SIZE-1)] = RXD2;
	err_type = (unsigned char)(SSR11 & 0x0007U);
        
        
        
	SIR11 = (unsigned short)err_type;
        if (err_type >= 4)
        {
         ST1 |= 0x0003; //_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;	// Disable UART2 receive and transmit
         SS1 |= 0x0003; //_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;	// Enable UART2 receive and transmit
        }
	//UART2_ErrorCallback(err_type);
        
        UART2_InitTxBuffers();
}
//************************************************************

void UART2_InitTxBuffers(void)
{    
  unsigned char j;
        for( j = 0; j < 64; j++)
        {
          TxBuff[j] = 0;              // Clear transmit buffer
        }
}
//**************************************************************
unsigned char UART_GetChar(void)
{
  if (RxBuffLen == 0)
  {
    return (0);
  }
  else
  {
    return (RxBufferPtr->buffer [(RxBufferPtr->rdPtr++) & (RX_BUFF_SIZE - 1)]);      // Read next byte in Rx buffer
  }
}
//*****************************************************************
unsigned char UART_CharPresent(void)
{
	long Timeout = 100000;
        
 	while (( RxBuffLen == 0) && (Timeout > 0))
	{
	 	Timeout--;                              // Count down until timeout
	}
	return RxBuffLen;                               // Return length of the Rx buffer
}
//*****************************************************************