/************************************************************************/
/*																		*/
/*	main.c	--	contains main function of RecorderDemo for Cerebot		*/
/*				32MX7													*/
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	Copyright (C) 2011 Ross Kristof										*/
/************************************************************************/
/*  Module Description: 												*/
/*  The RecorderDemo contains a sample project that demonstrates the	*/
/*	use of the PmodDA2, PmodMIC, PmodSF and BufferLib libraries included*/
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
#pragma config ICESEL = ICS_PGx1 //debug mode on Pic32 795
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

	// Local buffers to fill
	uint16_t dataOut[128];
	uint16_t dataIn[128];

	// Current buffer value counts
	uint8_t nDataOut = 0;
	uint8_t nDataIn = 128;
	
	// Current read and write addresses
	uint32_t writeAddress = 0;
	uint32_t readAddress = 0;

	// Create 97 buffers @ 256 bytes each
	// Total size limit if 25000 bytes
	// 256 bytes each because the PmodSF has 256 byte pages
	if(!RecorderInit(97, 256))
	{
		return 0;
	}

	while(1)
	{

		// Read BTN3 to determine if we need to clear the PmodSF
		if(PORTRead(IOPORT_D) & BIT_13)
		{
			// Turn on LED1 and LED2 while the erase is in progress
			PORTSetBits(IOPORT_G, BIT_12 | BIT_13);
		
			// Erase the entire SF
			PmodSFBulkErase(PMOD_SF_SPI);
			PmodSFBlockWhileWriteInProgress(PMOD_SF_SPI);
		
			// Turn off LED1 and LED2
			PORTClearBits(IOPORT_G, BIT_12 | BIT_13);
		}

		// Constantly poll the onboard buttons
		BTN1Status = PORTRead(IOPORT_G) & BIT_6;
		BTN2Status = PORTRead(IOPORT_G) & BIT_7
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

		// If recording is flagged, start filling the PmodSF
		if(Recording)
		{
			// If data is successfully read from the buffer
			if(BufLibReadBuffer(&dataOut[nDataOut]))
			{
				// Increment the buffer count
				nDataOut++;
			}
	
			// Once the buffer is full copy it to the PmodSF
			if(nDataOut == 128)
			{
				PmodSFPageProgram(PMOD_SF_SPI,PMODSF_PAGE_LEN,(uint8_t*)dataOut,writeAddress);
				writeAddress += PMODSF_PAGE_LEN; // Increment the write address by one page
				nDataOut = 0;	// reset buffer count
			}
		}

		// If playing is flagged and there is still data to be read, read from the PmodSF
		if(Playing && readAddress <= writeAddress)
		{
			// If done reading all the values in he buffer
			if(nDataIn == 128)
			{
				// Read a new page from the PmodSF
				PmodSFReadBytes(PMOD_SF_SPI,PMODSF_PAGE_LEN,(uint8_t*)dataIn,readAddress);
				readAddress += PMODSF_PAGE_LEN; // increment the read address by a page
				nDataIn = 0; // reset the read count
			}

			// Write data to the buffer
			if(BufLibWriteBuffer(dataIn[nDataIn]))
			{
				// If data was written successfully increment the read count
				nDataIn++;
			}
		}
	
		// Update previous status on each loop
		prevBTN1Status = BTN1Status;
		prevBTN2Status = BTN2Status;
	}

	return 1;
}