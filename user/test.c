#include <stdint.h>

void _start(void){
    int i;
    for (i = 0; i < 9; i++) {
        asm("int $0x30" : : "a" (0), "b" ('0' + i));
    }
	char str[] = "TestString\n";
	
	asm("int $0x30" : : "a" (3));
	asm("int $0x30" : : "a" (1), "b" (str));
	asm("int $0x30" : : "a" (2), "b" (0xff), "c" (0xf));
	asm("int $0x30" : : "a" (2), "b" (10), "c" (0xa));
    while(1);
}