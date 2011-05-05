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

/*  Table 2 Protected Area Sizes                                                                                   
** -----------------------------------------------------------------------------------------------------------------
** |Status Register Content|                                     Memory Content                                    |
** -----------------------------------------------------------------------------------------------------------------
** |  BP2  |  BP1  |  BP0  |              Protected Area              |            Unprotected Area                |
** |----------------------------------------------------------------------------------------------------------------
** |   0   |   0   |   0   | none                                     | All sectors1 (32 sectors: 0 to 31)         |
** |   0   |   0   |   1   | Upper 32nd (Sector 31)                   | Lower 31/32nds (31 sectors: 0 to 30)       |
** |   0   |   1   |   0   | Upper sixteenth (two sectors: 30 and 31) | Lower 15/16ths (30 sectors: 0 to 29)       |
** |   0   |   1   |   1   | Upper eighth (four sectors: 28 to 31)    | Lower seven-eighths (28 sectors: 0 to 27)  |
** |   1   |   0   |   0   | Upper quarter (eight sectors: 24 to 31)  | Lower three-quarters (24 sectors: 0 to 23) |
** |   1   |   0   |   1   | Upper half (sixteen sectors: 16 to 31)   | Lower half (16 sectors: 0 to 15)           |
** |   1   |   1   |   0   | All sectors (32 sectors: 0 to 31)        | none                                       |
** |   1   |   1   |   1   | All sectors (32 sectors: 0 to 31)        | none                                       |
** -----------------------------------------------------------------------------------------------------------------
** Table 3. Memory Organization
** ------------------------------
** | Sector | Address |  Range  |
** ------------------------------
** |   31   | 1F0000h | 1FFFFFh |
** |   30   | 1E0000h | 1EFFFFh |
** |   29   | 1D0000h | 1DFFFFh |
** |   28   | 1C0000h | 1CFFFFh |
** |   27   | 1B0000h | 1BFFFFh |
** |   26   | 1A0000h | 1AFFFFh |
** |   25   | 190000h | 19FFFFh |
** |   24   | 180000h | 18FFFFh |
** |   23   | 170000h | 17FFFFh |
** |   22   | 160000h | 16FFFFh |
** |   21   | 150000h | 15FFFFh |
** |   20   | 140000h | 14FFFFh |
** |   19   | 130000h | 13FFFFh |
** |   18   | 120000h | 12FFFFh |
** |   17   | 110000h | 11FFFFh |
** |   16   | 100000h | 10FFFFh |
** |   15   | 0F0000h | 0FFFFFh |
** |   14   | 0E0000h | 0EFFFFh |
** |   13   | 0D0000h | 0DFFFFh |
** |   12   | 0C0000h | 0CFFFFh |
** |   11   | 0B0000h | 0BFFFFh |
** |   10   | 0A0000h | 0AFFFFh |
** |   9    | 090000h | 09FFFFh |
** |   8    | 080000h | 08FFFFh |
** |   7    | 070000h | 07FFFFh |
** |   6    | 060000h | 06FFFFh |
** |   5    | 050000h | 05FFFFh |
** |   4    | 040000h | 04FFFFh |
** |   3    | 030000h | 03FFFFh |
** |   2    | 020000h | 02FFFFh |
** |   1    | 010000h | 01FFFFh |
** |   0    | 000000h | 00FFFFh |
** ------------------------------
*/
/* ----------------------------------------------------------------------------------------------------------------------------
** |                                                    PMODSF INSTRUCTION SET                                                |
** ----------------------------------------------------------------------------------------------------------------------------
** |                           |                                            |                          |Address|Dummy|  Data  |
** |    Instruction            |                   Description              |One-byte Instruction Code |Bytes  |Bytes|  Bytes |
** |--------------------------------------------------------------------------------------------------------------------------|
** | PMODSF_WRITE_ENABLE       | Write Enable (WREN)                        | 0000 0110 | 06h          |   0   |  0  |   0    |
** | PMODSF_WRITE_DISABLE      | Write Disable (WRDI)                       | 0000 0100 | 04h          |   0   |  0  |   0    |
** | PMODSF_READ_ID            | Read Identification (RDID)                 | 1001 1111 | 9Fh          |   0   |  0  |1 to 3  | 
** | PMODSF_READ_STATUS_REG    | Read Status Register (RDSR)                | 0000 0101 | 05h          |   0   |  0  |1 to 8  |
** | PMODSF_WRITE_STATUS_REG   | Write Status Register (WRSR)               | 0000 0001 | 01h          |   0   |  0  |   1    |
** | PMODSF_READ_DATA_BYTES    | Read Data Bytes (READ)                     | 0000 0011 | 03h          |   3   |  0  |1 to 8  |
** | *PMODSF_READ_DATA_BYTES_HS| Read Data Bytes at Higher Speed (FAST_READ)| 0000 1011 | 0Bh          |   3   |  1  |1 to 8  |
** | PMODSF_PAGE_PGM           | Page Program (PP)                          | 0000 0010 | 02h          |   3   |  0  |1 to 256|
** | *PMODSF_SECTOR_ERASE      | Sector Erase (SE)                          | 1101 1000 | D8h          |   3   |  0  |   0    |
** | *PMODSF_BULK_ERASE        | Bulk Erase (BE)                            | 1100 0111 | C7h          |   0   |  0  |   0    |
** | *PMODSF_DEEP_POWER_DOWN   | Deep Power Down                            | 1011 1001 | B9h          |   0   |  0  |   0    |
** |---------------------------------------------------------------------------------------------------------------------------
** | *PMODSF_RELEASE_FROM_DPD  | Release From Deep Power Down               | 1010 1011 | ABh          |   0   |  3  | 1 to 8 |
** |                           | and Read Electronic Signature              |           |              |       |     |        |
** |                           |--------------------------------------------|           |              |----------------------- 
** |                           | Release from Deep Power-down               |           |              |   0   |  0  |   0    |
** ----------------------------------------------------------------------------------------------------------------------------
**   Items marked with a '*' have not been implimented 
*/

//PMODSF INSTRUCTION SET: See table above for descriptions
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
/* -----------------------------------------------------------------------------------------------------------
** |                                    PMODSF Status Register Format                                        |
** -----------------------------------------------------------------------------------------------------------
** |       bit 7    |                                                                        |    bit 0      |
** -----------------------------------------------------------------------------------------------------------
** | PMODSF_SR_SRWD | 0 | 0 | PMODSF_SR_BP2 | PMODSF_SR_BP1 |  PMODSF_SR_BP0 | PMODSF_SR_WEL | PMODSF_SR_WIP |
** -----------------------------------------------------------------------------------------------------------
*/
// PMODSF Status Register Format, see table above
#define PMODSF_SR_SRWD 0x80 // Status Register Write Protect
#define PMODSF_SR_BP2  0x10 // See Protected Area Sizes
#define PMODSF_SR_BP1  0x8  // See Protected Area Sizes
#define PMODSF_SR_BP0  0x4  // See Protected Area Sizes
#define PMODSF_SR_WEL  0x2  // Write Enable Latch Bit
#define PMODSF_SR_WIP  0x1  // Write In Progress Bit

#define MAX_PAGE_LEN 255 //Maximum size of a page write is 256 bytes (0 - 255)

/* PMODSFCOMMAND is used in all communications
** with the Digilient PMODSF, commands sent
** and data returned will be stored in this
** datastructure
*/
typedef struct 
{
	uint8_t ucInstruction;  // Instruction to send PMODSF, reference PMODSF INSTRUCTION SET
	uint32_t ulReadAddress; // 24 bit address to start read operation from, reference Memory Organization
    uint32_t ulWriteAddress; //24 bit address to start write from, reference Memory Organization
    uint8_t ucStatusRegister; // Reference PMODSF Status Register Format (this field is for inpput and output)
	uint8_t ucID; //Manufacturer identification is assigned by JEDEC (20h for STMicrosystems)
	uint8_t ucMemType; //Memory type (typically 20h)
	uint8_t ucMemCapacity; // Memory capacity(15h 16Mb, 18h for 128Mb) 
 	uint8_t data[MAX_PAGE_LEN]; //Data bytes to be read from or written to PMODSF
	uint8_t ucNumBytesReadWrite; //Number of bytes to read from or write to PMODSF
	SPICHANNEL ucSpiChannel; // SPI channel to perform the command on
     	
}PMODSFCOMMAND;


void fnPmodSFsendCommand(PMODSFCOMMAND *command);
void fnSPIConfigureChannelMasterForPMODSF(SPICHANNEL spiChannel,uint32_t ulClock,uint32_t ulBaud);
void fnPmodSFReadID(PMODSFCOMMAND *command);
void fnPmodSFReadStatusRegister(PMODSFCOMMAND *command);
void fnPmodSFCommandNoReturn(PMODSFCOMMAND *command);
void fnPmodSFWriteStatusRegister(PMODSFCOMMAND *command);
void  fnPmodSFPageProgram(PMODSFCOMMAND *command);
void fnPmodSFReadBytes(PMODSFCOMMAND *command);
uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos);
#endif