extern "C"{
	#include <gdt.h>
	#include <console.h>
	#include <interrupts.h>
}

#include <interrupt_handler.h>

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void initialiseConstructors();
void initialiseConstructors()
{
  for (constructor* i = &start_ctors;i != &end_ctors;++i)
    (*i)();
}

class CallBack : public InterruptHandler{
	public:
		CallBack() : InterruptHandler(0x20) {
			
		}
		~CallBack(){
			
		}
		
		void Handle(){
			kprintf("Interrupt\n");
		}
};

extern "C" void init(){
	
	
	clrscr();
	kprintf("Hello Kernel World\n");
	kprintf("Init gdt\n");
	init_gdt();
	kprintf("Init intr\n");
	init_intr();
	CallBack c;
	while(1);
}