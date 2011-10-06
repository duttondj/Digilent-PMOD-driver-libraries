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
/*  the Digilent CEREBOT32MX4 and CEREBOT32MX7. For a complete          */
/*  description of opeations and signalling see the manuals for         */
/*  the ST M25P16(PmodSF-16) and M25P128(PmodSF-128)                    */
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
#include <peripheral/system.h>
#include "cerebotboards.h"

/*  Table 2 Protected Area Sizes (PMODSF-16)                                                                                  
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
**  Table 2 Protected Area Sizes (PMODSF-128)                                                                                  
** -----------------------------------------------------------------------------------------------------------------
** |Status Register Content|                                     Memory Content                                    |
** -----------------------------------------------------------------------------------------------------------------
** |  BP2  |  BP1  |  BP0  |              Protected Area              |            Unprotected Area                |
** |----------------------------------------------------------------------------------------------------------------
** |   0   |   0   |   0   | none                                     | All sectors(0 to 63)                       |
** |   0   |   0   |   1   | Upper 64th (1 Sector, 2Mb)               | Sectors: 0 to 62                           |
** |   0   |   1   |   0   | Upper 32nd (2 Sectors, 4 Mb)             | Sectors: 0 to 61)                          | 
** |   0   |   1   |   1   | Upper 16th (4 sectors, 8 Mb)             | Sectors: 0 to 59)                          |
** |   1   |   0   |   0   | Upper 8th  (8 sectors, 16 Mb)            | Sectors: 0 to 55)                          |
** |   1   |   0   |   1   | Upper 1/4  (16 sectors, 32 Mb)           | Sectors: 0 to 47)                          |
** |   1   |   1   |   0   | Upper 1/2  (32 sectors, 64 Mb)           | Sectors: 0 to 31)                          |
** |   1   |   1   |   1   | All sectors (64 sectors, 128 Mb)         | none                                       |
** -----------------------------------------------------------------------------------------------------------------
** Table 3. Memory Organization (PMODSF-16)
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
** Table 3. Memory Organization (PMODSF-128)
** ------------------------------
** | Sector | Address |  Range  |
** ------------------------------
** |   63   | FC0000h | FFFFFFh |
** |   62   | F80000h | FBFFFFh |
** |   61   | F40000h | F7FFFFh |
** |   60   | F00000h | F3FFFFh |
** |   59   | EC0000h | EFFFFFh |
** |   58   | E80000h | EBFFFFh |
** |   57   | E40000h | E7FFFFh |
** |   56   | E00000h | E3FFFFh |
** |   55   | DC0000h | DFFFFFh |
** |   54   | D80000h | DBFFFFh |
** |   53   | D40000h | D7FFFFh |
** |   52   | D00000h | D3FFFFh |
** |   51   | CC0000h | CFFFFFh |
** |   50   | C80000h | CBFFFFh |
** |   49   | C40000h | C7FFFFh |
** |   48   | C00000h | C3FFFFh |
** |   47   | BC0000h | BFFFFFh |
** |   46   | B80000h | BBFFFFh |
** |   45   | B40000h | B7FFFFh |
** |   44   | B00000h | B3FFFFh |
** |   43   | AC0000h | AFFFFFh |
** |   42   | A80000h | ABFFFFh |
** |   41   | A40000h | A7FFFFh |
** |   40   | A00000h | A3FFFFh |
** |   39   | 9C0000h | 9FFFFFh |
** |   38   | 980000h | 9BFFFFh |
** |   37   | 940000h | 97FFFFh |
** |   36   | 900000h | 93FFFFh |
** |   35   | 8C0000h | 8FFFFFh |
** |   34   | 880000h | 8BFFFFh |
** |   33   | 840000h | 87FFFFh |
** |   32   | 800000h | 83FFFFh |
** |   31   | 7C0000h | 7FFFFFh |
** |   30   | 780000h | 7BFFFFh |
** |   29   | 740000h | 77FFFFh |
** |   28   | 700000h | 73FFFFh |
** |   27   | 6C0000h | 6FFFFFh |
** |   26   | 680000h | 6BFFFFh |
** |   25   | 640000h | 67FFFFh |
** |   24   | 600000h | 63FFFFh |
** |   23   | 5C0000h | 5FFFFFh |
** |   22   | 580000h | 5BFFFFh |
** |   21   | 540000h | 57FFFFh |
** |   20   | 500000h | 53FFFFh |
** |   19   | 4C0000h | 4FFFFFh |
** |   18   | 480000h | 4BFFFFh |
** |   17   | 440000h | 47FFFFh |
** |   16   | 400000h | 43FFFFh |
** |   15   | 3C0000h | 3FFFFFh |
** |   14   | 380000h | 3BFFFFh |
** |   13   | 340000h | 37FFFFh |
** |   12   | 300000h | 33FFFFh |
** |   11   | 2C0000h | 2FFFFFh |
** |   10   | 280000h | 2BFFFFh |
** |    9   | 240000h | 27FFFFh |
** |    8   | 200000h | 23FFFFh |
** |    7   | 1C0000h | 1FFFFFh |
** |    6   | 180000h | 1BFFFFh |
** |    5   | 140000h | 17FFFFh |
** |    4   | 100000h | 13FFFFh |
** |    3   | 0C0000h | 0FFFFFh |
** |    2   | 080000h | 0BFFFFh |
** |    1   | 040000h | 07FFFFh |
** |    0   | 000000h | 03FFFFh |
** ------------------------------

** --------------------------------------------------------------------------------------------------
** | Excerpt from PIC32 Familiy Reference Manual Chapter 23 section 23.3.7                          |
** --------------------------------------------------------------------------------------------------
** | The following equation defines the SCKx clock frequency as a function of SPIxBRG settings      |
** |  Fsck = (Fpb) / 2 * (SPIxBRK + 1)                                                              |
** |                                                                                                |
** --------------------------------------------------------------------------------------------------
** |                                       Sample SCKx Frequencies                                  |
** --------------------------------------------------------------------------------------------------
** | SPIxBRG Setting   |    0      |    15      |     31     |     63     |     85     |     127    |
** --------------------------------------------------------------------------------------------------
** | FPB = 50 MHz      | 25.00 MHz | 1.56 MHz   | 781.25 kHz | 390.63 kHz | 290.7 kHz  | 195.31 kHz |
** | FPB = 40 MHz      | 20.00 MHz | 1.25 MHz   | 625.00 kHz | 312.50 kHz | 232.56 kHz | 156.25 kHz |
** | FPB = 25 MHz      | 12.50 MHz | 781.25 kHz | 390.63 kHz | 195.31 kHz | 145.35 kHz | 97.66 kHz  |
** | FPB = 20 MHz      | 10.00 MHz | 625.00 kHz | 312.50 kHz | 156.25 kHz | 116.28 kHz | 78.13 kHz  |
** | FPB = 10 MHZ      | 5.00 MHz  | 312.50 kHz | 156.25 kHz | 78.13 kHz  | 58.14 kHz  | 39.06 kHz  |
** --------------------------------------------------------------------------------------------------

** ----------------------------------------------------------------------------------------------------------------------------
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
** | #PMODSF_READ_DATA_BYTES_HS| Read Data Bytes at Higher Speed (FAST_READ)| 0000 1011 | 0Bh          |   3   |  1  |1 to 8  |
** | PMODSF_PAGE_PGM           | Page Program (PP)                          | 0000 0010 | 02h          |   3   |  0  |1 to 256|
** | PMODSF_SECTOR_ERASE       | Sector Erase (SE)                          | 1101 1000 | D8h          |   3   |  0  |   0    |
** | PMODSF_BULK_ERASE         | Bulk Erase (BE)                            | 1100 0111 | C7h          |   0   |  0  |   0    |
** | *PMODSF_DEEP_POWER_DOWN   | Deep Power Down                            | 1011 1001 | B9h          |   0   |  0  |   0    |
** |---------------------------------------------------------------------------------------------------------------------------
** | *PMODSF_RELEASE_FROM_DPD  | Release From Deep Power Down               | 1010 1011 | ABh          |   0   |  3  | 1 to 8 |
** |                           | and Read Electronic Signature              |           |              |       |     |        |
** |                           |--------------------------------------------|           |              |----------------------- 
** |                           | Release from Deep Power-down               |           |              |   0   |  0  |   0    |
** ----------------------------------------------------------------------------------------------------------------------------
**   Items marked with a '#' have not been implimented 
**   Items marked with a '*' are for the PmodSF-16 only
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
#define PMODSF_DEEP_POWER_DOWN     0xB9 //PmodSF-16 only
#define PMODSF_RELEASE_FROM_DPD    0xAB //PmodSF-16 only
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

#define PMODSF_128_MBIT 0x18	//128 Mb flash
#define PMODSF_16_MBIT 0x15    //16 Mb flash

#define PMOD_SF_PAGE_LEN 256 //Maximum size of a page write is 256 bytes (0 - 255)
enum
{
	PMODSD_MEM_CAPACITY_BYTE,
	PMODSD_MEM_TYPE_BYTE,
	PMODSD_MFID_BYTE,
};

typedef struct
{
     uint32_t portSS;
     uint32_t ssMask;
}SpiPortSS;

static const SpiPortSS SpiIO[] = {
	{0,0},
#if defined __CEREBOT32MX4__
	{IOPORT_D,BIT_9},
	{IOPORT_G,BIT_9}
#endif
#if defined __CEREBOT32MX7__
	{IOPORT_D,BIT_9},
	{0,0},
	{IOPORT_F,BIT_12}

#endif
};


void PmodSFInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);
uint32_t PmodSFReadID(SpiChannel chn);
void PmodSFSetSSHigh(SpiChannel chn);
void PmodSFSetSSLow(SpiChannel chn);
uint8_t PmodSFReadStatusRegister(SpiChannel chn);
void PmodSFWriteStatusRegister(SpiChannel chn,uint8_t statusReg);
void PmodSFWriteEnable(SpiChannel chn);
void PmodSFWriteDisable(SpiChannel chn);
void PmodSFPageProgram(SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address);
void PmodSFReadBytes(SpiChannel chn,uint8_t numBytes,uint8_t *data,uint32_t address);
void PmodSFBulkErase(SpiChannel chn);
void BlockWhileWriteInProgress(SpiChannel chn);
static void fnPmodSFSendCommand(SpiChannel chn,uint8_t command);
static uint8_t fnGetByteFromUint32(uint32_t value,uint8_t bytePos);
void PmodSFClearStatusRegBits(SpiChannel chn,uint8_t bitMask);
void PmodSFSetStatusRegBits(SpiChannel chn,uint8_t bitMask);
void PmodSFSectorErase(SpiChannel chn,uint32_t address);
uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn);
void PmodSFDeepPowerDown(SpiChannel chn);
#endif
