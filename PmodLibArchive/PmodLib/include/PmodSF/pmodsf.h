/************************************************************************/
/*                                                                      */
/* pmodsf.h -- header file for Digilent PmodSF manipulation, public API */
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

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <stdint.h>
#include <plib.h>

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
**
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
**
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
**
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
**
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
**
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

#define PMODSF_PAGE_LEN 256 //Maximum size of a page write is 256 bytes (0 - 255)
/* ---------------------------------------------------------------------------
** |            |         Byte 3              |    Byte 1  |      Byte 0     |
** ---------------------------------------------------------------------------
** |   Pmod     | Manufacturer Identification |     Device Identification    |
** |            |                             |------------------------------|
** |            |                             |Memory Type | Memory Capacity |
** |-------------------------------------------------------------------------|
** | PmodSF-16  |          20h                |     20h    |      15h        |
** |-------------------------------------------------------------------------|
** | PmodSF-128 |          20h                |     20h    |      18h        |
** --------------------------------------------------------------------------- 
*/
#define PMODSF_16_MBIT 0x15    //16 Mb flash
#define PMODSF_128_MBIT 0x18	//128 Mb flash

//Byte positions for manufacturer identification
#define PMODSD_MEM_CAPACITY_BYTE 0
#define	PMODSD_MEM_TYPE_BYTE 1
#define	PMODSD_MFID_BYTE 2

//feature set definitions used in conditional compiling for processor type
#define PIC32_300_400_SERIES ((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499))
#define PIC32_500_600_700_SERIES ((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799))



/*  PmodSFInit
**
**	Synopsis:
**  Initializes the PmodSF module on the selected SPI channel. 
**  
**  Input: SpiChannel chn  - spi channel initialize
**         uint32_t pbClock - peripheral bus clock in Hz
**         uint32_t bitRate - bit rate desired in Hz
**
**  Returns: none
**
**  Description:
**
**  Opens the desired SPI channel in 8-bit mode as a master, enables the slave select bit,
**  and sets the desired bit rate as a function of pbClock/bitRate.  Examples of peripheral bus
**  bit rate combinations are available in the table labeld "Excerpt from PIC32 Familiy Reference 
**  Manual Chapter 23 section 23.3.7" in pmodsf.h.
*/
void PmodSFInit(SpiChannel chn,uint32_t pbClock,uint32_t bitRate);

/*  PmodSFWriteStatusRegister
**
**	Synopsis: Writes configuration bits to the status register
**            on the specified SPI channel
**
**	Input: SpiChannel chn - spi channel to status reg write
**         uint8_t statusReg - status register bits to write
** 
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
** 
** Description from the M25P16/M25P128 reference manual:
**
** The Write Status Register (WRSR) instruction allows
** new values to be written to the Status Register.
** Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded and executed, the device sets
** the Write Enable Latch (WEL).
**
** The Write Status Register (WRSR) instruction is
** entered by driving Chip Select (S) Low, followed
** by the instruction code and the data byte on Serial
** Data Input (D).
** The Write Status Register (WRSR) instruction has
** no effect on b6, b5, b1 and b0 of the Status Register.
** b6 and b5 are always read as 0.
**
** Chip Select (S) must be driven High after the
** eighth bit of the data byte has been latched in. If
** not, the Write Status Register (WRSR) instruction
** is not executed. As soon as Chip Select (S) is driven
** High, the self-timed Write Status Register cycle
** (whose duration is tW) is initiated. While the Write
** Status Register cycle is in progress, the Status
** Register may still be read to check the value of the
** Write In Progress (WIP) bit. The Write In Progress
** (WIP) bit is 1 during the self-timed Write Status
** Register cycle, and is 0 when it is completed.
** When the cycle is completed, the Write Enable
** Latch (WEL) is reset.
**
** The Write Status Register (WRSR) instruction allows
** the user to change the values of the Block
** Protect (BP2, BP1, BP0) bits, to define the size of
** the area that is to be treated as read-only, as defined
** in Table 2.. The Write Status Register
** (WRSR) instruction also allows the user to set or
** reset the Status Register Write Disable (SRWD)
** bit in accordance with the Write Protect (W) signal.
** The Status Register Write Disable (SRWD) bit and
** Write Protect (W) signal allow the device to be put
** in the Hardware Protected Mode (HPM). The Write
** Status Register (WRSR) instruction is not executed
** once the Hardware Protected Mode (HPM) is
** entered.
*/
void PmodSFWriteStatusRegister(SpiChannel chn,uint8_t statusReg);

/*  PmodSFBlockWhileWriteInProgress
**
**	Synopsis:
**  Blocks while a write is in progress
**
**	Input: SpiChannel chn - channel to poll for write in progress
**
**  Returns: none
**
**	Errors: none
**
**	Description:
**
**  During a write operation the Write In Progress(WIP) bit
**  is set, operations that write to the PmodSF will be
**  ignored while this bit is set. Calling this function 
**  blocks further operations until the WIP cleared condition
**  is met.	
*/
void PmodSFBlockWhileWriteInProgress(SpiChannel chn);

/*  PmodSFWriteEnable
**
**	Synopsis:
**  Enables writing by setting the Write Enable Latch(WEL)
**  bit on the selected chanel
**
**	Input: SpiChannel chn - channel to enable writes
**
**  Returns: none
**
**	Errors: none
**
**  Description from the M25P16/M25P128 reference manual:
**
**  The Write Enable (WREN) instruction 
**  sets the Write Enable Latch (WEL) bit.
**
**  The Write Enable Latch (WEL) bit must be set prior
**  to every Page Program (PP), Sector Erase
**  (SE), Bulk Erase (BE) and Write Status Register
**  (WRSR) instruction.
**
**  The Write Enable (WREN) instruction is entered
**  by driving Chip Select (S) Low, sending the instruction
**  code, and then driving Chip Select (S)
**  High.
*/
void PmodSFWriteEnable(SpiChannel chn);

/*  PmodSFReadStatusRegister
**
**	Synopsis:
**  Reads the value of the status register
**
**	Input: SpiChannel chn - SPI channel to read status register
**
**  Returns: uint8_t - 8-bit representation of the status register
**           (see "PMODSF Status Register Format")
**
**	Errors: none
**
**	Description:
**
**	The Read Status Register (RDSR) instruction allows
**	the Status Register to be read. The Status
**	Register may be read at any time, even while a
**	Program, Erase or Write Status Register cycle is in
**	progress. When one of these cycles is in progress,
**	it is recommended to check the Write In Progress
**	(WIP) bit before sending a new instruction to the
**	device. It is also possible to read the Status Register
**	continuously.
**
**	See table "PMODSF Status Register Format" for a
**	description of the status register format.
**
**	The status and control bits of the Status Register
**	are as follows:
**
**	WIP bit:
**  The Write In Progress (WIP) bit indicates
**	whether the memory is busy with a Write Status
**	Register, Program or Erase cycle. When set to 1,
**	such a cycle is in progress, when reset to 0 no
**	such cycle is in progress.
**
**	WEL bit:
**  The Write Enable Latch (WEL) bit indicates
**	the status of the internal Write Enable Latch.
**	When set to 1 the internal Write Enable Latch is
**	set, when set to 0 the internal Write Enable Latch
**	is reset and no Write Status Register, Program or
**	Erase instruction is accepted.
**
**	BP2, BP1, BP0 bits:
**  The Block Protect (BP2,
**	BP1, BP0) bits are non-volatile. They define the
**	size of the area to be software protected against
**	Program and Erase instructions. These bits are
**	written with the Write Status Register (WRSR) instruction.
**	When one or both of the Block Protect
**	(BP2, BP1, BP0) bits is set to 1, the relevant memory
**	area (as defined in Table 2.) becomes protected
**	against Page Program (PP) and Sector Erase
**	(SE) instructions. The Block Protect (BP2, BP1,
**	BP0) bits can be written provided that the Hardware
**	Protected mode has not been set. The Bulk
**	Erase (BE) instruction is executed if, and only if,
**	both Block Protect (BP2, BP1, BP0) bits are 0.
**
**	SRWD bit:
**  The Status Register Write Disable
**	(SRWD) bit is operated in conjunction with the
**	Write Protect (W) signal. The Status Register
**	Write Disable (SRWD) bit and Write Protect (W)
**	signal allow the device to be put in the Hardware
**	Protected mode (when the Status Register Write
**	Disable (SRWD) bit is set to 1, and Write Protect
**	(W) is driven Low). In this mode, the non-volatile
**	bits of the Status Register (SRWD, BP2, BP1,
**	BP0) become read-only bits and the Write Status
**	Register (WRSR) instruction is no longer accepted
**	for execution.
*/
uint8_t PmodSFReadStatusRegister(SpiChannel chn);
 
/*  PmodSFWriteDisable
**
**	Synopsis:
**  Disable writing by resetting the Write Enable Latch(WEL)
**  bit on the selected chanel
**
**	Input: SpiChannel chn - channel to disable writes
**
**  Returns: none
**
**	Errors: none
**
**  Description from the M25P16/M25P128 reference manual:
**
**  The Write Disable (WRDI) instruction 
**  resets the Write Enable Latch (WEL) bit.
**
**  The Write Disable (WRDI) instruction is entered by
**  driving Chip Select (S) Low, sending the instruction
**  code, and then driving Chip Select (S) High.
**
**  The Write Enable Latch (WEL) bit is reset under
**  the following conditions:
**  – Power-up
**  – Write Disable (WRDI) instruction completion
**  – Write Status Register (WRSR) instruction
**    completion
**  – Page Program (PP) instruction completion
**  – Sector Erase (SE) instruction completion
**  – Bulk Erase (BE) instruction completion
*/
void PmodSFWriteDisable(SpiChannel chn);

/*  PmodSFReadID
**
**	Synopsis:
**  The Read Identification (RDID) instruction allows
**  the 24-bit device identification to be read
**  into a 32 bit unsigned integer 
**
**	Input: SpiChannel chn - spi channel to read PmodSF ID from
**
**  Returns: uin32_t => Bits 0 - 7:   Memory Capacity
**                      Bits 8 - 15:  Memory Type
**                      Bits 16- 23 : Manufacturer ID
**	Errors: none
** 
**  Description from the M25P16/M25P128 reference manual:
**
**  The Read Identification (RDID) instruction allows
**  the 8-bit manufacturer identification to be read, followed
**  by two bytes of device identification. The
**  manufacturer identification is assigned by JEDEC,
**  and has the value 20h for STMicroelectronics. The
**  device identification is assigned by the device
**  manufacturer, and indicates the memory type in
**  the first byte (20h), and the memory capacity of the
**  device in the second byte 15h (PmodSF-16) and 18h (PmodSF-18).
**
**  Any Read Identification (RDID) instruction while
**  an Erase or Program cycle is in progress, is not
**  decoded, and has no effect on the cycle that is in
**  progress.
**
**  The device is first selected by driving Chip Select
**  (S) Low. Then, the 8-bit instruction code for the instruction
**  is shifted in. This is followed by the 24-bit
**  device identification, stored in the memory, being
**  shifted out on Serial Data Output (Q), each bit being
**  shifted out during the falling edge of Serial
**  Clock (C).
**
**  The Read Identification (RDID) instruction is terminated
**  by driving Chip Select (S) High at any time
**  during data output.
**
**  When Chip Select (S) is driven High, the device is
**  put in the Stand-by Power mode. Once in the
**  Stand-by Power mode, the device waits to be selected,
**  so that it can receive, decode and execute
**  instructions.
*/
uint32_t PmodSFReadID(SpiChannel chn);

/*  PmodSFDeepPowerDownRelease (PmodSF-16 only)
**
**	Synopsis:
**  Release deep power down on the selected channel,
**  old style electronic signature 14h is returned
**
**	Input: SpiChannel chn - channel to perform Deep 
**                          power down release on
**
**  Returns: uint8_t electronic signature 14h
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description from the M25P16/M25P128 reference manual:
**
**  Once the device has entered the Deep Powerdown
**  mode, all instructions are ignored except the
**  Release from Deep Power-down and Read Electronic
**  Signature (RES) instruction. Executing this
**  instruction takes the device out of the Deep Power-
**  down mode.
**
**  The instruction can also be used to read, on Serial
**  Data Output (Q), the old-style 8-bit Electronic Signature,
**  whose value for the M25P16 is 14h.

**  Please note that this is not the same as, or even a
**  subset of, the JEDEC 16-bit Electronic Signature
**  that is read by the Read Identifier (RDID) instruction.
**  The old-style Electronic Signature is supported
**  for reasons of backward compatibility, only, and
**  should not be used for new designs. New designs
**  should, instead, make use of the JEDEC 16-bit
**  Electronic Signature, and the Read Identifier
**  (RDID) instruction.
**
**  Except while an Erase, Program or Write Status
**  Register cycle is in progress, the Release from
**  Deep Power-down and Read Electronic Signature
**  (RES) instruction always provides access to the
**  old-style 8-bit Electronic Signature of the device,
**  and can be applied even if the Deep Power-down
**  mode has not been entered.
**
**  Any Release from Deep Power-down and Read
**  Electronic Signature (RES) instruction while an
**  Erase, Program or Write Status Register cycle is in
**  progress, is not decoded, and has no effect on the
**  cycle that is in progress.
**
**  The device is first selected by driving Chip Select
**  (S) Low. The instruction code is followed by 3
**  dummy bytes, each bit being latched-in on Serial
**  Data Input (D) during the rising edge of Serial
**  Clock (C). Then, the old-style 8-bit Electronic Signature,
**  stored in the memory, is shifted out on Serial
**  Data Output (Q), each bit being shifted out
**  during the falling edge of Serial Clock (C).
**
**  The Release from Deep Power-down and Read
**  Electronic Signature (RES) instruction is terminated
**  by driving Chip Select (S) High after the Electronic
**  Signature has been read at least once.
**  Sending additional clock cycles on Serial Clock
**  (C), while Chip Select (S) is driven Low, cause the
**  Electronic Signature to be output repeatedly.
**
**  When Chip Select (S) is driven High, the device is
**  put in the Stand-by Power mode. If the device was
**  not previously in the Deep Power-down mode, the
**  transition to the Stand-by Power mode is immediate.
**  If the device was previously in the Deep Power-
**  down mode, though, the transition to the Standby
**  Power mode is delayed by tRES2, and Chip Select
**  (S) must remain High for at least tRES2(max)
**  Once in the Stand-by Power mode, the device waits 
**  to be selected, so that it can receive, decode and
**  execute instructions.
*/
uint8_t PmodSFDeepPowerDownRelease(SpiChannel chn);

/*  PmodSFDeepPowerDown (PmodSF-16 only)
**
**	Synopsis:
**  Performs a deep power down on the selected channel
**
**	Input: SpiChannel chn - channel to perform Deep 
**                          power down
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description from the M25P16/M25P128 reference manual:
**  Executing the Deep Power-down (DP) instruction
**  is the only way to put the device in the lowest consumption
**  mode (the Deep Power-down mode). It
**  can also be used as an extra software protection
**  mechanism, while the device is not in active use,
**  since in this mode, the device ignores all Write,
**  Program and Erase instructions.
**
**  Driving Chip Select (S) High deselects the device,
**  and puts the device in the Standby mode (if there
**  is no internal cycle currently in progress). But this
**  mode is not the Deep Power-down mode. The
**  Deep Power-down mode can only be entered by
**  executing the Deep Power-down (DP) instruction,
**  to reduce the standby current.
**
**  Once the device has entered the Deep Powerdown
**  mode, all instructions are ignored except the
**  Release from Deep Power-down and Read Electronic
**  Signature (RES) instruction. This releases
**  the device from this mode. The Release from
**  Deep Power-down and Read Electronic Signature
**  (RES) instruction also allows the Electronic Signature
**  of the device to be output on Serial Data Output(Q).
**  The Deep Power-down mode automatically stops
**  at Power-down, and the device always Powers-up
**  in the Standby mode.
**
**  The Deep Power-down (DP) instruction is entered
**  by driving Chip Select (S) Low, followed by the instruction
**  code on Serial Data Input (D). Chip Select
**  (S) must be driven Low for the entire duration
**  of the sequence.
**
**  Chip Select (S) must be driven High after the
**  eighth bit of the instruction code has been latched
**  in, otherwise the Deep Power-down (DP) instruction
**  is not executed. As soon as Chip Select (S) is
**  driven High, it requires a delay of tDP before the
**  supply current is reduced to ICC2 and the Deep
**  Power-down mode is entered.
**
**  Any Deep Power-down (DP) instruction, while an
**  Erase, Program or Write cycle is in progress, is rejected
**  without having any effects on the cycle that
**  is in progress.
*/
void PmodSFDeepPowerDown(SpiChannel chn);

/*  PmodSFPageProgram
**
**	Synopsis:
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0).
**
**	Input: SpiChannel chn - spi channel
**         uint32_t numBytes - number of bytes to write to the PmodSF
**         uint8_t *data - data to write to PmodSF
**         uint32_t address - 24bit repsresentation of the page address
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Description from the M25P16/M25P128 reference manual:
**
**  The Page Program (PP) instruction allows bytes to
**  be programmed in the memory (changing bits from
**  1 to 0). Before it can be accepted, a Write Enable
**  (WREN) instruction must previously have been executed.
**  After the Write Enable (WREN) instruction
**  has been decoded, the device sets the Write Enable
**  Latch (WEL).
**
**  The Page Program (PP) instruction is entered by
**  driving Chip Select (S) Low, followed by the instruction
**  code, three address bytes and at least
**  one data byte on Serial Data Input (D). If the 8
**  least significant address bits (A7-A0) are not all
**  zero, all transmitted data that goes beyond the end
**  of the current page are programmed from the start
**  address of the same page (from the address
**  whose 8 least significant bits (A7-A0) are all zero).
**  Chip Select (S) must be driven Low for the entire
**  duration of the sequence.
**  The instruction sequence is shown in Figure 16..
**
**  If more than 256 bytes are sent to the device, previously
**  latched data are discarded and the last 256
**  data bytes are guaranteed to be programmed correctly
**  within the same page. If less than 256 Data
**  bytes are sent to device, they are correctly programmed
**  at the requested addresses without having
**  any effects on the other bytes of the same
**  page.
**
**  Chip Select (S) must be driven High after the
**  eighth bit of the last data byte has been latched in,
**  otherwise the Page Program (PP) instruction is not
**  executed.
**
**  As soon as Chip Select (S) is driven High, the selftimed
**  Page Program cycle (whose duration is tPP)
**  is initiated. While the Page Program cycle is in
**  progress, the Status Register may be read to
**  check the value of the Write In Progress (WIP) bit.
**  The Write In Progress (WIP) bit is 1 during the selftimed
**  Page Program cycle, and is 0 when it is
**  completed. At some unspecified time before the
**  cycle is completed, the Write Enable Latch (WEL)
**  bit is reset.
**
**  A Page Program (PP) instruction applied to a page
**  which is protected by the Block Protect (BP2, BP1,
**  BP0) bits (see Table 2. and Table 3.) is not executed
*/ 
void PmodSFPageProgram(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address);

/*  PmodSFReadBytes
**
**	Synopsis:
**  The Read Data bytes  instruction allows bytes to
**  be read from memory into a buffer from the specified
**  24-bit address on the SPI channel selected.
**
**	Input: SpiChannel chn - spi channel
**         uint32_t numBytes - number of bytes to read from the PmodSF
**         uint8_t *data - buffer to store data read in from the PmodSF
**         uint32_t address - 24bit repsresentation of the page address
**
**  Returns: none
**
**	Errors:	none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
** Description from the M25P16/M25P128 reference manual:
**
** The device is first selected by driving Chip Select
** (S) Low. The instruction code for the Read Data
** Bytes (READ) instruction is followed by a 3-byte
** address (A23-A0), each bit being latched-in during
** the rising edge of Serial Clock (C). Then the memory
** contents, at that address, is shifted out on Serial
** Data Output (Q), each bit being shifted out, at
** a maximum frequency fR, during the falling edge of
** Serial Clock (C).
**
** The first byte addressed can be at any location.
** The address is automatically incremented to the
** next higher address after each byte of data is shifted
** out. The whole memory can, therefore, be read
** with a single Read Data Bytes (READ) instruction.
** When the highest address is reached, the address
** counter rolls over to 000000h, allowing the read
** sequence to be continued indefinitely.
**
** The Read Data Bytes (READ) instruction is terminated
** by driving Chip Select (S) High. Chip Select
** (S) can be driven High at any time during data output.
** Any Read Data Bytes (READ) instruction,
** while an Erase, Program or Write cycle is in
** progress, is rejected without having any effects on
** the cycle that is in progress.
*/
void PmodSFReadBytes(SpiChannel chn,uint32_t numBytes,uint8_t *data,uint32_t address);

/*  PmodSFBulkErase
**
**	Synopsis:
**  Performs a bulk erase of the entire PmodSF setting
**  all bits to 1.
**
**	Input: SpiChannel chn - channel to perform bulk erase on
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
** Description from the M25P16/M25P128 reference manual:
**
** The Bulk Erase (BE) instruction sets all bits to 1
** (FFh). Before it can be accepted, a Write Enable
** (WREN) instruction must previously have been executed.
** After the Write Enable (WREN) instruction
** has been decoded, the device sets the Write Enable
** Latch (WEL).
**
** The Bulk Erase (BE) instruction is entered by driving
** Chip Select (S) Low, followed by the instruction
** code on Serial Data Input (D). Chip Select (S)
** must be driven Low for the entire duration of the
** sequence.
**
** Chip Select (S) must be driven High after the
** eighth bit of the instruction code has been latched
** in, otherwise the Bulk Erase instruction is not executed.
** As soon as Chip Select (S) is driven High,
** the self-timed Bulk Erase cycle (whose duration is
** tBE) is initiated. While the Bulk Erase cycle is in
** progress, the Status Register may be read to
** check the value of the Write In Progress (WIP) bit.
** The Write In Progress (WIP) bit is 1 during the selftimed
** Bulk Erase cycle, and is 0 when it is completed.
** At some unspecified time before the cycle
** is completed, the Write Enable Latch (WEL) bit is
** reset.
**
** The Bulk Erase (BE) instruction is executed only if
** all Block Protect (BP2, BP1, BP0) bits are 0. The
** Bulk Erase (BE) instruction is ignored if one, or
** more, sectors are protected.
*/
void PmodSFBulkErase(SpiChannel chn);

/*  PmodSFClearStatusRegBit
**
**	Synopsis:
**  Clear bits in the status register based on a bit mask passed in, the bits
**  to be cleared should be set to 1 in the bitmask
**
**	Input: SpiChannel chn - spi channel to clear status register bits on
**         uint8_t bitMask - bitmask to apply to status register                          
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Decription: 
**
**  Status register is read in, a bitwise OR is performed on the bitmask passed in,
**  an AND operation is performed on the value of the status register and the bitmask,
**  this value is then written back to the status register.
*/
void PmodSFClearStatusRegBits(SpiChannel chn,uint8_t bitMask);

/*  PmodSFSetStatusRegBit
**
**	Synopsis:
**  Set bits in the status register based on a bit mask passed in
**
**	Input: SpiChannel chn - spi channel to set status register bits on
**         uint8_t bitMask - bitmask to apply to status register                          
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
**  Decription: 
**
**  Status register is read in, an OR operation is performed on the 
**  value of the status register, this value is then written back to the
**  status register.
*/
void PmodSFSetStatusRegBits(SpiChannel chn,uint8_t bitMask);

/*  PmodSFSectorErase
**
**	Synopsis:
**  Erases a sector on the PmodSF on the specified channel
**  containing the 24-bit address passed in
**
**	Input: SpiChannel chn - spi channel to perform sector erase
**         uint32_t address - 24-bit address contained in sector
**                            to erase
**
**  Returns: none
**
**	Errors: none
**
**  Notes: Blocks while Write In Progress bit is set
**         prior to performing operation
**
** Description from the M25P16/M25P128 reference manual:
**
** The Sector Erase (SE) instruction sets to 1 (FFh)
** all bits inside the chosen sector. Before it can be
** accepted, a Write Enable (WREN) instruction
** must previously have been executed. After the
** Write Enable (WREN) instruction has been decoded,
** the device sets the Write Enable Latch (WEL).
**
** The Sector Erase (SE) instruction is entered by
** driving Chip Select (S) Low, followed by the instruction
** code, and three address bytes on Serial
** Data Input (D). Any address inside the Sector (see
** Table 3.) is a valid address for the Sector Erase
** (SE) instruction. Chip Select (S) must be driven
** Low for the entire duration of the sequence.
**
** Chip Select (S) must be driven High after the
** eighth bit of the last address byte has been latched
** in, otherwise the Sector Erase (SE) instruction is
** not executed. As soon as Chip Select (S) is driven
** High, the self-timed Sector Erase cycle (whose duration
** is tSE) is initiated. While the Sector Erase cycle
** is in progress, the Status Register may be read
** to check the value of the Write In Progress (WIP)
** bit. The Write In Progress (WIP) bit is 1 during the
** self-timed Sector Erase cycle, and is 0 when it is
** completed. At some unspecified time before the
** cycle is completed, the Write Enable Latch (WEL)
** bit is reset.
**
** A Sector Erase (SE) instruction applied to a page
** which is protected by the Block Protect (BP2, BP1,
** BP0) bits (see Table 2. and Table 3.) is not executed
*/
void PmodSFSectorErase(SpiChannel chn,uint32_t address);

#endif
