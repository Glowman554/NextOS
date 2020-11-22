#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include <stdint.h>

extern "C" void handle_interrupt(uint32_t interrupt_num);

class InterruptHandler{
	protected:
		uint32_t int_num;
		InterruptHandler(uint32_t itrn);
		~InterruptHandler();
	public:
		virtual void Handle();
};


#endif