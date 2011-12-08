/************************************************************************/
/*  cerebot_robot_remote_types.h - remote/robot common data structures  */
/*  																	*/
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*              Copyright (C) 2011 Ryan Hoffman                         */
/************************************************************************/
/*  Module Description: 												*/
/*	Common data structures used for communication and storing state for */
/*	cerebot robot and cerebot robot remote.                 			*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*  12/4/2011(RyanH):                                                   */
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

//indicates a left or a right turn 
typedef enum 
{
	ROBOT_TURN_LEFT,
	ROBOT_TURN_RIGHT	
}CEREBOT_ROBOT_TURN;

//indicates direction of travel (forward/reverse)
typedef enum 
{
	ROBOT_DIR_REV,
	ROBOT_DIR_FWD	
}CEREBOT_ROBOT_DIR;

//Control message remote->robot
typedef struct
{
	uint8_t fwdRevSpeed;	//duty cycle, must be between 0 - 100
	uint8_t leftRightSpeed; //duty cycle, must be between 0 - 100
	CEREBOT_ROBOT_DIR vehicleDirectionFwdRev; //direction of travel(fwd/rev) detected from Y axis
	CEREBOT_ROBOT_TURN vehicleDirectionLeftRight; //turn direction from X axis (left/right)
}__attribute__((__packed__)) CEREBOT_REMOTE_MSG;

//Status message robot->remote
typedef struct
{
	uint16_t leftWheelRPM;	//RPM value reported from left motor
	uint16_t rightWheelRPM; //RPM value reported from right motor
	uint16_t batteryVoltage; //Battery voltage (raw register value) detected from bus
	CEREBOT_ROBOT_DIR vehicleDirection; //Direction the robot is traveling (FWD/REV)
} __attribute__((__packed__)) CEREBOT_ROBOT_MSG;	

//Joystick axes range values, used in determining joystick position
//during duty cycle calculation
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