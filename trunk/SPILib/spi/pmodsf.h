/************************************************************************/
/*                                                                      */
/*   pmodsf.h -- Library to manipulate the Digilent PMODSF	            */
/*                                                                      */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*  This file contains function definitions, data structures and        */
/*  macros used in manipulating the Digilent PMODSF on                  */
/*  the Digilent CEREBOT32MX4 and CEREBOT32MX7                          */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  5/5/2011(RyanH):                                                    */
/*                                                                      */
/************************************************************************/

#ifndef __PMODSF_H__
#define __PMODSF_H__
#include <peripheral/ports.h>
#include <peripheral/spi.h>
#include <stdint.h>
#include <stdio.h> //remove, for debug use
#include "spi_mx4_7.h"

//Instructions for use with fnPmodSFsendCommand
#define PMODSF_WRITE_ENABLE        0x06
#define PMODSF_WRITE_DISABLE       0x04
#define PMODSF_READ_ID             0x9F
#define PMODSF_READ_STATUS_REG     0x05
#define PMODSF_WRITE_STATUS_REG    0x01
#define PMODSF_READ_DATA_BYTES     0x03
#define PMODSF_READ_DATA_BYTES_HS  0x0B
#define PMODSF_PAGE_PGM            0x02  
#define PMODSF_SECTOR_ERASE        0xD8
#define PMODSF_BULK_ERASE          0xC7
#define PMODSF_DEEP_POWER_DOWN     0xB9
#define PMODSF_RELEASE_FROM_DPD    0xAB

//Status register bits

#define PMODSF_SR_SRWD 0x80
#define PMODSF_SR_BP2  0x10
#define PMODSF_SR_BP1  0x8
#define PMODSF_SR_BP0  0x4
#define PMODSF_SR_WEL  0x2
#define PMODSF_SR_WIP  0x1

#define MAX_PAGE_LEN 256

typedef struct 
{
	uint8_t ucInstruction;
	uint32_t ulReadAddress;
    uint32_t ulWriteAddress;
    uint8_t ucStatusRegister;
	uint8_t ucID;
	uint8_t ucMemType;
	uint8_t ucMemCapacity;
 	uint8_t data[MAX_PAGE_LEN];
	uint8_t ucNumBytesReadWrite;
	SPICHANNEL ucSpiChannel;
     	
}PMODSFCOMMAND;


void fnPmodSFsendCommand(PMODSFCOMMAND *command);
uint8_t fnPmodSFReadByte(SPICHANNEL channel);
void fnSPIConfigureChannelMasterForPMODSF(SPICHANNEL spiChannel,uint32_t ulClock,uint32_t ulBaud);
void fnPmodSFReadID(PMODSFCOMMAND *command);
void fnPmodSFReadStatusRegister(PMODSFCOMMAND *command);
void fnPmodSFCommandNoReturn(PMODSFCOMMAND *command);
void fnPmodSFWriteStatusRegister(PMODSFCOMMAND *command);
void  fnPmodSFPageProgram(PMODSFCOMMAND *command);
uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos);
void fnPmodSFReadBytes(PMODSFCOMMAND *command);
#endif