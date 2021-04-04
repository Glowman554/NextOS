SRCS = $(shell find -name '*.[cSn]')
CPPSRC = $(shell find -name '*.cpp')
OBJS = $(addsuffix .o,$(basename $(SRCS) $(CPPSRC)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -Iinclude -Wno-nonnull-compare
CPPFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -Iinclude -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
LDFLAGS = -melf_i386 -Tlink.ld

nxkrnl: $(OBJS)
	@echo LD $^
	@$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo CC $^
	@$(CC) $(CFLAGS) -c -o $@ $^
	
%.o: %.cpp
	@echo CPP $^
	@$(CC) $(CPPFLAGS) -c -o $@ $^

%.o: %.S
	@echo AS $^
	@$(CC) $(ASFLAGS) -c -o $@ $^

%.o: %.n
	@echo NASM $^
	@nasm -f elf32 -o $@ $^

run: nxkrnl
	qemu-system-i386.exe -kernel nxkrnl

clean:
	rm $(OBJS)

.PHONY: clean