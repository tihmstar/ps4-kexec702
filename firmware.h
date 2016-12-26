/*
 * ps4-kexec - a kexec() implementation for Orbis OS / FreeBSD
 *
 * Copyright (C) 2015-2016 shuffle2 <godisgovernment@gmail.com>
 * Copyright (C) 2015-2016 Hector Martin "marcan" <marcan@marcan.st>
 *
 * This code is licensed to you under the 2-clause BSD license. See the LICENSE
 * file for more information.
 */

#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "types.h"

// sizes for liverpool
#define FW_CE_SIZE 8576
#define FW_ME_SIZE 16768
#define FW_MEC_SIZE 16768
#define FW_PFP_SIZE 16768
#define FW_RLC_SIZE 6144
#define FW_SDMA_SIZE 4200
/* sizes for gladius
#define FW_CE_SIZE (0x860*4)
#define FW_ME_SIZE (0x1060*4)
#define FW_MEC_SIZE (0x1060*4)
#define FW_PFP_SIZE (0x1060*4)
#define FW_RLC_SIZE (0x800*4)
#define FW_SDMA_SIZE (0x41a*4)
*/

// Conservative value (max 113 bytes plus name size plus alignment)
#define CPIO_HEADER_SIZE 256

// Leave space for 16 files (currently 10)
#define FW_CPIO_SIZE ( (CPIO_HEADER_SIZE * 16) + FW_CE_SIZE + FW_ME_SIZE + \
                       FW_MEC_SIZE + FW_PFP_SIZE + FW_RLC_SIZE + \
                       FW_SDMA_SIZE)

ssize_t firmware_extract(void *dest);

#endif
