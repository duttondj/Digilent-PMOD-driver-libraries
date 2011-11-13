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
	PMODACL_AXIS pmodACLAxisBaseline;
	PMODACL_AXIS pmodACLAxisTest;
	uint8_t results[128];
	uint8_t testResult = 1;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLGetAxisData\r\n",uartID);
	
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G);
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);
	
	PmodACLCalibrate(chn,5);

	PmodACLGetAxisData(chn,&pmodACLAxisBaseline);
	sprintf(results,"Getting baseline measurements=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisBaseline.xAxis,pmodACLAxisBaseline.yAxis,pmodACLAxisBaseline.zAxis);
	UARTPutS(results,uartID);
	
	UARTPutS("While holding the board at a different angle to change accelerometer measurements, press any key",uartID);
	UARTGetOneByte(uartID);
	
	PmodACLGetAxisData(chn,&pmodACLAxisTest);
	sprintf(results,"Results=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisTest.xAxis,pmodACLAxisTest.yAxis,pmodACLAxisTest.zAxis);
	UARTPutS(results,uartID);
	
	testResult &= (pmodACLAxisBaseline.xAxis != pmodACLAxisTest.xAxis);
	testResult &= (pmodACLAxisBaseline.yAxis != pmodACLAxisTest.yAxis);
	testResult &= (pmodACLAxisBaseline.zAxis != pmodACLAxisTest.zAxis);

	return(testResult);
}

uint8_t UNIT_PmodACLCalibrate(UART_MODULE uartID)
{
	
	PMODACL_AXIS pmodACLAxisTest;
	PMODACL_AXIS pmodACLAxisBaseline;
	uint8_t results[128];
	uint8_t testResult = 1;
	int32_t offsetRegisterReturned = 0;
	int32_t offsetRegisterFromReg = 0;
	uint8_t offsetBytes[3];

	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLCalibrate\r\n",uartID);
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G);
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);
	


	PmodACLGetAxisData(chn,&pmodACLAxisBaseline);
	sprintf(results,"Getting baseline measurements=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisBaseline.xAxis ,pmodACLAxisBaseline.yAxis,pmodACLAxisBaseline.zAxis);
	UARTPutS(results,uartID);
	
	offsetRegisterReturned = PmodACLCalibrate(chn,5);
	
	PmodACLGetAxisData(chn,&pmodACLAxisTest);
	sprintf(results,"Results after calibration=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisTest.xAxis,pmodACLAxisTest.yAxis,pmodACLAxisTest.zAxis);
	UARTPutS(results,uartID);
	
	PmodACLGetOffset(2,offsetBytes);

	offsetRegisterFromReg |= offsetBytes[0] << 16;
	offsetRegisterFromReg |= offsetBytes[1] << 8;
	offsetRegisterFromReg |= offsetBytes[2];
	return ((offsetRegisterFromReg & 0xffffff) == (offsetRegisterReturned & 0xffffff));

}

uint8_t UNIT_PmodACLSetGetTapThresh(UART_MODULE uartID)
{
	uint8_t tapThresh = 0x0F;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetTapThresh\r\n",uartID);
	PmodACLSetTapThresh(chn,tapThresh);
	return (PmodACLGetTapThresh(chn) == tapThresh);
}

uint8_t UNIT_PmodACLInterupt(UART_MODULE uartID)
{
	uint8_t intMap = 0;
	uint8_t singleTap = 0;
	uint8_t doubleTap = 0;
	uint8_t tapSource = 0;
	
	UARTPutS("\n\rEXECUTING TEST =>UNIT_ UNIT_PmodACLInterupt\r\n",uartID);
	UARTPutS("Perform a single and double on the PmodACL to complete test\r\n",uartID);

	//setup PmodACL
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G );
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);
	PmodACLSetTapAxes(chn,PMODACL_BIT_TAP_AXES_TAP_X |PMODACL_BIT_TAP_AXES_TAP_Y | PMODACL_BIT_TAP_AXES_TAP_Z);
	PmodACLSetTapThresh(chn,0x38);
	PmodACLSetTapDuration(chn,0x10);
	PmodACLSetTapLatency(chn,0x50);
	PmodACLSetTapWindow(chn,0xFF);

	//setup external interrupt pin for external int 0 to recivieve input
	PORTSetPinsDigitalIn(IOPORT_D, BIT_0); //CEREBOT32MX4 PIN JH-08

	INTDisableInterrupts();
	INTClearFlag(INT_INT0);//make sure interrupt flag is cleared
	
	//configure multi vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

	//configure INT0 to trigger on rising edge transition with priority 7
	ConfigINT0(EXT_INT_PRI_7 | RISING_EDGE_INT | EXT_INT_ENABLE); 

	PmodACLSetIntEnable(chn,0); //disable PmodACL interrupts
	
	PmodACLGetIntSource(chn); //get interrupt source, clear interrupt
	
	//map all interrupts to INT1
	PmodACLSetIntMap(chn,0);
	
	//enable interrupts for Single and Double taps
	PmodACLSetIntEnable(chn,PMODACL_BIT_INT_ENABLE_SINGLE_TAP | PMODACL_BIT_INT_ENABLE_DOUBLE_TAP );

	INTEnableInterrupts();

	while(!(singleTap & doubleTap))
	{
		if(isrFired)
		{
			tapSource = PmodACLGetIntSource(chn); //get interrupt source, clear interrupt
			
			isrFired = 0;//clear interrupt flag
			
			if((tapSource & PMODACL_BIT_INT_SOURCE_DOUBLE_TAP) == PMODACL_BIT_INT_SOURCE_DOUBLE_TAP)
			{
				doubleTap = 1;
				UARTPutS("Double Tap\r\n",uartID);
			}
			else if((tapSource & PMODACL_BIT_INT_SOURCE_SINGLE_TAP) == PMODACL_BIT_INT_SOURCE_SINGLE_TAP)
			{
				singleTap = 1;
				UARTPutS("Single Tap\r\n",uartID);
			}		
		}
	}

	return 1;
}


void __ISR(_EXTERNAL_0_VECTOR, ipl7) ExternalInteruptHandlerINT0(void)
{	
	isrFired = 1;
	INTClearFlag(INT_INT0);
}


uint8_t UNIT_PmodACLSetGetDataFormat(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetDataFormat\r\n",uartID);
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_16G);
	return (PmodACLGetDataFormat(chn) == PMODACL_BIT_DATA_FORMAT_RANGE_16G);
}

uint8_t UNIT_PmodACLSetGetPowerCtl(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetPowerCtl\r\n",uartID);
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_AUTO_SLEEP);
	return (PmodACLGetPowerCtl(chn) == PMODACL_BIT_POWER_CTL_AUTO_SLEEP);
}