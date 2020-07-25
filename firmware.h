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

//sizes eap_hdd_key
#define EAP_HDD_KEY_SIZE 0x20
#define EDID_SIZE	256


#define MAX(x ,y) (((x) > (y)) ? (x) : (y))

// Conservative value (max 113 bytes plus name size plus alignment)
#define CPIO_HEADER_SIZE 256

#define FW_HEADER_SIZE 256

#define FW_CPIO_SIZE (EAP_HDD_KEY_SIZE /* + EDID_SIZE */ + (CPIO_HEADER_SIZE * 16) + \
                       FW_HEADER_SIZE * 8)

ssize_t firmware_extract(void *dest);

#endif
