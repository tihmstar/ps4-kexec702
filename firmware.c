/*
 * ps4-kexec - a kexec() implementation for Orbis OS / FreeBSD
 *
 * Copyright (C) 2015-2016 shuffle2 <godisgovernment@gmail.com>
 * Copyright (C) 2015-2016 Hector Martin "marcan" <marcan@marcan.st>
 *
 * This code is licensed to you under the 2-clause BSD license. See the LICENSE
 * file for more information.
 */

#include "firmware.h"
#include "types.h"
#include "kernel.h"
#include "string.h"
#include "types.h"
#include "crc32.h"

#define DIR  0040755
#define FILE 0100644

static inline char hex(u8 c)
{
    if (c <= 9)
        return '0' + c;
    return 'a' + c - 10;
}

static void hex8(u8 **p, u32 val)
{
    *(*p)++ = hex(val >> 28);
    *(*p)++ = hex((val >> 24) & 0xf);
    *(*p)++ = hex((val >> 20) & 0xf);
    *(*p)++ = hex((val >> 16) & 0xf);
    *(*p)++ = hex((val >> 12) & 0xf);
    *(*p)++ = hex((val >> 8) & 0xf);
    *(*p)++ = hex((val >> 4) & 0xf);
    *(*p)++ = hex(val & 0xf);
}

void cpio_hdr(u8 **p, const char *name, u32 mode, size_t size)
{
    size_t name_len = strlen(name);

    // Pad to 4 byte multiple
    while (((uintptr_t)*p) & 0x3)
        *(*p)++ = 0;

    memcpy(*p, "070701", 6);
    *p += 6;
    hex8(p, 0); // c_ino
    hex8(p, mode); // c_mode
    hex8(p, 0); // c_uid
    hex8(p, 0); // c_gid
    hex8(p, 1); // c_nlink
    hex8(p, 0); // c_mtime
    hex8(p, size); // c_filesize
    hex8(p, 0); // c_maj
    hex8(p, 0); // c_min
    hex8(p, 0); // c_rmaj
    hex8(p, 0); // c_rmin
    hex8(p, name_len + 1); // c_namesize
    hex8(p, 0); // c_chksum
    memcpy(*p, name, name_len);
    *p += name_len;
    *(*p)++ = 0;

    while (((uintptr_t)*p) & 0x3)
        *(*p)++ = 0;
}



void copy_edid(u8 **p)
{
	int i;
	u8 *edid = *p;
	u8 *off_edid = kern.edid;
	
	memset(edid, 0, 256);
    *p += 256;
	
	for(i = 0; i < 256; i++)
		*(edid + i) = *(off_edid + i);
	
	*p += 256;
}

void copy_eap_hdd_key(u8 **p)
{
	int i;
	u8 *eap_key = *p;
	u8 *off_eap_key = kern.eap_hdd_key;
	
	memset(eap_key, 0, 0x20);
    *p += 0x20;
	
	for(i = 0; i < 0x20; i++)
	{
		if(i < 0x10)
			*(eap_key + i) = *(off_eap_key + 0xF - i);
		else
			*(eap_key + i) = *(off_eap_key + 0x2F - i);
	}
	*p += 0x20;
}


ssize_t firmware_extract(void *dest)
{
    u8 *p = dest;

    // Yeah, this calls it Starsha... Liverpool, Starsha, ThebeJ, whatever.

	
	//Eap hdd key
	cpio_hdr(&p, "key", DIR, 0);
	cpio_hdr(&p, "key/eap_hdd_key.bin", FILE, 0x20);
	copy_eap_hdd_key(&p);
	
    cpio_hdr(&p, "lib", DIR, 0);
    cpio_hdr(&p, "lib/firmware", DIR, 0);
    
    /** We need detect the size of edid first, on some monitor it is 128 on other 256 bytes, so for now remove it **/
    cpio_hdr(&p, "lib/firmware/edid", DIR, 0);
    cpio_hdr(&p, "lib/firmware/edid/1920x1080.bin", FILE, 256);
	copy_edid(&p);
      

    size_t size = p - (u8*)dest;
    if (size > FW_CPIO_SIZE) {
        kern.printf("firmware_extract: overflow! %d > %d\n", size, FW_CPIO_SIZE);
        return -1;
    }

    return size;
}
