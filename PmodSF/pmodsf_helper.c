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
/*  fnGetByteFromUint32
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

/** fnPmodSFSendCommand
**
**	Synopsis:
**  Sends a command to the PmodSF on the selected SPI channel,
**  use this fungtion only for commands that do not return values
**	Input: SpiChannel chn - spi channel to send command to
**     	   uint8_t command - 8-bit command from the "PMODSF INSTRUCTION SET"
**  Returns: none
**	Errors: none
**  Description:
**  SS is driven low, the 8 bit command is shifted out, 1 dummy byte
**  is shifted in, SS is driven high 
*/
void fnPmodSFSendCommand(SpiChannel chn,uint8_t command)
{
	PmodSFSetSSLow(chn); //SS to low 
    SpiChnPutC(chn,command); //send read status register command
	SpiChnGetC(chn);
	PmodSFSetSSHigh(chn); //SS to High
}
