/************************************************************************/
/*																		*/
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

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./TestHarness/PmodACL/pmodACL_test_driver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

void PmodACL_INIT(UART_MODULE uartID)
{
	UARTPutS("\r\nPmodACL SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODACL is connected to
	PmodACLInitSpi4Wire(chn,PB_CLOCK,SPI_BITRATE);
}

uint8_t UNIT_PmodACLGetDeviceID(UART_MODULE uartID)
{
	uint8_t results[128];
	uint8_t devID = 0;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLGetDeviceID\r\n",uartID);
	devID = PmodACLGetDeviceID(chn);
	sprintf(results,"Device ID=> %x\r\n",devID);
	UARTPutS(results,uartID);
	return (devID == PMODACL_DEVICE_ID)?1:0;
}

uint8_t UNIT_PmodACLGetAxisData(UART_MODULE uartID)
{
	PMODACL_AXIS pmodACLAxis;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLGetAxisData\r\n",uartID);
	PmodACLGetAxisData(chn,&pmodACLAxis);
	UARTPutS("Exit",uartID);
}
