/************************************************************************/
/*                                                                      */
/*  This file contains functions declaration for use with the n-way     */
/*  buffering library					                                */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains functions declaration for use with the n-way     */
/*  buffering library									                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/7/2011(RossK):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef _BUFFLIB_TEST_DRIVER_H_
#define _BUFFLIB_TEST_DRIVER_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <peripheral/system.h>	// Set up the system and perihperal clocks for best performance

#include "../../PmodCommon/bufferlib/bufferlib.h"

#define SYSTEM_CLOCK 80000000L  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock

#endif