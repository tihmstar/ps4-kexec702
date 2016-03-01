/*
 * ps4-kexec - a kexec() implementation for Orbis OS / FreeBSD
 *
 * Copyright (C) 2015-2016 shuffle2 <godisgovernment@gmail.com>
 * Copyright (C) 2015-2016 Hector Martin "marcan" <marcan@marcan.st>
 *
 * This code is licensed to you under the 2-clause BSD license. See the LICENSE
 * file for more information.
 */

#ifndef KEXEC_H
#define KEXEC_H

#include "types.h"

struct sys_kexec_args {
	void *image;
	size_t image_size;
	void *initramfs;
	size_t initramfs_size;
	char *cmd_line;
};

int sys_kexec(void *td, struct sys_kexec_args *uap);

#endif
