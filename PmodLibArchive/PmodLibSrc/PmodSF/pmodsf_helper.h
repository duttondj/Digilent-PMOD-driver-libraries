/************************************************************************/
/*                                                                      */
/*   pmodsf_helper.h Header file for PmodSF helper functions            */
/*                           fnPmodSFSendCommand  						*/
/*                           fnGetByteFromUint32                        */
/************************************************************************/
/*	Author: 	Ryan Hoffman											*/
/*	                                									*/
/************************************************************************/
/*  Module Description: 												*/
/*  Header file for PmodSF helper functions                             */
/*                           fnPmodSFSendCommand  						*/
/*                           fnGetByteFromUint32                        */
/*  Functions are not part of the public API, this file should be       */   
/*  included in every public function implementation file as needed     */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*  10/6/2011(RyanH):                                                   */
/*                                                                      */
/************************************************************************/

#ifndef _PMODSF_HELPER_H_
#define _PMODSF_HELPER_H_

#include "pmodsf.h"

void fnPmodSFSendCommand(SpiChannel chn,uint8_t command);

#endif
