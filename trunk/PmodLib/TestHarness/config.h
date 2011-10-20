/************************************************************************/
/*                                                                      */
/*                        config.h                                      */
/*      Configuration header for testing Digilent PmodLib               */
/*               Copyright (C) 2011 Ryan Hoffman                        */                                                                     
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  Processor configuration and contants for use with test driver main  */
/*  this file should be included in the main task loop.                 */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/18/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* ------------------------------------------------------------ */
/*				PIC32 Configuration Settings					*/
/* ------------------------------------------------------------ */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1  //(8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2


/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define SYSTEM_CLOCK 80000000  //system clock speed (8 MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port

/* ------------------------------------------------------------ */
/*                     Pmod Selection                           */
/*  This section defines which module to test, 1 to test,       */
/*  0 to exclude, module tests are mutually exclusive           */
/*  conditional compiling will occur in main.c based            */
/*  on these values.                                            */
/* ------------------------------------------------------------ */
#define PMODSF 0    
#define PMODJSTK 1

//Some test functions will need this value since, since it is not included in the 
//standard function definition accepted by the test loop utilizing the arrays of function 
//pointers. If this value is needed use the extern forward declation in the test module
//for this variable
uint32_t systemClock = SYSTEM_CLOCK;

#endif

