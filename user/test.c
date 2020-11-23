#include <stdint.h>
#include <syslib.h>

void test_task(void){
	while(1){
		kprintf("TEST ");
	}
}

void _start(void){
	
	char c = getchar();
	kprintf("%d\n", c - 48);
	
	//exec("/terminal.bin");
	init_task(test_task);
	
	task_exit(0);
}