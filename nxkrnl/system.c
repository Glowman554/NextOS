#include <system.h>

void reboot(){
	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	asm volatile ("hlt");
}

char getchar(){
	uint8_t in;
	char inbuff = 0;
	while(inbuff == 0){
		in = inb(0x60);
		inbuff = keymap_de(in);
	}
	return inbuff;
}