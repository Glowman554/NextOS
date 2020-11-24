#include <stdint.h>
#include <syslib.h>

void test_task(void){
	while(1){
		kprintf("TEST ");
	}
}

void _start(void){
	//exec("/terminal.bin");
	init_vga();
	init_task(test_task);
	
	task_exit(0);
}