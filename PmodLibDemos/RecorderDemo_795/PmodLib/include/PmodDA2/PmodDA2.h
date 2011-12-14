/* -------------------------------------------------------------------- */
/*                                                                      */
/*            pmodDA2.h - Header file for manipulating PmodDA2          */
/*                        with Timer interrupts to produce sound        */
/*                        through the PmodAMP			                */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ross Kristof											*/
/*	Copyright (C) 2011 Ross Kristof 									*/
/************************************************************************/
/*  Module Description: 												*/
/*            Header file for manipulating PmodDA2  			        */
/*            with Timer interrupts to produce sound			        */
/*            through the PmodAMP			                			*/
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  112/3/2011(RossK):                                                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

#ifndef _PMOD_DA2_H_
#define _PMOD_DA2_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

/*  PmodDA2Init
**
**	Synopsis:
**  Initailzes the PmodDA2 and all the buffers needed for playback
**
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Init(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);

/*  PmodDA2Enable
**
**	Synopsis:
**  Enables the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Enable(SpiChannel chn);

/*  PmodDA2Disable
**
**	Synopsis:
**  Disables the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**
**  Returns: none
**
**	Errors:	none
*/
void PmodDA2Disable(SpiChannel chn);

/*  PmodDA2Send
**
**	Synopsis:
**  sends a 16 bit value from the PmodDA2
**
**  Input: SpiChannel chn  - spi channel
**		   uint_16 data - the digital representation of the analog
**						  signal to send to the PmodDA2
**
**  Returns: 1 if successfully sent, 0 if failed to send
**
**	Errors:	0 is returned if failed to send
*/
uint8_t PmodDA2Send(SpiChannel chn, uint16_t data);

#endif