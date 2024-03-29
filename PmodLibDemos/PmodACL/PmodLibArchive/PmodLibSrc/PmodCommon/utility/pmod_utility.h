/************************************************************************/
/*                                                                      */
/* pmod_utility.h -- utility functions used by pmodlib                  */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	Copyright (C) 2011 Ryan Hoffman    									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Common utility functions used by multiple pmodlib drivers           */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/

#ifndef _PMOD_UTILITY_H_
#define _PMOD_UTILITY_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>


/*  fnPmodGetByteFromUint32
**	Synopsis:
**  Takes in an unsigned 32 bit value and a byte position and returns
**  a byte represending the positions requested
**          --------------------------------------
** Position |	  3    |    2    |   1    |   0  |	
**          --------------------------------------
** Bits     | 32 - 24 | 23 - 16 | 15 - 8 | 7 - 0 |
**          --------------------------------------
**  INPUT:
**  uint32_t value  32 bit value to retrieve byte from
**  uint8_t bytePos byte position to retrieve (0 - 3)
**  OUTPUT:
**  uint8_t representing the byte specified in the bytePos from value
**	Errors:
**	if bytePos > 3 0 is returned since all bits will be shitfed out
*/
uint8_t fnPmodGetByteFromUint32(uint32_t value,uint8_t bytePos);

/*  
**  fnPmodDelayNcycles
**
**	Synopsis:
**  Block program execution for a minimum number of cpu cycles
**  
**  Input: 
**      uint32_t numCycles - minimum number of cpu cycles
**
**  Returns: none
**
**	Errors:	none
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
void fnPmodDelayNcycles(uint32_t numCycles);

#endif
