/************************************************************************/
/*                                                                      */
/*        fnPmodDelayNcycles.c --  Implementation file for                  */
/*                                   fnPmodGetByteFromUint32            */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Implimentation for fnPmodDelayNcycles	                                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <plib.h>
#include "./PmodCommon/utility/pmod_utility.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  
**  fnPmodDelayNcycles
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
void fnPmodDelayNcycles(uint32_t systemClock,uint32_t numCycles)
{
//TODO: Fix this, need to mode and maybe fix the conditions in the loo
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