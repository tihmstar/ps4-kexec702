# PS4 kexec implementation

This repo implements a kexec()-style system call for the PS4 Orbis kernel
(FreeBSD derivative). This is designed to boot a Linux kernel directly from
FreeBSD.

This is not an exploit. It is useless without some mechanism of injecting code
into the PS4 OS kernel.

## Building

To build a kexec.a archive using the supplied Makefile, just type `make`. You
can use the Makefile as a template or integrate the source files into your own
code, perhaps as a git submodule.

The code expects a reasonably sane C environment. The Makefile includes a few
CFLAGS tricks that you may find useful. There is also a sample linker script
(`kexec.ld`).

## Usage

The code is designed to be mostly standalone. There are two entry points:
`kernel_init()` in kernel.h and `sys_kexec()` in kexec.h. You should call
`kernel_init()` after your exploit has cleaned up after itself and is in a
clean kernel code context, then inject a system call table entry pointing to
`sys_kexec`.

The `sys_kexec` system call takes (userspace) pointers to the kernel and
initramfs blobs, their sizes, and a pointer to the (null-terminated) command
line string. From userspace, this looks like this:

    int kexec(void *kernel_image, size_t image_size,
              void *initramfs, size_t initramfs_size,
              const char *cmdline);

`kexec()` will load the kernel and initramfs into memory, but will not directly
boot them. To boot the loaded kernel, shut down the system. This can be
accomplished by pressing the power button, but can also be done more quickly
and reliably from userspace with the following sequence of system calls (this
kills userspace quickly but still does a controlled filesystem unmount):

    int evf = syscall(540, "SceSysCoreReboot");
    syscall(546, evf, 0x4000, 0);
    syscall(541, evf);
    // should be syscall(37, 1, 30) but only tested via kill symbol
    kill(1, 30);

Note that this software should be loaded into kernel memory space. If you are
running kernel code from userland mappings, you should either switch to kernel
mappings or compile and load this code as a separate blob that is loaded into
the kernel. While syscalls or exploit code may run properly from userland,
the shutdown hook will not, as it will be called from a different process
context.

## Features

`kernel_init()` will automatically find the Orbis OS kernel and resolve all
necessary symbols to work. There are no static symbol dependencies.

In addition to loading the user-supplied initramfs, `kexec` will locate the
Radeon firmware blobs inside Orbis OS, extract them, convert them to a format
suitable for Linux, and append them as an additional initramfs cpio image to
the existing initramfs. This avoids the need to distribute the Radeon firmware
blobs. The `radeon` module, when compiled into the kernel, will automatically
load this firmware on boot. Note however that most typical initramfs scripts
will wipe the initramfs contents while pivoting to the real system, so if you
compile `radeon` as a module you may not be able to access the firmware after
boot. To cover that case, add some code to your initramfs `/init` script to
copy the firmware to a tmpfs mounted on the real filesystem:

    # assuming real root FS is mounted on /mnt
    
    mkdir -p /mnt/lib/firmware/radeon
    mount -t tmpfs none /mnt/lib/firmware/radeon
    cp /lib/firmware/radeon/* /mnt/lib/firmware/radeon/
    
    # now switch_root to /mnt

This avoids having to permanently store copies of the Radeon firmware, which
isn't really necessary for most use cases.

There is significant debug logging available, which will appear on the system
UART. Most of the code relies on the kernel `printf` implementation, and
therefore you should patch out the UART output blanker to see it. The final
code that runs on the boot CPU before booting the kernel uses direct UART
writes and is not affected by the blanking feature of Orbis OS.

To debug the early symbol resolver code before it has had a chance to locate
the kernel's `printf` function, set the global variable `early_printf` to
the address of that function before calling `kernel_init()`.
