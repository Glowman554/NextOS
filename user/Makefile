TEST = test.o

CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc -I include
LDFLAGS = -melf_i386

test.bin: $(TEST)
	ld $(LDFLAGS) -Ttext=0x200000 -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

clean:
	rm *.o
	rm *.bin

.PHONY: clean