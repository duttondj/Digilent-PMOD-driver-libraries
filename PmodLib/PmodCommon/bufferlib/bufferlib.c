/* -------------------------------------------------------------------- */
/*                                                                      */
/*  bufferlib.c - implementation of the buffer library for application  */
/*                of n-way buffering                                    */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ross Kristof											*/
/*              Copyright (C) 2011 Ross Kristof                         */
/************************************************************************/
/*  Module Description: 												*/
/*  Standard library calls for read and writing to buffers		        */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  11/7/2011(RossK):                                                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "../../PmodCommon/bufferlib/bufferlib.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define BUFLIB_NUM_ARRAYS 5 // Number of buffers to be allocated
#define BUFLIB_MEM_SIZE 1280 // Maximum amount of memory allocated for all buffers


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

AppBuffer gBufferArray[BUFLIB_NUM_ARRAYS];
uint8_t gBufLibReadIndex = 0;
uint8_t gBufLibWriteIndex = 0;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


void void BufLibInitBuffers();
{
	int ii = 0;

	// Loop through each array and allocate memory for each buffer
	for(ii = 0; ii < BUFLIB_NUM_ARRAYS;i++)
	{
		gBufferArray[ii].space_used = 0;
		// Size of a buffer is maximum memory divided by amount of buffers
		gBufferArray[ii].buffer = malloc(BUFLIB_MEM_SIZE/BUFLIB_NUM_ARRAYS);
		gBufferArray[ii].in_use = 0;
		gBufferArray[ii].offset = 0;
	}
}

uint16_t BufLibReadBuffer()
{
	uint16_t data;
	uint8_t next = 0;
	uint8_t valid = 1;

	// Check if we're at the end of a given buffer
	if(gBufferArray[gBufLibReadIndex].space_used == gBufferArray[gBufLibReadIndex].offset)
	{
		// Find the next buffer in line
		next = (gBufLibReadIndex + 1) % BUFLIB_NUM_ARRAYS;
		
		// Check that it is ready to be used
		if(!gBufferArray[next].in_use)
		{
			// Reset all the values in that buffer
			gBufferArray[gBufLibReadIndex].space_used = 0;
			gBufferArray[gBufLibReadIndex].offset = 0;
			gBufferArray[gBufLibReadIndex].in_use = 0;
			gBufLibReadIndex = next;
		}
		else // Next buffer is still in use so don't do anything
		{
			valid = 0;
		}
	}

	if(valid)
	{
		// If buffer is valid, then read data from the current position in that buffer
		data = gBufferArray[gBufLibReadIndex].buffer[gBufferArray[gBufLibReadIndex].offset];

		// Increment the offset for the data just read
		(gBufferArray[gBufLibReadIndex].offset)++;
	}

	return data;
}

void BufLibWriteBuffer(uint16_t data)
{
	uint8_t maxSize = BUFLIB_MEM_SIZE/BUFLIB_NUM_ARRAYS;
	uint8_t next = 0;
	uint8_t valid = 1;

	// Check if we're at the end of a given buffer
	if(gBufferArray[gBufLibWriteIndex].space_used == (maxSize / 2))
	{
		// Find the next buffer in line
		next = (gBufLibWriteIndex + 1) % BUFLIB_NUM_ARRAYS;
		
		// Check that it is ready to be used
		if(!gBufferArray[next].in_use)
		{
			// Reset all the values in that buffer
			gBufferArray[gBufLibWriteIndex].offset = 0;
			gBufferArray[gBufLibWriteIndex].in_use = 0;
			gBufLibWriteIndex = next;
		}
		else // Next buffer is still in use so don't do anything
		{
			valid = 0;
		}
	}

	if(valid)
	{
		// If buffer is valid, then store data into the current position in the buffer
		gBufferArray[gBufLibWriteIndex].buffer[gBufferArray[gBufLibWriteIndex].space_used] = data;

		// Increment to the next available memory
		(gBufferArray[gBufLibWriteIndex].space_used)++;
	}
}