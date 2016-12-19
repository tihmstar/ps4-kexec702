CFLAGS=$(CFLAG)
CFLAGS += -march=btver2 -masm=intel -std=gnu11 -ffreestanding -fno-common \
	-fPIE -pie -fomit-frame-pointer -nostdlib -nostdinc \
	-fno-asynchronous-unwind-tables \
	-Os -Wall -Werror -Wl,--build-id=none,-T,kexec.ld,--nmagic \
	-mcmodel=small -mno-red-zone

SOURCES := kernel.c kexec.c linux_boot.c linux_thunk.S uart.c firmware.c

OBJS := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SOURCES)))
DEPS := $(OBJS) $(SOURCES) $(INCLUDES:%=$(INC_DIR)/%) Makefile kexec.ld

all: libkexec.a kexec.bin

%.o: %.c *.h
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.S
	gcc -c $(CFLAGS) -o $@ $<

libkexec.a: $(OBJS)
	ar -rc $@ $(OBJS)

kexec.elf: libkexec.a kexec.ld
	gcc $(CFLAGS) -o $@ libkexec.a

%.bin: %.elf
	objcopy -O binary $< $@

clean:
	rm -f libkexec.a kexec.elf kexec.bin $(OBJS)
