/********************************************
* README.txt for PmodLib library            *
* Author: Ryan Hoffman                      *
* Date: December 13, 2011                   *
*********************************************/

System Requirements:
---------------------------------
This library was developed using MPLAB v8.80 and the Microchip C32 libraries and is
desgined to operate select Digilent Pmods attached to boards running PIC32 processors.

File Manifest:
---------------------------------
README.txt
PmodLib\include\pmodlib.h
PmodLib\include\PmodACL\pmodACL.h
PmodLib\include\PmodCommon\bufferlib\bufferlib.h
PmodLib\include\PmodCommon\spi\pmod_spi_common.h
PmodLib\include\PmodCommon\utility\pmod_utility.h
PmodLib\include\PmodDA2\PmodDA2.h
PmodLib\include\PmodHB5\pmodHB5.h
PmodLib\include\PmodJSTK\pmodJSTK.h
PmodLib\include\PmodMic\PmodMic.h
PmodLib\include\PmodSF\pmodsf.h
PmodLib\include\PmodSF\pmodsf_helper.h
PmodLib\lib\pmodlib_460.a
PmodLib\lib\pmodlib_795.a
PmodLibBuild\PmodLibBuild.mcp
PmodLibBuild\PmodLibBuild.mcs
PmodLibBuild\PmodLibBuild.mcw
PmodLibSrc\pmodlib.h
PmodLibSrc\PmodACL\pmodACL.c
PmodLibSrc\PmodACL\pmodACL.h
PmodLibSrc\PmodACL\pmodACL_PmodACLCalibrate.c
PmodLibSrc\PmodCommon\bufferlib\bufferlib.c
PmodLibSrc\PmodCommon\bufferlib\bufferlib.h
PmodLibSrc\PmodCommon\spi\pmod_spi_common.c
PmodLibSrc\PmodCommon\spi\pmod_spi_common.h
PmodLibSrc\PmodCommon\utility\fnDelayNcycles.c
PmodLibSrc\PmodCommon\utility\fnGetByteFromUint32.c
PmodLibSrc\PmodCommon\utility\pmod_utility.h
PmodLibSrc\PmodDA2\PmodDA2.c
PmodLibSrc\PmodDA2\PmodDA2.h
PmodLibSrc\PmodHB5\pmodHB5.h
PmodLibSrc\PmodHB5\pmodHB5_PmodHB5ChangeDirection.c
PmodLibSrc\PmodHB5\pmodHB5_pmodHB5getQEncRMP.c
PmodLibSrc\PmodHB5\pmodHB5_PmodHB5SetDCInitialDirection.c
PmodLibSrc\PmodHB5\pmodHB5_PmodHB5SetDCPWMDutyCycle.c
PmodLibSrc\PmodJSTK\pmodJSTK.c
PmodLibSrc\PmodJSTK\pmodJSTK.h
PmodLibSrc\PmodMic\PmodMic.c
PmodLibSrc\PmodMic\PmodMic.h
PmodLibSrc\PmodSF\pmodsf.c
PmodLibSrc\PmodSF\pmodsf.h
PmodLibSrc\PmodSF\pmodsf_bulk_erase.c
PmodLibSrc\PmodSF\pmodsf_dpd_rel.c
PmodLibSrc\PmodSF\pmodsf_helper.c
PmodLibSrc\PmodSF\pmodsf_helper.h
PmodLibSrc\PmodSF\pmodsf_page_program.c
PmodLibSrc\PmodSF\pmodsf_read_bytes.c
PmodLibSrc\PmodSF\pmodsf_read_id.c
PmodLibSrc\PmodSF\pmodsf_sector_erase.c
PmodLibSrc\PmodSF\pmodsf_set_clear_status_reg_bits.c
PmodLibSrc\PmodSF\pmodsf_write_disable.c


