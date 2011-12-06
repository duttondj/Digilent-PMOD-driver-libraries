/************************************************************************/
/*                                                                      */
/*																		*/
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  12/4/2011(RyanH):                                                  */
/*																		*/
/************************************************************************/

#ifndef _CEREBOT_ROBOT_REMOTE_TYPES_
#define _CEREBOT_ROBOT_REMOTE_TYPES_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

typedef enum 
{
	ROBOT_TURN_LEFT,
	ROBOT_TURN_RIGHT
	
}CEREBOT_ROBOT_TURN;

typedef enum 
{
	ROBOT_DIR_REV,
	ROBOT_DIR_FWD
	
}CEREBOT_ROBOT_DIR;


typedef struct
{
	uint16_t fwdRevSpeed;
	uint16_t leftRightSpeed;
	uint8_t vehicleDirectionFwdRev;
	uint8_t vehicleDirectionLeftRight;
}__attribute__((__packed__)) CEREBOT_REMOTE_MSG;

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