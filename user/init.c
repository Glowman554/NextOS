#include <stdint.h>
#include <syslib.h>

void _start(void){
	kprintf("NextOS init\n");
	kprintf("Kernel Reports version: %d, vendor: %s\n", kversion(), kvendor());
	exec("/test.bin");
	task_exit(0);
}