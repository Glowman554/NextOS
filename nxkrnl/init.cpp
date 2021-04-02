extern "C"{
	#include <gdt.h>
	#include <console.h>
	#include <string.h>
	#include <interrupts.h>
	#include <task.h>
	#include <mem.h>
	#include <driver/serial.h>
	#include <system.h>
	#include <fs/fs.h>
	#include <fs/initrd.h>
}

#include <driver/keyboard.h>
#include <driver/mouse.h>
#include <config.h>
#include <multiboot.h>
#include <pci.h>

#include <fe/fe_runner.h>

struct multiboot_info *pmb_info;

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void initialiseConstructors();
void initialiseConstructors(){
	for (constructor* i = &start_ctors;i != &end_ctors;++i)
		(*i)();
}

class InterruptKeyboardEventHandler : public KeyboardEventHandler{
	public:
		void KeyDown(char c){
			kb_handle(c);
		}
};

class InterruptMouseEventHandler : public MouseEventHandler {
	public:
		virtual void OnMouseMove(long x, long y) {
			mouse_handle_move(x, y);
		}

		virtual void OnMouseDown(uint8_t button) {
			mouse_handle_button(button);
		}
};

extern "C" void init(struct multiboot_info *mb_info){
	
	//init_serial();
	
	clrscr();
	kprintf("nxkrnl %d Loading...\n\n", VERSION);
	//kprintf("Reporting kernel version %d\n", VERSION);
	//kprintf("Reporting kernel vendor %s\n\n", VENDOR);
	
	pmb_info = mb_info;
	debug_write("Setting global multiboot pointer!");
	debug_write("Initializing memory!");
	pmm_init(mb_info);
	debug_write("Initializing GDT!");
	init_gdt();
	debug_write("Initializing interrupts!");
	init_intr();
	 
	DriverManager drvManager;

	debug_write("Adding keyboard driver!");
	InterruptKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard_driver(&kbhandler);
	drvManager.AddDriver(&keyboard_driver);

	debug_write("Adding keyboard mouse!");
	InterruptMouseEventHandler mshandler;
	MouseDriver mouse_driver(&mshandler);
	drvManager.AddDriver(&mouse_driver);
	
	PeripheralComponentInterconnectController PCIController;
	//kprintf("Found PCI Devices:\n");
	//PCIController.PrintDevices();
	debug_write("Activating all drivers!");
	drvManager.ActivateAll(false);
	
	debug_write("Initializing multitasking!");
	init_multitasking(mb_info);
	
	//asm volatile("int $0x1");

	debug_write("Running main.fe!");
	extern const char fe_main[];

	kprintf("\nRunnning main.fe now!\n");
	run_fe((char*) fe_main);

	debug_write("Loading autoexec!");
	exec_file(AUTOEXEC);

	while(1);
}