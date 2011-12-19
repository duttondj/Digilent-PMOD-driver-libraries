/* -------------------------------------------------------------------- */
/*                                                                      */
/*                    bufferlib.h                                 */
/*                                                                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
/*	Author: 	Ross Kristof											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/* -------------------------------------------------------------------- */
/*  Revision History:													*/
/*                                                                      */
/*  11/7/2011(RossK):                                                  */
/*                                                                      */
/* -------------------------------------------------------------------- */
#ifndef _BUFFERLIB_H_
#define _BUFFERLIB_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */
#define BUFLIB_MAX_ARRAY_MEM 25000

typedef struct appBuffer
{
	uint16_t space_used;
	uint16_t *buffer;
	uint8_t read;
	uint8_t write;
	uint8_t offset;
} AppBuffer;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
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
uint8_t BufLibInitBuffers(uint8_t BufCount, uint16_t BufSize);

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
uint8_t BufLibReadBuffer(uint16_t *data);

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
uint8_t BufLibWriteBuffer(uint16_t data);

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
void BufLibResetBuffers(void);

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
void BufLibFinishWrite(void);

#endif