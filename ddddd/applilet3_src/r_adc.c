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
* Filename:	r_adc.c
* Abstract:	This file implements device driver for ADC module.
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
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

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
**	This function initializes the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Create(void)
{
	ADCEN = 1U;	/* supply AD clock */
	ADM0 = _00_AD_ADM0_INITIALVALUE;	/* disable AD conversion and clear ADM0 register */
	ADMK = 1U;	/* disable INTAD interrupt */
	ADIF = 0U;	/* clear INTAD interrupt flag */
	/* Set INTAD high priority */
	ADPR1 = 0U;
	ADPR0 = 0U;
	/* Set ANI0 - ANI3 pin as analog input */
	PM2 |= 0x0FU;
	ADM0 = _00_AD_CONVERSION_CLOCK_64 | _00_AD_TIME_MODE_NORMAL_1 | _00_AD_OPERMODE_SELECT;
	ADM1 = _00_AD_TRIGGER_SOFTWARE | _00_AD_CONVMODE_CONSELECT;
	ADM2 = _00_AD_POSITIVE_VDD | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _00_AD_RESOLUTION_10BIT;
	ADUL = _FF_AD_ADUL_VALUE;
	ADLL = _00_AD_ADLL_VALUE;
	ADS = _03_AD_INPUT_CHANNEL_3;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Start(void)
{
	ADIF = 0U;	/* clear INTAD interrupt flag */
	ADMK = 0U;	/* enable INTAD interrupt */
	ADCS = 1U;	/* enable AD conversion */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Stop(void)
{
	ADCS = 0U;	/* disable AD conversion */
	ADMK = 1U;	/* disable INTAD interrupt */
	ADIF = 0U;	/* clear INTAD interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables comparator operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Set_OperationOn(void)
{
	ADCE = 1U;	/* enable AD comparator */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops comparator operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Set_OperationOff(void)
{
	ADCE = 0U;	/* disable AD comparator */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function returns the conversion result in the buffer.
**
**  Parameters:
**	buffer: the address where to write the conversion result
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Get_Result(uint16_t *buffer)
{
	*buffer = (uint16_t)(ADCR >> 6U);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
