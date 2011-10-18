/************************************************************************/
/*                                                                      */
/*   config.h                                                           */
/*   configuration header for testing Digilent PmodLib                  */
/*                                                                      */
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
#define SYSTEM_CLOCK 80000000
#define PB_CLOCK (SYSTEM_CLOCK/2) //peripheral bus clock
#define SPI_BITRATE 625000 //bit rate for SPI port

//Define which module to test, 1 to test, 0 to exclude, module tests are mutually exclusive
//conditional compiling will occur in main.c based on these values
#define PMODSF 0    
#define PMODJSTK 1

#endif

