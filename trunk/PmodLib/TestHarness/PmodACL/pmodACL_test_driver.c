/************************************************************************/
/*	pmodACL_test_driver.c -- pmodACL test driver implimentation file	*/
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman 											*/
/*	Copyright (C) 2011 Ryan Hoffman										*/
/************************************************************************/
/*  Module Description: 												*/
/*  pmodACL_test_driver implimenation file								*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/* <11/14/11>(Ryan H): Initial Release									*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./PmodACL/pmodACL.h"
#include "./TestHarness/Common/test_harness_common.h"
#include "./TestHarness/PmodACL/pmodACL_test_driver.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

void fnPmodACL_INIT(UART_MODULE uartID)
{
	UARTPutS("\r\nPmodACL SPI port=>",uartID);
	chn =  getIntegerFromConsole(uartID); //SPI port PMODACL is connected to
	PmodACLInitSpi(chn,PB_CLOCK,SPI_BITRATE);
}

void __ISR(_EXTERNAL_0_VECTOR, ipl7) ExternalInteruptHandlerINT0(void)
{	
	isrFired = 1;
	INTClearFlag(INT_INT0);
}

void fnSetupPmodACLForInterruptsInt1(uint8_t interruptMask,uint8_t tapAxes,uint8_t fifoCtl)
{
	//setup PmodACL
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G );
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);//reset FIFO
	PmodACLSetFIFOCtl(chn,fifoCtl);
	PmodACLSetTapAxes(chn,tapAxes);
	PmodACLSetThreshTap(chn,0x38);
	PmodACLSetTapDuration(chn,0x10);
	PmodACLSetTapLatency(chn,0x50);
	PmodACLSetTapWindow(chn,0xFF);
 	PmodACLSetTimeInact(chn,0x0);
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
	PmodACLSetIntEnable(chn,interruptMask);

	INTEnableInterrupts();

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
	uint16_t delayCount = 0;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLGetAxisData\r\n",uartID);
	
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G);
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);
	//Wait for settings to take effect
	for(delayCount = 0;delayCount < 10000;delayCount++);
	PmodACLCalibrate(chn,5,PMODACL_CALIBRATE_Z_AXIS);
	//Wait for settings to take effect
	for(delayCount = 0;delayCount < 10000;delayCount++);
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
	int32_t offsetRegisterReturned = 0;
	int32_t offsetRegisterFromReg = 0;
	int8_t offsetBytes[3];
	uint8_t calibrationAxis = 0;
	uint16_t delayCount = 0;
	
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLCalibrate\r\n",uartID);
	PmodACLSetDataFormat(chn,PMODACL_BIT_DATA_FORMAT_RANGE_4G);
	PmodACLSetPowerCtl(chn,PMODACL_BIT_POWER_CTL_MEASURE);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_BYPASS);
 	do
 	{
	 	UARTPutS("Select axis with 1g force applied, others should be 0g {x,y,z} =>",uartID);
 		calibrationAxis =  UARTGetOneByte(uartID);
 		UARTSendOneByte(calibrationAxis,uartID);
 		UARTPutS("\r\n",uartID);
 		calibrationAxis -= 'x';
	}while(calibrationAxis > 2);
	
	PmodACLGetAxisData(chn,&pmodACLAxisBaseline);
	sprintf(results,"Getting baseline measurements=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisBaseline.xAxis ,pmodACLAxisBaseline.yAxis,pmodACLAxisBaseline.zAxis);
	UARTPutS(results,uartID);
	
	offsetRegisterReturned = PmodACLCalibrate(chn,10,calibrationAxis);
	for(delayCount = 0;delayCount < 10000;delayCount++){ asm("nop");};
	
	PmodACLGetAxisData(chn,&pmodACLAxisTest);
	sprintf(results,"Results after calibration=> x-axis: %d  y-axis: %d  z-axis: %d\r\n",pmodACLAxisTest.xAxis,pmodACLAxisTest.yAxis,pmodACLAxisTest.zAxis);
	UARTPutS(results,uartID);
	
	PmodACLGetOffset(2,offsetBytes);

	offsetRegisterFromReg |= offsetBytes[0] << 16;
	offsetRegisterFromReg |= offsetBytes[1] << 8;
	offsetRegisterFromReg |= offsetBytes[2];
	
	//make sure offset register is set correctly

	return ((offsetRegisterFromReg & 0xffffff) == (offsetRegisterReturned & 0xffffff));
}

uint8_t UNIT_PmodACLSetGetThreshTap(UART_MODULE uartID)
{
	uint8_t tapThresh = 0x0F;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetThreshTap\r\n",uartID);
	PmodACLSetThreshTap(chn,tapThresh);
	return (PmodACLGetThreshTap(chn) == tapThresh);
}

uint8_t UNIT_PmodACLInterupt(UART_MODULE uartID)
{
	uint8_t intMap = 0;
	uint8_t singleTap = 0;
	uint8_t doubleTap = 0;
	uint8_t tapSource = 0;
	
	UARTPutS("\n\rEXECUTING TEST =>UNIT_ UNIT_PmodACLInterupt\r\n",uartID);
	UARTPutS("Perform a single and double tap on the PmodACL to complete test\r\n",uartID);

	fnSetupPmodACLForInterruptsInt1(PMODACL_BIT_INT_ENABLE_SINGLE_TAP | PMODACL_BIT_INT_ENABLE_DOUBLE_TAP,
									PMODACL_BIT_TAP_AXES_TAP_X |PMODACL_BIT_TAP_AXES_TAP_Y | PMODACL_BIT_TAP_AXES_TAP_Z,
									PMODACL_BIT_FIFO_CTL_BYPASS);

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

uint8_t UNIT_PmodACLGSetGetFIFOCtl(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetFIFOCtl\r\n",uartID);
	PmodACLSetFIFOCtl(chn,PMODACL_BIT_FIFO_CTL_STREAM);
	return (PmodACLGetFIFOCtl(chn) == PMODACL_BIT_FIFO_CTL_STREAM);
}

uint8_t UNIT_PmodACLSetGetOffset(UART_MODULE uartID)
{
	uint8_t offsetBytesOut[PMODACL_NUM_OFFSET_BYTES] = {0x10,0x20,0x30};
	uint8_t offsetBytesIn[PMODACL_NUM_OFFSET_BYTES];
	uint8_t testResult = 1;
	uint8_t byteIndex = 0;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetOffset\r\n",uartID);
	PmodACLSetOffset(chn,offsetBytesOut);
	PmodACLGetOffset(chn,offsetBytesIn);
	for(byteIndex = 0;byteIndex < PMODACL_NUM_OFFSET_BYTES;byteIndex++)
	{
		testResult &= (offsetBytesOut[byteIndex] == offsetBytesIn[byteIndex]);
	}	
	return testResult;;
}

uint8_t UNIT_PmodACLSetGetIntEnable(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetIntEnable\r\n",uartID);
	PmodACLSetIntEnable(chn,PMODACL_BIT_INT_ENABLE_DOUBLE_TAP);
	return (PmodACLGetIntEnable(chn) == PMODACL_BIT_INT_ENABLE_DOUBLE_TAP);
}


uint8_t UNIT_PmodACLSetGetIntMap(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetIntMap\r\n",uartID);
	PmodACLSetIntMap(chn,PMODACL_BIT_INT_MAP_DOUBLE_TAP);
	return (PmodACLGetIntMap(chn) == PMODACL_BIT_INT_MAP_DOUBLE_TAP);
}

uint8_t UNIT_PmodACLSetGetIntSource(UART_MODULE uartID)
{

	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetIntSource\r\n",uartID);
	

	fnSetupPmodACLForInterruptsInt1(PMODACL_BIT_INT_ENABLE_DOUBLE_TAP,
									PMODACL_BIT_TAP_AXES_TAP_X |PMODACL_BIT_TAP_AXES_TAP_Y | PMODACL_BIT_TAP_AXES_TAP_Z,
									PMODACL_BIT_FIFO_CTL_BYPASS);
	
	UARTPutS("Perform a double tap on the PmodACL\r\n",uartID);	

	isrFired = 0;
	while(!isrFired);
	isrFired = 0;

	return (PmodACLGetIntSource(chn) & PMODACL_BIT_INT_SOURCE_DOUBLE_TAP);
}

uint8_t UNIT_PmodACLSetGetTapAxes(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetTapAxes\r\n",uartID);
	PmodACLSetTapAxes(chn,PMODACL_BIT_TAP_AXES_SUPRESS);
	return (PmodACLGetTapAxes(chn) == PMODACL_BIT_TAP_AXES_SUPRESS);
}

uint8_t UNIT_PmodACLSetGetTapDuration(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetTapDuration\r\n",uartID);
	PmodACLSetTapDuration(chn,0x20);
	return (PmodACLGetTapDuration(chn) == 0x20);
}

uint8_t UNIT_PmodACLSetGetTapLatency(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetTapLatency\r\n",uartID);
	PmodACLSetTapLatency(chn,0x40);
	return (PmodACLGetTapLatency(chn) == 0x40);
}

uint8_t UNIT_PmodACLSetGetTapWindow(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetTapWindow\r\n",uartID);
	PmodACLSetTapWindow(chn,0x7F);
	return (PmodACLGetTapWindow(chn) == 0x7F);
}
//Turn on double tap and enable tap for the X axis only
//we should have bits in for X axis activity and X axis Tap
uint8_t UNIT_PmodACLGetActTapStatus(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_ PmodACLGetActTapStatus\r\n",uartID);
	fnSetupPmodACLForInterruptsInt1(PMODACL_BIT_INT_ENABLE_DOUBLE_TAP,PMODACL_BIT_TAP_AXES_TAP_X,PMODACL_BIT_FIFO_CTL_BYPASS);
	UARTPutS("Perform a double tap on the PmodACL X axis\r\n",uartID);
	
	isrFired = 0;
	while(!isrFired);
	isrFired = 0;
	
	return (PmodACLGetActTapStatus(chn) & PMODACL_BIT_ACT_TAP_STATUS_ACT_X|PMODACL_BIT_ACT_TAP_STATUS_TAP_X);
}

uint8_t UNIT_PmodACLSetGetThreshFF(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetThreshFF\r\n",uartID);
	PmodACLSetThreshFF(chn,0x07);
	return (PmodACLGetThreshFF(chn) == 0x07);
}

uint8_t UNIT_PmodACLSetGetBwRate(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetBWRate\r\n",uartID);
    PmodACLSetBwRate(chn,PMODACL_BIT_BW_RATE_LOW_POWER| PMODACL_BIT_BW_RATE_50HZ);
	return (PmodACLGetBwRate(chn) == PMODACL_BIT_BW_RATE_LOW_POWER| PMODACL_BIT_BW_RATE_50HZ);
}

uint8_t UNIT_PmodACLGetFIFOStatus(UART_MODULE uartID)
{
	isrFired = 0;
	fnSetupPmodACLForInterruptsInt1(PMODACL_BIT_INT_ENABLE_SINGLE_TAP,PMODACL_BIT_TAP_AXES_TAP_X,PMODACL_BIT_FIFO_CTL_TRIGGER|0x0F);
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLGetFIFOStatus\r\n",uartID);
	UARTPutS("\n\rBegin tapping the PmodACL to fill up the FIFO buffer.\r\n",uartID);
	while(1)
	{
		if(isrFired)
		{
			isrFired = 0;
			PmodACLGetIntSource(chn); 
			if ((PmodACLGetFIFOStatus(chn) & (PMODACL_BIT_FIFO_STATUS_FIFO_TRIG|0x20)) == (PMODACL_BIT_FIFO_STATUS_FIFO_TRIG|0x20))
			{
				break;
			}
		}
	}	
		
	return (1);
}

uint8_t UNIT_PmodACLSetGetTimeInact(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetTimeInact\r\n",uartID);
    PmodACLSetTimeInact(chn,0x1);
	return (PmodACLGetTimeInact(chn) == 0x1);
}

uint8_t UNIT_PmodACLSetGetThreshAct(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetThreshAct\r\n",uartID);
    PmodACLSetThreshAct(chn,0x3);
	return (PmodACLGetThreshAct(chn) == 0x3);
}

uint8_t UNIT_PmodACLSetGetActInactCtl(UART_MODULE uartID)
{
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodACLSetGetActInactCtl\r\n",uartID);
    PmodACLSetActInactCtl(chn,0x1);
	return (PmodACLGetActInactCtl(chn) == 0x1);
}