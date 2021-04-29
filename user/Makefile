INIT = init.o syslib.o
TERMINAL = terminal.o syslib.o
DESKTOP = desktop.o syslib.o
DTEST = dtest.o syslib.o
CMOS = cmos.o syslib.o driverlib.o
TIMER = timer.o syslib.o
BF = bf.o syslib.o
REBOOT = reboot.o syslib.o

CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -I include -fpic
LDFLAGS = -melf_i386 -pic

all: init.bin terminal.bin desktop.bin cmos.drv dtest.bin timer.bin bf.drv reboot.bin
	
init.bin: $(INIT)
	ld $(LDFLAGS) -o $@ $^
	
terminal.bin: $(TERMINAL)
	ld $(LDFLAGS) -o $@ $^

desktop.bin: $(DESKTOP)
	ld $(LDFLAGS) -o $@ $^

cmos.drv: $(CMOS)
	ld $(LDFLAGS) -o $@ $^

dtest.bin: $(DTEST)
	ld $(LDFLAGS) -o $@ $^

timer.bin: $(TIMER)
	ld $(LDFLAGS) -o $@ $^

bf.drv: $(BF)
	ld $(LDFLAGS) -o $@ $^

reboot.bin: $(REBOOT)
	ld $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo CC $^
	@gcc $(CFLAGS) -c -o $@ $^

clean:
	rm *.o
	rm *.bin
	rm *.drv

.PHONY: clean