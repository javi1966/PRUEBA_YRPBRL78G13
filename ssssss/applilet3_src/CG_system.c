/*
*******************************************************************************
* Copyright(C) 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation. 
*
* This device driver was created by Applilet3 for RL78/G13
* 16-Bit Single-Chip Microcontrollers
* Filename:	CG_system.c
* Abstract:	This file implements device driver for System module.
* APIlib:	Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device:	R5F1006A
* Compiler:	IAR Systems iccrl78
* Creation date:	18/01/2013
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "CG_macrodriver.h"
#include "CG_system.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "CG_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the clock generator.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void CLOCK_Init(void)
{
	/* Set fMX */
	CMC = _00_CG_HISYS_PORT | _00_CG_SYSOSC_DEFAULT;
	MSTOP = 1U;
	/* Set fMAIN */
	MCM0 = 0U;
	OSMC = _10_CG_RTC_CLK_FIL;
	/* Set fIH */
	HIOSTOP = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
