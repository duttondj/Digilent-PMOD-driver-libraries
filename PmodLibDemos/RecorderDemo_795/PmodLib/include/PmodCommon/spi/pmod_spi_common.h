/* -------------------------------------------------------------------- */
/*                                                                      */
/*                    pmod_spi_common.h                                 */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/* -------------------------------------------------------------------- */
#ifndef _PMOD_SPI_COMMON_H_
#define _PMOD_SPI_COMMON_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <plib.h>
#include <stdint.h>


typedef struct
{
     uint32_t portSS; //IO Port
     uint32_t ssMask; //Slave Select bit mask
}SpiPortSS;


/*  PmodSPISetSSLow
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to low
**  
**  Input: SpiChannel chn  - spi channel to set SS bit low on
**
**  Returns: none
*/
void PmodSPISetSSLow(SpiChannel chn);

/*  PmodSPISetSSHigh
**
**	Synopsis:
**  Sets the slave select bit on the selected SPI channel to high
**
**  Input: SpiChannel chn  - spi channel to set SS bit high on
**
**  Returns: none
**
**	Errors:	none
*/
void PmodSPISetSSHigh(SpiChannel chn);

#endif

