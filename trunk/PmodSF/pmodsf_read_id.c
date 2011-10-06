/************************************************************************/
/*                                                                      */
/*   pmodsf_read_id.c Implementation file for PmodSFReadID              */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Implementation file for PmodSFReadID for PmodSF                     */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/
#include "pmodsf.h"
#include "pmodsf_helper.h"
/** PmodSFReadID
**
**	Synopsis:
**  The Read Identification (RDID) instruction allows
**  the 24-bit device identification to be read
**  into a 32 bit unsigned integer 
**	Input: SpiChannel chn - spi channel to read PmodSF ID from
**  Returns: uin32_t => Bits 0 - 7: Memory Capacity
**                     Bits 8 - 15: Memory Type
**                     Bits 16- 23 : Manufacturer ID
**	Errors: none
** 
** Description from the M25P16/M25P128 reference manual:
** Read Identification (RDID)
** The Read Identification (RDID) instruction allows
** the 8-bit manufacturer identification to be read, followed
** by two bytes of device identification. The
* *manufacturer identification is assigned by JEDEC,
** and has the value 20h for STMicroelectronics. The
** device identification is assigned by the device
** manufacturer, and indicates the memory type in
** the first byte (20h), and the memory capacity of the
** device in the second byte 15h (PmodSF-16) and 18h (PmodSF-18).
** Any Read Identification (RDID) instruction while
** an Erase or Program cycle is in progress, is not
** decoded, and has no effect on the cycle that is in
** progress.
** The device is first selected by driving Chip Select
** (S) Low. Then, the 8-bit instruction code for the instruction
** is shifted in. This is followed by the 24-bit
** device identification, stored in the memory, being
** shifted out on Serial Data Output (Q), each bit being
** shifted out during the falling edge of Serial
** Clock (C).
** The Read Identification (RDID) instruction is terminated
** by driving Chip Select (S) High at any time
** during data output.
** When Chip Select (S) is driven High, the device is
** put in the Stand-by Power mode. Once in the
** Stand-by Power mode, the device waits to be selected,
** so that it can receive, decode and execute
** instructions
**
*/
uint32_t PmodSFReadID(SpiChannel chn)
{
    uint32_t pmodID = 0;

	BlockWhileWriteInProgress(chn); 

    PmodSFSetSSLow(chn); //SS to low 

    SpiChnPutC(chn,PMODSF_READ_ID); //Send read ID command
    SpiChnGetC(chn);//first byte will be garbage
  
    SpiChnPutC(chn,0);
    pmodID = SpiChnGetC(chn); //get manufacturer ID
 	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory type
	
	pmodID = pmodID << 8;
    SpiChnPutC(chn,0);
    pmodID |= SpiChnGetC(chn); //get memory capacity
 
    PmodSFSetSSHigh(chn); //SS to High
 
    return pmodID;
}
