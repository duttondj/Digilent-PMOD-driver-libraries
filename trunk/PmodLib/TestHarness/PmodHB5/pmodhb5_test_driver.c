/************************************************************************/
/*                                                                      */
/*                     pmodHB5_test_driver.c                            */
/*              test driver implimentation file for PmodHB5             */   
/*                Copyright (C) 2011 Ryan Hoffman                       */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains function implimentations for use with            */
/*  the  Digilent PMODHB5 on Digilent Pic32 based boards                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  11/2/2011(RyanH):                                                  */
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "./TestHarness/PmodHB5/pmodhb5_test_driver.h"



/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

uint8_t UNIT_PmodHB5ChangeDirection(UART_MODULE uartID)
{
	uint8_t dir = 0;
	uint8_t result = 1;
	uint32_t delay = 0;
	getRPMSampleCount = 0;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_UNIT_PmodHB5ChangeDirection\r\n",uartID);
	PmodHB5SetDCPWMDutyCycle(0,hBridge.ocChannel);
	for(delay = 0;delay < 1000000;delay++){}; //wait for motor to stop
	PmodHB5SetDCInitialDirection(&hBridge); //set direction to CW
	PmodHB5SetDCPWMDutyCycle(PR2,hBridge.ocChannel);//SET DUTY CYCLE 100%
	dir = fnGetDirFromQenc();
	hBridge.currentDirection = PMOD_HB5_DIR_CW;
	hBridge.newDirection = PMOD_HB5_DIR_CCW;
	
	while(1)
	{
		if(isr1Fire)
		{
			isr1Fire = 0;
			PmodHB5getQEncRPM(&hBridge,2000,5);
			if(getRPMSampleCount == 3000 && PmodHB5ChangeDirection(&hBridge))
			{
				break;
			}
		}
	}

	PmodHB5SetDCPWMDutyCycle(PR2,hBridge.ocChannel)	;

	result &= (fnGetDirFromQenc() == PMOD_HB5_DIR_CCW);
	result &= (hBridge.newDirection == hBridge.currentDirection);
	
	return result;
}


uint8_t UNIT_PmodHB5SetDCPWMDutCycle(UART_MODULE uartID)
{
	uint8_t dutyCycle = 0;
	PmodHB5SetDCPWMDutyCycle(dutyCycle,hBridge.ocChannel)	;
	UARTPutS("\n\rEXECUTING TEST =>UNIT_PmodHB5SetDCPWMDutyCycle\r\n",uartID);
	do
	{
		UARTPutS("Enter a duty cycle percentage between 0 and 100 =>",uartID);
		dutyCycle = getIntegerFromConsole(uartID);
		if(dutyCycle > 100)
		{
			UARTPutS("Invalid Entry\r\n",uartID);
		}
	}while(dutyCycle > 100);
	PORTSetBits(IOPORT_D,BIT_7); //set direction forward

	PmodHB5SetDCPWMDutyCycle(PR2 * dutyCycle/100,hBridge.ocChannel);
	if(OC2RS == PR2 * dutyCycle/100)
	{ 
		return 1;
	}
	else
	{
		return 0;
	}

}

void PmodHB5_INIT(UART_MODULE uartID)
{
	PORTSetPinsDigitalOut(IOPORT_D, BIT_7); //HB5 Direction
	PORTSetPinsDigitalOut(IOPORT_D, BIT_1); //HB5 Enable
	PORTSetPinsDigitalIn(IOPORT_D, BIT_9);  //HB5 Sensor A
	PORTSetPinsDigitalIn(IOPORT_C, BIT_1);  //HB5 Sensor B

	hBridge.sensorAport = IOPORT_D;
	hBridge.sensorAportBit = BIT_9;
	hBridge.sensorBport = IOPORT_C;
	hBridge.sensorBportBit = BIT_1;
	hBridge.directionPort = IOPORT_D;
	hBridge.directionPortBit = BIT_7;
	hBridge.currentDirection = PMOD_HB5_DIR_CW;
	hBridge.newDirection = PMOD_HB5_DIR_CW;
	hBridge.ocChannel = 2;
	

    OpenOC2(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenTimer2(T2_ON | T2_PS_1_256, SYSTEM_CLOCK/PB_DIV/PRESCALE/(TOGGLES_PER_SEC/2));
    
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
	INTEnableSystemMultiVectoredInt();
	
	UARTPutS("\r\nPmodHB5 init complete\r\n",uartID);

}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{	
	isr1Fire = 1;
	if(getRPMSampleCount < 3000)
	{
		getRPMSampleCount++;
	}
	
	// clear the interrupt flag
  	INTClearFlag(INT_T1);

}

static uint8_t fnGetDirFromQenc()
{
	uint8_t initQEnc = 0;
	PmodHB5getQEncRPM(&hBridge,2000,5);
	initQEnc = hBridge.quadPos;
	while(1)
	{	
		PmodHB5getQEncRPM(&hBridge,2000,5);
		switch(initQEnc)
		{
			case Q_0_0:
				if(hBridge.quadPos == Q_0_1)
					return PMOD_HB5_DIR_CW;
				else if(hBridge.quadPos == Q_1_0)
					return PMOD_HB5_DIR_CCW;
				break;
			case Q_0_1:
				if(hBridge.quadPos == Q_1_1)
					return PMOD_HB5_DIR_CW;
				else if(hBridge.quadPos == Q_0_1)
					return PMOD_HB5_DIR_CCW;
				break;
			case Q_1_1:
					if(hBridge.quadPos == Q_1_0)
					return PMOD_HB5_DIR_CW;
				else if(hBridge.quadPos == Q_0_1)
					return PMOD_HB5_DIR_CCW;
				break;
			case Q_1_0:
				if(hBridge.quadPos == Q_0_0)
					return PMOD_HB5_DIR_CW;
				else if(hBridge.quadPos == Q_1_0)
					return PMOD_HB5_DIR_CCW;
				break;
		}
	}
}
