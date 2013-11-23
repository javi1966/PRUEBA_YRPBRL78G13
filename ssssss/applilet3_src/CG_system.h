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
* Filename:	CG_system.h
* Abstract:	This file implements device driver for System module.
* APIlib:	Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device:	R5F1006A
* Compiler:	IAR Systems iccrl78
* Creation date:	18/01/2013
*******************************************************************************
*/

#ifndef _MDSYSTEM_
#define _MDSYSTEM_
/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/
/*
	Clock operation mode control register (CMC)
*/
/* High-speed system clock pin operation mode (EXCLK, OSCSEL) */
#define	_C0_CG_HISYS_PIN		0xC0U
#define	_00_CG_HISYS_PORT		0x00U	/* X1, X2 as I/O port */
#define	_40_CG_HISYS_OSC		0x40U	/* X1, X2 as crystal/ceramic resonator connection */
#define	_80_CG_HISYS_PORT1		0x80U	/* X1, X2 as I/O port */
#define	_C0_CG_HISYS_EXT		0xC0U	/* X1 as I/O port, X2 as external clock input */
/* Control of X1 high-speed system clock oscillation frequency (AMPH) */
#define	_00_CG_SYSOSC_DEFAULT		0x00U
#define	_00_CG_SYSOSC_UNDER10M		0x00U	/* fX <= 10MHz */
#define	_01_CG_SYSOSC_OVER10M		0x01U	/* fX > 10MHz */

/*
	Clock operation status control register (CSC)
*/
/* Control of high-speed system clock operation (MSTOP) */
#define	_00_CG_HISYS_OPER		0x00U	/* X1 oscillator/external clock operating */
#define	_80_CG_HISYS_STOP		0x80U	/* X1 oscillator/external clock stopped */
/* High-speed OCO operation (HIOSTOP) */
#define	_00_CG_HIO_OPER			0x00U	/* high-speed OCO operating */
#define	_01_CG_HIO_STOP			0x01U	/* high-speed OCO stopped */

/*
	Oscillation stabilization time counter status register (OSTC)
*/
/* Oscillation stabilization time status (MOST18-MOST8) */
#define	_FF_CG_OSCSTAB_STATUS		0xFFU
#define	_00_CG_OSCSTAB_STA0		0x00U	/* < 2^8/fX */
#define	_80_CG_OSCSTAB_STA8		0x80U	/* 2^8/fX */
#define	_C0_CG_OSCSTAB_STA9		0xC0U	/* 2^9/fX */
#define	_E0_CG_OSCSTAB_STA10		0xE0U	/* 2^10/fX */
#define	_F0_CG_OSCSTAB_STA11		0xF0U	/* 2^11/fX */
#define	_F8_CG_OSCSTAB_STA13		0xF8U	/* 2^13/fX */
#define	_FC_CG_OSCSTAB_STA15		0xFCU	/* 2^15/fX */
#define	_FE_CG_OSCSTAB_STA17		0xFEU	/* 2^17/fX */
#define	_FF_CG_OSCSTAB_STA18		0xFFU	/* 2^18/fX */

/*
	Oscillation stabilization time select register (OSTS)
*/
/* Oscillation stabilization time selection (OSTS2-OSTS0) */
#define	_07_CG_OSCSTAB_SEL		0x07U
#define	_00_CG_OSCSTAB_SEL8		0x00U	/* 2^8/fX */
#define	_01_CG_OSCSTAB_SEL9		0x01U	/* 2^9/fX */
#define	_02_CG_OSCSTAB_SEL10		0x02U	/* 2^10/fX */
#define	_03_CG_OSCSTAB_SEL11		0x03U	/* 2^11/fX */
#define	_04_CG_OSCSTAB_SEL13		0x04U	/* 2^13/fX */
#define	_05_CG_OSCSTAB_SEL15		0x05U	/* 2^15/fX */
#define	_06_CG_OSCSTAB_SEL17		0x06U	/* 2^17/fX */
#define	_07_CG_OSCSTAB_SEL18		0x07U	/* 2^18/fX */

/*
	System clock control register (CKC)
*/
/* Status of Main system clock fMAIN (MCS) */
#define	_00_CG_MAINCLK_HIO		0x00U	/* high-speed OCO clock (fIH) */
#define	_20_CG_MAINCLK_HISYS		0x20U	/* high-speed system clock (fMX) */
/* Selection of Main system clock fMAIN (MCM0) */
#define	_00_CG_MAINCLK_SELHIO		0x00U	/* high-speed OCO clock (fIH) */
#define	_10_CG_MAINCLK_SELHISYS		0x10U	/* High-speed system clock (fMX) */

/*
	Operation speed mode control register (OSMC)
*/
/* RTC macro operation clock (WUTMMCK0) */
#define	_00_CG_RTC_CLK_FSUB		0x00U	/* use fSUB clock */
#define	_10_CG_RTC_CLK_FIL		0x10U	/* use fIL clcok */
/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
enum ClockMode
{ 
	HIOCLK, SYSX1CLK, SYSEXTCLK
};
enum PSLevel
{
	PSSTOP, PSHALT
};
enum StabTime
{
	STLEVEL0, STLEVEL1, STLEVEL2, STLEVEL3, STLEVEL4, STLEVEL5, STLEVEL6, STLEVEL7
};
/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void CLOCK_Init(void);
void CG_ReadResetSource(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
