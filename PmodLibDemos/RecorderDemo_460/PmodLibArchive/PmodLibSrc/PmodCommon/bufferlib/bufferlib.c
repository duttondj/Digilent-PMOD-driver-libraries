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
/*				Global Variables								*/
/* ------------------------------------------------------------ */

AppBuffer *gBufLibBufferArray;
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
**  Returns: 1 if successfully created the buffers, 0 if failed to create them
**
**	Errors:	0 is returned if failed to create buffers
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
	gBufLibBufferArray = malloc(sizeof(AppBuffer) * BufCount);

	// Loop through each array and allocate memory for each buffer
	for(ii = 0; ii < BufCount;ii++)
	{
		gBufLibBufferArray[ii].space_used = 0;
		// Size of a buffer is maximum memory divided by amount of buffers
		gBufLibBufferArray[ii].buffer = malloc(BufSize);
		gBufLibBufferArray[ii].read = 0;
		gBufLibBufferArray[ii].write = 0;
		gBufLibBufferArray[ii].offset = 0;
	}

	// Set the defaults for where each buffer current points
	gBufLibReadIndex = BufCount - 1;
	gBufLibBufferArray[0].write = 1;
	gBufLibBufferArray[gBufLibReadIndex].read = 1;

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
	if(gBufLibBufferArray[gBufLibReadIndex].space_used == gBufLibBufferArray[gBufLibReadIndex].offset)
	{
		// Find the next buffer in line
		next = (gBufLibReadIndex + 1) % gBufLibNumBuffers;
		
		// Check that it is ready to be used
		if(!gBufLibBufferArray[next].write)
		{
			// Reset all the values in that buffer
			gBufLibBufferArray[gBufLibReadIndex].space_used = 0;
			gBufLibBufferArray[gBufLibReadIndex].offset = 0;
			gBufLibBufferArray[gBufLibReadIndex].read = 0;
			gBufLibReadIndex = next;
			gBufLibBufferArray[gBufLibReadIndex].read = 1;
		}
		else // Next buffer is still in use so don't do anything
		{
			valid = 0;
		}
	}

	if(valid)
	{
		// If buffer is valid, then read data from the current position in that buffer
		*data = gBufLibBufferArray[gBufLibReadIndex].buffer[gBufLibBufferArray[gBufLibReadIndex].offset];

		// Increment the offset for the data just read
		(gBufLibBufferArray[gBufLibReadIndex].offset)++;
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

	// If the buffer isn't flag for write, don't touch
	if(!gBufLibBufferArray[gBufLibWriteIndex].write)
	{
		return 0;
	}

	// Check if we're at the end of a given buffer
	if(gBufLibBufferArray[gBufLibWriteIndex].space_used == (gBufLibBufferSize / 2))
	{
		// Find the next buffer in line
		next = (gBufLibWriteIndex + 1) % gBufLibNumBuffers;
		
		// Check that it is ready to be used
		if(!gBufLibBufferArray[next].read)
		{
			// Reset all the values in that buffer
			gBufLibBufferArray[gBufLibWriteIndex].offset = 0;
			gBufLibBufferArray[gBufLibWriteIndex].write = 0;
			gBufLibWriteIndex = next;
			gBufLibBufferArray[gBufLibWriteIndex].write = 1;
		}
		else // Next buffer is still in use so don't do anything
		{
			valid = 0;
		}
	}

	if(valid)
	{
		// If buffer is valid, then store data into the current position in the buffer
		gBufLibBufferArray[gBufLibWriteIndex].buffer[gBufLibBufferArray[gBufLibWriteIndex].space_used] = data;

		// Increment to the next available memory
		(gBufLibBufferArray[gBufLibWriteIndex].space_used)++;
	}

	return valid;
}

/*  BufLibResetBuffers
**
**	Synopsis:
**  Resets all values to default for all the buffers
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
*/
void BufLibResetBuffers(void)
{
	uint8_t ii = 0;

	// Loop through each buffer and reset it's values
	for(ii = 0; ii < gBufLibNumBuffers;ii++)
	{
		gBufLibBufferArray[ii].space_used = 0;
		gBufLibBufferArray[ii].read = 0;
		gBufLibBufferArray[ii].write = 0;
		gBufLibBufferArray[ii].offset = 0;
	}

	// Set the defaults for where each buffer current points
	gBufLibWriteIndex = 0;
	gBufLibReadIndex = gBufLibNumBuffers - 1;
	gBufLibBufferArray[0].write = 1;
	gBufLibBufferArray[gBufLibReadIndex].read = 1;
}

/*  BufLibFinishWrite
**
**	Synopsis:
**  Clears the write flag so you can read the remaining data from the buffer
**	NOTE: once called you must reset the buffers to start writing again
**
**  Input: none
**
**  Returns: none
**
**	Errors:	none
*/
void BufLibFinishWrite(void)
{
	gBufLibBufferArray[gBufLibWriteIndex].write = 0;
}