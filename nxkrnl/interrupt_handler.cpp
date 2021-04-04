#include <interrupt_handler.h>

InterruptHandler* handlers[256];


extern "C"{
	#include <console.h>
}

extern "C" void do_handle_interrupt(uint32_t interrupt_num){
	//kprintf("Interrupt 0x%x\n", interrupt_num);
	if(handlers[interrupt_num] != 0)
		handlers[interrupt_num]->handle();
}

InterruptHandler::InterruptHandler(uint32_t itrn){
	char buffer[100];
	sprintf(buffer, "Reisterring interrupt handler %d at 0x%x!", itrn, this);
	debug_write(buffer);
	int_num = itrn;
	handlers[itrn] = this;
}

InterruptHandler::~InterruptHandler(){
	if(handlers[int_num] == this)
		handlers[int_num] = 0;
}

void InterruptHandler::handle(){
	
}