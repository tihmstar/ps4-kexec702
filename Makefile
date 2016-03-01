CFLAGS := -march=btver2 -masm=intel -std=gnu11 -ffreestanding -fno-common \
	-fPIC -fomit-frame-pointer -nostdlib -nostdinc \
	-fno-asynchronous-unwind-tables \
	-Os -Wall -Werror -Wl,--build-id=none,-T,kexec.ld

SOURCES := kernel.c kexec.c linux_boot.c linux_thunk.S uart.c firmware.c

OBJS := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SOURCES)))
DEPS := $(OBJS) $(SOURCES) $(INCLUDES:%=$(INC_DIR)/%) Makefile kexec.ld

all: kexec.a

%.o: %.c *.h
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.S
	gcc -c $(CFLAGS) -o $@ $<

kexec.a: $(OBJS)
	ar -rc $@ $(OBJS)

clean:
	rm -f kexec.a $(OBJS)
