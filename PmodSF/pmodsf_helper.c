/************************************************************************/
/*                                                                      */
/*   pmodsf_helper.c Implementation file for PmodSF helper functions    */
/*                           fnPmodSFSendCommand  						*/
/*                           fnGetByteFromUint32                        */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSF helper functions                     */
/*                           fnPmodSFSendCommand  						*/
/*                           fnGetByteFromUint32                        */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

#include "pmodsf_helper.h"
/** fnGetByteFromUint32
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
uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos)
{
	return value >> (bytePos * 8) & 255;
}


void fnPmodSFSendCommand(SpiChannel chn,uint8_t command)
{
	PmodSFSetSSLow(chn); //SS to low 
    SpiChnPutC(chn,command); //send read status register command
	SpiChnGetC(chn);
	PmodSFSetSSHigh(chn); //SS to High
}
