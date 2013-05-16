/*******************************************************************************
 *                            C STMicroelectronics
 *    Reproduction and Communication of this document is strictly prohibited
 *      unless specifically authorized in writing by STMicroelectronics.
 *------------------------------------------------------------------------
 * 	                             APG - AID
 *----------------------------------------------------------------
 * This module provides the header interface for the CRC algorithm
 *----------------------------------------------------------------
 *
 ******************************************************************************/

#ifndef _BOOT_CRC_H_
#define _BOOT_CRC_H_

#include "stdint.h"

// Ethernet FCS algorithm, accumulating the result from a previous CRC calculation
uint32_t crc32(uint32_t, uint8_t *, uint32_t);

#endif /* _BOOT_CRC_H_ */
