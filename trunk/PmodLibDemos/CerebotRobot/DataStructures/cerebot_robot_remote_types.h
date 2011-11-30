#ifndef _CEREBOT_ROBOT_REMOTE_TYPES_
#define _CEREBOT_ROBOT_REMOTE_TYPES_

#include <stdint.h>

typedef enum 
{
	ROBOT_DIR_LEFT,
	ROBOT_DIR_RIGHT
	
}CEREBOT_ROBOT_DIR;


typedef struct
{
	uint16_t fwdRevSpeed;
	uint16_t leftRightSpeed;
	uint8_t vehicleDirectionFwdRev;
	uint8_t vehicleDirectionLeftRight;
	uint8_t resetRobot;
}__attribute__((__packed__))CEREBOT_REMOTE_MSG;

typedef struct
{
	uint16_t leftWheelRPM;
	uint16_t rightWheelRPM;
	uint16_t batteryVoltage;
	uint8_t vehicleDirection;
} __attribute__((__packed__)) CEREBOT_ROBOT_MSG;	

typedef struct
{
	uint16_t yAxisMin;
	uint16_t yAxisMax;
	uint16_t xAxisMin;
	uint16_t xAxisMax;
	uint16_t xAxisRange;
	uint16_t yAxisRange;
	uint16_t xAxisCenter;
	uint16_t yAxisCenter;
}JSTK_AXIS_RANGE;

#endif