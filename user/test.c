#include <stdint.h>

void _start(void)
{
    int i;
    for (i = 0; i < 9; i++) {
        asm("int $0x30" : : "a" (0), "b" ('0' + i));
    }

    while(1);
}