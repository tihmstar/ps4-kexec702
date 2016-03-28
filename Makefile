ifdef DDO_NOT_REMAP_RWX
DO_NOT_REMAP_RWX := -DDO_NOT_REMAP_RWX
endif

CFLAGS := -march=btver2 -masm=intel -std=gnu11 -ffreestanding -fno-common \
	-fPIC -fomit-frame-pointer -nostdlib -nostdinc \
	-fno-asynchronous-unwind-tables \
	-Os -Wall -Werror -Wl,--build-id=none,-T,kexec.ld,--nmagic $(DO_NOT_REMAP_RWX)

SOURCES := kernel.c kexec.c linux_boot.c linux_thunk.S uart.c firmware.c

OBJS := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SOURCES)))
DEPS := $(OBJS) $(SOURCES) $(INCLUDES:%=$(INC_DIR)/%) Makefile kexec.ld

all: kexec.a kexec.bin

%.o: %.c *.h
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.S
	gcc -c $(CFLAGS) -o $@ $<

kexec.a: $(OBJS)
	ar -rc $@ $(OBJS)

kexec.elf: kexec.a kexec.ld
	gcc $(CFLAGS) -o $@ kexec.a

%.bin: %.elf
	objcopy -O binary $< $@

clean:
	rm -f kexec.a kexec.elf kexec.bin $(OBJS)
