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
* Filename:	r_serial_user.c
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
extern uint8_t  *gp_Csi01RxAddress;	/* csi01 receive buffer address */
extern uint16_t g_Csi01RxLen;		/* csi01 receive data length */
extern uint16_t g_Csi01RxCnt;		/* csi01 receive data count */
extern uint8_t  *gp_Csi01TxAddress;	/* csi01 send buffer address */
extern uint16_t g_Csi01TxLen;		/* csi01 send data length */
extern uint16_t g_Csi01TxCnt;		/* csi01 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTCSI01 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTCSI01_vect
__interrupt void R_CSI01_Interrupt(void)
{
	uint8_t err_type;

	err_type = (uint8_t)(SSR01 & _0001_SAU_OVERRUN_ERROR);
	SIR01 = (uint16_t)err_type;
	if (err_type != 1U)
	{
		if (g_Csi01TxCnt > 0U)
		{
			SIO01 = *gp_Csi01TxAddress;
			gp_Csi01TxAddress++;
			g_Csi01TxCnt--;
		}
		else
		{
			R_CSI01_Callback_SendEnd();	/* complete send */
		}
	}
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is a callback function when CSI01 finishes transmission.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_CSI01_Callback_SendEnd(void)
{
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
