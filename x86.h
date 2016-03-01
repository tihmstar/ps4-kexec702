/*
 * ps4-kexec - a kexec() implementation for Orbis OS / FreeBSD
 *
 * Copyright (C) 2015-2016 shuffle2 <godisgovernment@gmail.com>
 * Copyright (C) 2015-2016 Hector Martin "marcan" <marcan@marcan.st>
 *
 * This code is licensed to you under the 2-clause BSD license. See the LICENSE
 * file for more information.
 */

#ifndef X86_H
#define X86_H

#define CR0_WP (1 << 16)
#define CR0_NW (1 << 29)
#define CR0_CD (1 << 30)

#define	PG_V  (1 << 0)
#define PG_RW (1 << 1)
#define PG_U  (1 << 2)
#define	PG_PS (1 << 7)

#define SEG_TYPE_DATA       (0 << 3)
#define SEG_TYPE_READ_WRITE (1 << 1)
#define SEG_TYPE_CODE       (1 << 3)
#define SEG_TYPE_EXEC_READ  (1 << 1)
#define SEG_TYPE_TSS        ((1 << 3) | (1 << 0))

static inline u64 cr0_read(void)
{
    u64 reg;
    asm volatile("mov %0, cr0;" : "=r" (reg));
    return reg;
}

static inline void cr0_write(u64 val)
{
    asm volatile("mov cr0, %0;" :: "r" (val));
}

static inline u64 cr3_read(void)
{
    u64 reg;
    asm volatile("mov %0, cr3;" : "=r" (reg));
    return reg;
}

static inline void cr3_write(u64 val)
{
    asm volatile("mov cr3, %0;" :: "r" (val));
}

static inline void cr4_pge_disable(void)
{
    u64 cr4_temp;
    asm volatile(
        "mov %0, cr4;"
        "and %0, ~0x80;"
        "mov cr4, %0;"
        : "=r" (cr4_temp)
    );
}

static inline void wbinvd(void)
{
    asm volatile("wbinvd;");
}

static inline void cpu_stop(void)
{
    for (;;)
        asm volatile("cli; hlt;" : : : "memory");
}

static inline void outl(int port, unsigned int data)
{
    asm volatile("out %w1,%0" : : "a" (data), "d" (port));
}

static inline void wrmsr(u32 msr_id, u64 msr_value)
{
    asm volatile (
        "wrmsr"
        : :"c" (msr_id), "a" (msr_value&0xffffffff), "d" (msr_value>>32) );
}

static inline void disable_interrupts(void)
{
    asm volatile("cli");
}

static inline void enable_interrupts(void)
{
    asm volatile("sti");
}

#endif
