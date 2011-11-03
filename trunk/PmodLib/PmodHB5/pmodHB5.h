#ifndef _PMOD_HB5_H_
#define _PMOD_HB5_H_
#include <stdint.h>
#include <plib.h>

typedef struct
{
	uint8_t quadPos;
	uint16_t sensorAport;
	uint16_t sensorAportBit;
	uint16_t sensorBport;
	uint16_t sensorBportBit;
	uint16_t directionPort;
	uint16_t directionPortBit;
	uint32_t pulseCount;
	uint32_t pollCount;
	uint16_t rpm;
	uint16_t prevRpm;
	uint8_t newDirection;
	uint8_t currentDirection;
	uint8_t ocChannel;
	
}HBRIDGE;

#define PMOD_HB5_DIR_CW 0x0
#define PMOD_HB5_DIR_CCW 0x1

uint8_t PmodHB5ChangeDirection(HBRIDGE *hBridge);
void PmodHB5SetDCPWMDutyCycle(uint16_t dutyCycle,uint8_t oc);
void PmodHB5getQEncRPM(HBRIDGE *hBridge,uint32_t pollsPerSec);

#endif
