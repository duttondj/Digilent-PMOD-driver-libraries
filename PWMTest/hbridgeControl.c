#include "hbridgeControl.h"

static uint32_t *DCOutPutCompare[] = {(uint32_t*)0x0,
#ifdef _OCMP1
				(uint32_t*)(_OCMP1_BASE_ADDRESS + _OC1CON_OC32_MASK), //OC1
#endif
#ifdef _OCMP2
				(uint32_t*)(_OCMP2_BASE_ADDRESS + _OC2CON_OC32_MASK), //OC2
#endif
#ifdef _OCMP3
				(uint32_t*)(_OCMP3_BASE_ADDRESS + _OC3CON_OC32_MASK), //OC3
#endif
#ifdef _OCMP4
				(uint32_t*)(_OCMP4_BASE_ADDRESS + _OC4CON_OC32_MASK), //OC4
#endif
#ifdef _OCMP5
				(uint32_t*)(_OCMP5_BASE_ADDRESS + _OC5CON_OC32_MASK)  //OC5
#endif
};


uint8_t PmodHB5ChangeDirection(HBRIDGE *hBridge)
{	
	if(hBridge->newDirection != hBridge->currentDirection)
	{
		PmodHB5SetDCPWMDutyCycle(0,hBridge->ocChannel);
	
		if(hBridge->rpm == 0)
		{
			if(hBridge->newDirection == PMOD_HB5_DIR_FWD)
			{
				PORTSetBits(hBridge->directionPort,hBridge->directionPortBit);
			}
			else
			{
				PORTClearBits(hBridge->directionPort,hBridge->directionPortBit);
			}
			hBridge->currentDirection = hBridge->newDirection;
	
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

void PmodHB5SetDCPWMDutyCycle(uint16_t dutyCycle,uint8_t oc)
{
	*(DCOutPutCompare[oc]) = dutyCycle;
}

void PmodHB5getQEncRPM(HBRIDGE *hBridge,uint32_t pollsPerSec)
{
	//read and calculate the current quadratue encoding
	uint8_t newQuadPos = PORTReadBits(hBridge->sensorAport,hBridge->sensorAportBit) ^ PORTReadBits(hBridge->sensorBport,hBridge->sensorBportBit) ;

	pollsPerSec /= 5; //calculate the RPM every 200ms
	
	hBridge->pollCount++; //increase the number of polls total
	
	//the position has changed
	if(newQuadPos != hBridge->quadPos)
	{
		hBridge->quadPos = newQuadPos;
		hBridge->pulseCount++;
	}
	//number of polls reached for RPM calculation
	if(hBridge->pollCount == pollsPerSec)
	{
		hBridge->prevRpm = hBridge->rpm;
		hBridge->rpm = (hBridge->pulseCount/4) * 5;

		hBridge->pulseCount = 0;
		hBridge->pollCount = 0;
	}
	
}
