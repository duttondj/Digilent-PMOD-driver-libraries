#include "hbridgeControl.h"

static uint32_t *DCOutPutCompare[] = {(uint32_t*)0x0,
#if (__PIC32_FEATURE_SET__ == 460)
				(uint32_t*)0xBF803020, //OC1
				(uint32_t*)0xBF803220, //OC2
				(uint32_t*)0xBF803420, //OC3
				(uint32_t*)0xBF803620, //OC4
				(uint32_t*)0xBF803820  //OC5
#else
	#error ********hbridgeControl.h => PROCESSOR NOT SUPPORTED********
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
