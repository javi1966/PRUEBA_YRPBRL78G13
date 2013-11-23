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
* Filename:	r_timer_user.c
* Abstract:	This file implements device driver for TAU module.
* APIlib:	Applilet3 for RL78/G13 V1.01.00 [31 May 2011]
* Device:	R5F100LE
* Compiler:	IAR Systems iccrl78
* Creation date:	18/02/2013
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
uint32_t g_Tau0Ch1Width = 0U;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTTM01 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTTM01_vect
__interrupt void R_TAU0_Channel1_Interrupt(void)
{
	if ((TSR01 & _0001_TAU_OVERFLOW_OCCURS) != 0U)	/* overflow occurs */
	{			
		g_Tau0Ch1Width = (uint32_t)TDR01 + 0x10000U;
	}
	else
	{
		g_Tau0Ch1Width = (uint32_t)TDR01;
	}
	/* Start user code. Do not edit comment generated here */
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
