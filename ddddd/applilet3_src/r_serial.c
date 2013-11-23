/*
*******************************************************************************
* Copyright(C) 2010, 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation. 
*
* This device driver was created by Applilet3 for RL78/G13
* 16-Bit Single-Chip Microcontrollers
* Filename:	r_serial.c
* Abstract:	This file implements device driver for Serial module.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	07/01/2013
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
uint8_t  *gp_Csi01RxAddress;	/* csi01 receive buffer address */
uint16_t g_Csi01RxLen;		/* csi01 receive data length */
uint16_t g_Csi01RxCnt;		/* csi01 receive data count */
uint8_t  *gp_Csi01TxAddress;	/* csi01 send buffer address */
uint16_t g_Csi01TxLen;		/* csi01 send data length */
uint16_t g_Csi01TxCnt;		/* csi01 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the SAU0 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_SAU0_Create(void)
{
	SAU0EN = 1U;	/* supply SAU0 clock */
	NOP();
	NOP();
	NOP();
	NOP();
	SPS0 = _0006_SAU_CK00_FCLK_6 | _0000_SAU_CK01_FCLK_0;
	R_CSI01_Create();
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the CSI01 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI01_Create(void)
{
	ST0 |= _0002_SAU_CH1_STOP_TRG_ON;	/* disable CSI01 */
	CSIMK01 = 1U;	/* disable INTCSI01 interrupt */
	CSIIF01 = 0U;	/* clear INTCSI01 interrupt flag */
	/* Set INTCSI01 low priority */
	CSIPR101 = 1U;
	CSIPR001 = 1U;
	SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;	/* clear error flag */
	SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
	SCR01 = _8000_SAU_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
	SDR01 = _8000_CSI01_DIVISOR;
	SO0 |= _0200_SAU_CH1_CLOCK_OUTPUT_1;	/* CSI01 clock initial level */
	SO0 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;	/* CSI01 SO initial level */
	SOE0 |= _0002_SAU_CH1_OUTPUT_ENABLE;	/* enable CSI01 output */
	/* Set SO01 pin */
	P7 |= 0x08U;
	PM7 &= 0xF7U;
	/* Set SCK01 pin */
	P7 |= 0x20U;
	PM7 &= 0xDFU;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the CSI01 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI01_Start(void)
{
	CSIIF01 = 0U;	/* clear INTCSI01 interrupt flag */
	CSIMK01 = 0U;	/* enable INTCSI01 */
	SO0 |= _0200_SAU_CH1_CLOCK_OUTPUT_1;	/* CSI01 clock initial level */
	SO0 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;	/* CSI01 SO initial level */
	SOE0 |= _0002_SAU_CH1_OUTPUT_ENABLE;	/* enable CSI01 output */
	SS0 |= _0002_SAU_CH1_START_TRG_ON;	/* enable CSI01 */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the CSI01 module operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI01_Stop(void)
{
	ST0 |= _0002_SAU_CH1_STOP_TRG_ON;	/* disable CSI01 */
	SOE0 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;	/* disable CSI01 output */
	CSIMK01 = 1U;	/* disable INTCSI01 interrupt */
	CSIIF01 = 0U;	/* clear INTCSI01 interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function sends CSI01 data.
**
**  Parameters:
**	txbuf: transfer buffer pointer
**	txnum: buffer size
**
**  Returns:
**	MD_OK
**	MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
MD_STATUS R_CSI01_Send(uint8_t *txbuf, uint16_t txnum)
{
	MD_STATUS status = MD_OK;

	if (txnum < 1U)
	{
		status = MD_ARGERROR;
	}
	else
	{
		g_Csi01TxCnt = txnum;	/* send data count */
		gp_Csi01TxAddress = txbuf;	/* send buffer pointer */
		CSIMK01 = 1U;	/* disable INTCSI01 interrupt */
		SIO01 = *gp_Csi01TxAddress;	/* started by writing data to SDR[7:0] */
		gp_Csi01TxAddress++;
		g_Csi01TxCnt--;
		CSIMK01 = 0U;	/* enable INTCSI01 interrupt */
	}

	return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
