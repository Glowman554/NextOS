TEST = test.o syslib.o
INIT = init.o syslib.o
TERMINAL = terminal.o syslib.o

CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -I include
LDFLAGS = -melf_i386

all: test.bin init.bin terminal.bin
	
init.bin: $(INIT)
	ld $(LDFLAGS) -Ttext=0x200000 -o $@ $^
	
terminal.bin: $(TERMINAL)
	ld $(LDFLAGS) -Ttext=0x300000 -o $@ $^
	
test.bin: $(TEST)
	ld $(LDFLAGS) -Ttext=0x400000 -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

clean:
	rm *.o
	rm *.bin

.PHONY: clean