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
#include "./PmodCommon/bufferlib/bufferlib.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define BUFLIB_MAX_ARRAY_MEM 20000

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

AppBuffer *gBufferArray;
uint16_t gBufLibMaxSize = 0;
uint8_t gBufLibNumBuffers = 0;
uint16_t gBufLibBufferSize = 0;
uint8_t gBufLibReadIndex = 0;
uint8_t gBufLibWriteIndex = 0;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/*  BufLibInitBuffers
**
**	Synopsis:
**  Initailzes all the buffers needed for the AD/DA library
**
**  Input: uint8_t BufCount - the number of buffers to create
**		   uint16_t BufSize - the size of each buffer in bytes
**
**  Returns: 0 if successfully created the buffers, 1 if failed to create them
**
**	Errors:	1 is returend if failed to create buffers
*/
uint8_t BufLibInitBuffers(uint8_t BufCount, uint16_t BufSize)
{
	uint8_t ii = 0;
	
	// Calculate the max size of the buffer
	gBufLibMaxSize = BufCount * BufSize;

	// Set Global Values
	gBufLibNumBuffers = BufCount;
	gBufLibBufferSize = BufSize;

	if(BUFLIB_MAX_ARRAY_MEM < gBufLibMaxSize)
	{
		return 0;
	}

	// Allocate memory for the buffer arra
	gBufferArray = malloc(sizeof(AppBuffer) * BufCount);

	// Loop through each array and allocate memory for each buffer
	for(ii = 0; ii < BufCount;ii++)
	{
		gBufferArray[ii].space_used = 0;
		// Size of a buffer is maximum memory divided by amount of buffers
		gBufferArray[ii].buffer = malloc(BufSize);
		gBufferArray[ii].read = 0;
		gBufferArray[ii].write = 0;
		gBufferArray[ii].offset = 0;
	}
	gBufLibReadIndex = BufCount - 1;
	gBufferArray[0].write = 1;

	return 1;
}

/*  BufLibReadBuffer
**
**	Synopsis:
**  Reads a 2-byte value from the buffer, switching to the next if no
**	data is available in the current
**
**  Input: uint16_t pointer - a value for the read 2-bytes to be placed in
**
**  Returns: 0 if failed to read 2-bytes, 1 if read 2-bytes
**
**	Errors:	returns 0 if failed to read 2-bytes
*/
uint8_t BufLibReadBuffer(uint16_t *data)
{
	uint8_t next = 0;
	uint8_t valid = 1;
	
	// Check if we're at the end of a given buffer
	if(gBufferArray[gBufLibReadIndex].space_used == gBufferArray[gBufLibReadIndex].offset)
	{
		// Find the next buffer in line
		next = (gBufLibReadIndex + 1) % gBufLibNumBuffers;
		
		// Check that it is ready to be used
		if(!gBufferArray[next].write && gBufferArray[next].space_used > 0)
		{
			// Reset all the values in that buffer
			gBufferArray[gBufLibReadIndex].space_used = 0;
			gBufferArray[gBufLibReadIndex].offset = 0;
			gBufferArray[gBufLibReadIndex].read = 0;
			gBufLibReadIndex = next;
			gBufferArray[gBufLibReadIndex].read = 1;
		}
		else // Next buffer is still in use so don't do anything
		{
			valid = 0;
		}
	}

	if(valid)
	{
		// If buffer is valid, then read data from the current position in that buffer
		*data = gBufferArray[gBufLibReadIndex].buffer[gBufferArray[gBufLibReadIndex].offset];

		// Increment the offset for the data just read
		(gBufferArray[gBufLibReadIndex].offset)++;
	}

	return valid;
}

/*  BufLibWriteBuffer
**
**	Synopsis:
**  Writes a 2-byte value from the buffer, switching to the next if no
**	room is available in the current
**
**  Input: uint16_t - a 2-bytes value to be placed in the buffer
**
**  Returns: 0 if failed to write, 1 if wrote 2-bytes
**
**	Errors:	returns 0 if failed to write
*/
uint8_t BufLibWriteBuffer(uint16_t data)
{
	uint8_t next = 0;
	uint8_t valid = 1;

	// If the buffer is being read from, don't touch
	if(gBufferArray[gBufLibReadIndex].read)
	{
		return 0;
	}

	// Check if we're at the end of a given buffer
	if(gBufferArray[gBufLibWriteIndex].space_used == (gBufLibBufferSize / 2))
	{
		// Find the next buffer in line
		next = (gBufLibWriteIndex + 1) % gBufLibNumBuffers;
		
		// Check that it is ready to be used
		if(!gBufferArray[next].in_use)
		{
			// Reset all the values in that buffer
			gBufferArray[gBufLibWriteIndex].offset = 0;
			gBufferArray[gBufLibWriteIndex].write = 0;
			gBufLibWriteIndex = next;
			gBufferArray[gBufLibWriteIndex].write = 1;
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

	return valid;
}

/*  BufLibClearBuffers
**
**	Synopsis:
**  Resets all values to clear all the buffers
**
**  Input: none
**
**  Returns: 0 if failed to write, 1 if wrote 2-bytes
**
**	Errors:	returns 0 if failed to write
*/