extern "C"{
	#include <gdt.h>
	#include <console.h>
	#include <interrupts.h>
	#include <task.h>
	#include <exec.h>
	#include <mem.h>
}

#include <driver/driver.h>
#include <driver/keyboard.h>
#include <config.h>
#include <multiboot.h>
#include <pci.h>

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void initialiseConstructors();
void initialiseConstructors(){
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
	kprintf("Reporting kernel vendor %s\n\n", VENDOR);
	
	pmb_info = mb_info;
	pmm_init(mb_info);
	init_gdt();
	init_intr();
	 
	DriverManager drvManager;
	//PrintfKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard_driver(0);
	drvManager.AddDriver(&keyboard_driver);
	PeripheralComponentInterconnectController PCIController;
	//kprintf("Found PCI Devices:\n");
	//PCIController.PrintDevices();
	drvManager.ActivateAll();
	
	init_multitasking(mb_info);
	
	//asm volatile("int $0x1");
	if(LIST_FILES_ON_BOOT)
		list_files();
	exec_file(AUTOEXEC);
	while(1);
}