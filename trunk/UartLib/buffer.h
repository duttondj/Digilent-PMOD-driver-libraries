/* 
 * File:   buffer.h
 * Author: skylar
 *
 * Created on February 16, 2011, 11:10 AM
 */

#ifndef _BUFFER_H
#define	_BUFFER_H

//cbuffer struct
 typedef struct struct_cBuffer
{
     unsigned char *dataptr;         // the physical memory address where the buffer is stored
     unsigned short size;            // the allocated size of the buffer
     unsigned short datalength;      // the length of the data currently in the buffer
     unsigned short dataindex;       // the index into the buffer where the data starts
} cBuffer;


//TODO:
//1.)CREATE FUNCTIONS


#endif	/* _BUFFER_H */

