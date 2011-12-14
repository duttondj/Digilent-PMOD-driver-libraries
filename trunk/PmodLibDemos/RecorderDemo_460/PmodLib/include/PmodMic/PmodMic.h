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

#ifndef _PMOD_MIC_H_
#define _PMOD_MIC_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  PmodMicInit
**
**	Synopsis:
**  Initializes the PmodMic module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
*/
void PmodMicInit(SpiChannel chn, uint32_t pbClock,uint32_t bitRate);

/*  PmodMicGetData
**
**	Synopsis:
**  Gets a 12-bit sample from the PmodMIC on each call
**  
**  Input: SpiChannel chn  - spi channel that the PmodMIC is connected to
**
**  Returns: uint16_t - a 12-bit sample from the PmodMIC
**
**  Description:
**
**  Sets the SPI slave select high and then transfers two 8-bit values from the
**  PmodMIC and puts then back together as a single 16-bit word. Only 12-bits of
**	which actually contain relavent data
*/
void PmodMicTakeSample();

#endif
