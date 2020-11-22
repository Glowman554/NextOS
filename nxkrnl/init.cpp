extern "C"{
	#include <gdt.h>
	#include <console.h>
	#include <interrupts.h>
	#include <task.h>
	#include <mem.h>
}

#include <driver/driver.h>
#include <driver/keyboard.h>
#include <config.h>
#include <multiboot.h>

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void initialiseConstructors();
void initialiseConstructors()
{
  for (constructor* i = &start_ctors;i != &end_ctors;++i)
    (*i)();
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
	public:
		void KeyDown(char c){
			kputc(c);
		}
};



extern "C" void init(struct multiboot_info *mb_info){
	
	
	clrscr();
	kprintf("nxkrnl %d Loading...\n", VERSION);
	kprintf("Reporting kernel version %d\n", VERSION);
	kprintf("Reporting kernel vendor %s\n", VENDOR);
	kprintf("\nStage 1 fundamentals\n");
	kprintf("Init Memory\n");
	pmm_init(mb_info);
	kprintf("Init global descriptor tabel\n");
	init_gdt();
	kprintf("Init interupts\n");
	init_intr();
	
	kprintf("\nStage 2 devices\n");
	kprintf("Creating DriverManager\n"); 
	DriverManager drvManager;
	kprintf("Building KeyboardDriver");
	PrintfKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard_driver(&kbhandler);
	drvManager.AddDriver(&keyboard_driver);
	kprintf("Activating All Driver\n");
	drvManager.ActivateAll();
	
	kprintf("\nStage 2 advanced\n");
	kprintf("Init Multitasking\n");
	init_multitasking();
	
	//asm volatile("int $0x1");
	
	while(1);
}