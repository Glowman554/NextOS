#include <stdint.h>
#include <syslib.h>

void test_task(void){
	while(1){
		kprintf("TEST ");
	}
}

void _start(void){
	
	//init_task(test_task);
	
	while(1);
}