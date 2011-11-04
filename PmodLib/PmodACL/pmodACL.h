/************************************************************************/
/*	<FILE NAME>	--	<BRIEF DESCRIPTION>         						*/
/*																		*/
/************************************************************************/
/*	Author: 	<AUTHOR NAME> 											*/
/*	Copyright (C) <YEAR> <ENTITY>										*/
/************************************************************************/
/*  Module Description: 												*/
/*  <MODULE DESCRIPTION>												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <MM/DD/YY>(<FIRST NAME><LAST INITIAL): <NOTES>						*/
/*																		*/
/************************************************************************/
#ifndef _PMOD_ACL_H_
#define _PMOD_ACL_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <plib.h>
#include <stdint.h>
#include "./PmodCommon/spi/pmod_spi_common.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

//Register address definitions
#define PMODACL_REG_DEVID  			0x00 
#define PMODACL_REG_THRESH_TAP 		0x1D 
#define PMODACL_REG_OFSX  			0x1E 
#define PMODACL_REG_OFSY  			0x1F 
#define PMODACL_REG_OFSZ  			0x20 
#define PMODACL_REG_DUR  			0x21 
#define PMODACL_REG_LATENT  		0x22 
#define PMODACL_REG_Window  		0x23 
#define PMODACL_REG_THRESH_ACT  	0x24 
#define PMODACL_REG_THRESH_INACT 	0x25 
#define PMODACL_REG_TIME_INACT  	0x26 
#define PMODACL_REG_ACT_INACT_CTL   0x27 
#define PMODACL_REG_THRESH_FF  		0x28 
#define PMODACL_REG_TIME_FF  		0x29 
#define PMODACL_REG_TAP_AXES  		0x2A 
#define PMODACL_REG_ACT_TAP_STATUS  0x2B 
#define PMODACL_REG_BW_RATE  		0x2C 
#define PMODACL_REG_POWER_CTL  		0x2D 
#define PMODACL_REG_INT_ENABLE  	0x2E 
#define PMODACL_REG_INT_MAP  		0x2F 
#define PMODACL_REG_INT_SOURCE  	0x30 
#define PMODACL_REG_DATA_FORMAT  	0x31 
#define PMODACL_REG_DATAX0  		0x32 
#define PMODACL_REG_DATAX1  		0x33 
#define PMODACL_REG_DATAY0  		0x34 
#define PMODACL_REG_DATAY1  		0x35 
#define PMODACL_REG_DATAZ0  		0x36 
#define PMODACL_REG_DATAZ1  		0x37 
#define PMODACL_REG_FIFO_CTL  		0x38 
#define PMODACL_REG_FIFO_STATUS  	0x39 

#define PMODACL_RW_BIT				0x80
#define PMODACL_MB_BIT              0x40

#define PMODACL_DEVICE_ID			0xE5

typedef struct
{
	uint16_t xAxis;
	uint16_t yAxis;
	uint16_t zAxis;
}PMODACL_AXIS;


void PmodACLInitSpi4Wire(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);
uint8_t PmodACLGetDeviceID(SpiChannel chn);
void PmodACLGetAxisData(SpiChannel chn, PMODACL_AXIS *pmodACLAxis);


#endif
