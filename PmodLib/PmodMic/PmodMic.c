/************************************************************************/
/*                                                                      */
/*                           pmodMic.c                                 */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Skylar Graika											*/
/*              Copyright (C) 2011 Skylar Graika                         */
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/07/2011(SkylarG):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "./PmodMic/pmodMic.h"
#include "./PmodCommon/bufferlib/bufferlib.h"

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

static uint32_t NumCycles10us = 0;
static uint32_t NumCycles15us = 0;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  
**  fnDelayNcycles
**
**	Synopsis:
**  Block program execution for a minimum number of cpu cycles
**  
**  Input: 
**  	uint32_t systemClock - cpu system clock in Hz
**      uint32_t numCycles - minimum number of cpu cycles
**
**  Returns: none
**
**	Errors:	none
**
**  Notes:
**  This function is not part of the public API therefore a function
**  prototype is not present in pmodJSTK.h and the function is given 
**  a storage class of static.
** 
**  Description:
**  Intruduces a blocking delay based on the "numCycles" which is the minumum
**  number cpu cycles which must pass before returning. The number
**  of cycles passed is determined by polling coprocessor register 9
**  (See PIC32MX Family Data Sheet table 2-2) for a baseline cycle 
**  count then maing subsequent polls taking the difference until
**  the cycle count difference meets or exceeds the minumum desired
**  cycle count.
** 
*/
static void fnDelayNcycles(uint32_t systemClock,uint32_t numCycles)
{
	volatile uint32_t clockStart = _CP0_GET_COUNT();
	volatile uint32_t clockStop =  clockStart + numCycles;
	if(clockStop > clockStart)
	{
		while(_CP0_GET_COUNT() < clockStop);
	}
	else
	{
		while(_CP0_GET_COUNT() > clockStart || _CP0_GET_COUNT() < clockStop);
	}

}


