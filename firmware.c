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

#define DIR  0040755
#define FILE 0100755

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

struct fw_header_t {
    u64 size_words;
    char *unk_ident;
    u64 unk;
    void *blob;
    u64 unk2;
};

struct fw_info_t {
    struct fw_header_t *rlc;
    struct fw_header_t *sdma0;
    struct fw_header_t *sdma1;
    struct fw_header_t *ce;
    struct fw_header_t *pfp;
    struct fw_header_t *me;
    struct fw_header_t *mec1;
    struct fw_header_t *mec2;
};

void copy_swab(void *dst, void *src, size_t size)
{
    u8 *s = src, *d = dst;
    while (size) {
        *d++ = s[3];
        *d++ = s[2];
        *d++ = s[1];
        *d++ = s[0];
        s += 4;
        size -= 4;
    }
}

int copy_firmware(u8 **p, const char *name, struct fw_header_t *hdr, size_t expected_size)
{
    if (expected_size != (hdr->size_words * 4)) {
        kern.printf("copy_firmware: %s: expected size %d, got %d\n",
                    name, expected_size, hdr->size_words * 4);
        return 0;
    }

    copy_swab(*p, hdr->blob, expected_size);
    *p += expected_size;

    return 1;
}

u32 nop_handler[] = {
    0xdc120000, //     mov r4, ctr
    0x31144000, //     seteq r5, r4, #0x4000
    0x95400009, //     cbz r5, l0
    0xc4200016, //     ldw r8, [r0, #0x16]
    0xdc030000, //     mov ctr, r0
    0xcc000049, //     stw r0, [r0, #0x49]
    0xcc200013, //     stw r0, [r8, #0x13]
    0xc424007e, //     ldw r9, [r0, #0x7e]
    0x96400000, // l1: cbz r9, l1
    0x7c408001, //     mov r2, r1
    0x88000000, //     btab
    0xd440007f, // l0: stm r1, [r0, #0x7f]
    0x7c408001, //     mov r2, r1
    0x88000000, //     btab
};

#define PACKET_TYPE_NOP 0x10
#define NOP_START 0xff0

static void patch_pfp(u8 *pfp) {
    copy_swab(&pfp[4 * NOP_START], nop_handler, sizeof(nop_handler));

    // patch the branch table entry
    for(int off = 4 * 0x1000; off < FW_PFP_SIZE; off += 4) {
        if (pfp[off] == 0 && pfp[off + 1] == PACKET_TYPE_NOP) {
            pfp[off+2] = NOP_START >> 8;
            pfp[off+3] = NOP_START & 0xff;
        }
    }
}

ssize_t firmware_extract(void *dest)
{
    u8 *p = dest;

    // Yeah, this calls it Starsha... Liverpool, Starsha, ThebeJ, whatever.
    struct fw_info_t *info = kern.Starsha_UcodeInfo;
    if (!info) {
        kern.printf("firmware_extract: Could not locate firmware table");
        return -1;
    }

    cpio_hdr(&p, "lib", DIR, 0);
    cpio_hdr(&p, "lib/firmware", DIR, 0);
    cpio_hdr(&p, "lib/firmware/radeon", DIR, 0);
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_ce.bin", FILE, FW_CE_SIZE);
    if (!copy_firmware(&p, "CE", info->ce, FW_CE_SIZE))
        return -1;
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_me.bin", FILE, FW_ME_SIZE);
    if (!copy_firmware(&p, "ME", info->me, FW_ME_SIZE))
        return -1;
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_mec.bin", FILE, FW_MEC_SIZE);
    if (!copy_firmware(&p, "MEC", info->mec1, FW_MEC_SIZE))
        return -1;
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_pfp.bin", FILE, FW_PFP_SIZE);
    u8 *pfp = p;
    if (!copy_firmware(&p, "PFP", info->pfp, FW_PFP_SIZE))
        return -1;
    patch_pfp(pfp);
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_rlc.bin", FILE, FW_RLC_SIZE);
    if (!copy_firmware(&p, "RLC", info->rlc, FW_RLC_SIZE))
        return -1;
    cpio_hdr(&p, "lib/firmware/radeon/LIVERPOOL_sdma.bin", FILE, FW_SDMA_SIZE);
    if (!copy_firmware(&p, "SDMA", info->sdma1, FW_SDMA_SIZE))
        return -1;
    cpio_hdr(&p, "TRAILER!!!", FILE, 0);

    size_t size = p - (u8*)dest;
    if (size > FW_CPIO_SIZE) {
        kern.printf("firmware_extract: overflow! %d > %d\n", size, FW_CPIO_SIZE);
        return -1;
    }

    return size;
}
