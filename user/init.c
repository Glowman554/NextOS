#include <stdint.h>
#include <syslib.h>

void _start(void){
	kprintf("NextOS init\n");
	kprintf("Kernel Reports version: %d, vendor: %s", kversion(), kvendor());
    while(1);
}