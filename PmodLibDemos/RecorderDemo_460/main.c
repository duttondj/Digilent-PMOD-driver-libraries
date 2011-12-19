/************************************************************************/
/*																		*/
/*	main.c	--	contains main function of RecorderDemo for Cerebot		*/
/*				32MX4													*/
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	Copyright (C) 2011 Ross Kristof										*/
/************************************************************************/
/*  Module Description: 												*/
/*  The RecorderDemo contains a sample project that demonstrates the	*/
/*	use of the PmodDA2, PmodMIC and BufferLib libraries included		*/
/*	in the PmodLib Library												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <12/09/11>(RossK): Created											*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "setup.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
int main(void)
{
	uint8_t BTN1Status = 0;
	uint8_t prevBTN1Status = 0;
	uint8_t Recording = 0;
	uint8_t BTN2Status = 0;
	uint8_t prevBTN2Status = 0;
	uint8_t Playing = 0;

	// Create 97 buffers @ 256 bytes each
	// Total size limit if 25000 bytes
	// 256 bytes each because the PmodSF has 256 byte pages
	if(!RecorderInit(97, 256))
	{
		return 0;
	}

	while(1)
	{
		// Constantly poll the onboard buttons
		BTN1Status = PORTRead(IOPORT_A) & BIT_6;
		BTN2Status = PORTRead(IOPORT_A) & BIT_7
;
		// If BTN1 changes, toggle recording
		// Hold BTN1 to record
		// Release BTN1 to stop recording
		if(prevBTN1Status != BTN1Status)
		{
			Recording = HandleBTN1(Recording);
		}

		// If BTN2 changes, toggle playback
		// Hold BTN2 to play
		// Release BTN2 to stop playback
		if(prevBTN2Status != BTN2Status)
		{
			Playing = HandleBTN2(Playing);
		}
	
		// Update previous status on each loop
		prevBTN1Status = BTN1Status;
		prevBTN2Status = BTN2Status;
	}

	return 1;
}