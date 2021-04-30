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
	#include <parser.h>
	#include <external_driver.h>
}

#include <driver/keyboard.h>
#include <driver/mouse.h>
#include <driver/pit.h>
#include <driver/ata.h>
#include <config.h>
#include <multiboot.h>
#include <pci.h>

char debug_write_buffer[1000];


struct multiboot_info *pmb_info;
bool is_init_done = false;

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void initialise_constructors();
void initialise_constructors(){
	for (constructor* i = &start_ctors;i != &end_ctors;++i)
		(*i)();
}

class InterruptKeyboardEventHandler : public KeyboardEventHandler{
	public:
		void key_down(char c){
			kb_handle(c);
		}
};

class InterruptMouseEventHandler : public MouseEventHandler {
	public:
		virtual void on_mouse_move(long x, long y) {
			mouse_handle_move(x, y);
		}

		virtual void on_mouse_down(uint8_t button) {
			mouse_handle_button(button);
		}
};

DriverManager* global_driver_manager;

extern "C" void init(struct multiboot_info *mb_info){
	
	init_serial();

	clrscr();
	kprintf("nxkrnl %d Loading...\n\n", VERSION);
	//kprintf("Reporting kernel version %d\n", VERSION);
	//kprintf("Reporting kernel vendor %s\n\n", VENDOR);
	
	global_kernel_info = parse_arguments((char*) mb_info->mbs_cmdline, parser_buffer);
	
	pmb_info = mb_info;
	debug_write_lame("Setting global multiboot pointer!");
	debug_write_lame("Initializing memory!");
	pmm_init(mb_info);
	debug_write_lame("Initializing GDT!");
	init_gdt();
	debug_write_lame("Initializing interrupts!");
	init_intr();
	 
	DriverManager drv_manager;

	debug_write_lame("Adding keyboard driver!");
	InterruptKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard_driver(&kbhandler);
	drv_manager.add_driver(&keyboard_driver);

	debug_write_lame("Adding mouse driver!");
	InterruptMouseEventHandler mshandler;
	MouseDriver mouse_driver(&mshandler);
	drv_manager.add_driver(&mouse_driver);

	debug_write_lame("Adding pit driver!");
	ProgrammableIntervalTimerDriver pit_driver(5);
	drv_manager.add_driver(&pit_driver);

	debug_write_lame("Adding ata drivers!");
	AdvancedTechnologyAttachment ata0m(true, 0x1F0, "ata0m");
	AdvancedTechnologyAttachment ata0s(false, 0x1F0, "ata0s");
	AdvancedTechnologyAttachment ata1m(true, 0x170, "ata1m");
	AdvancedTechnologyAttachment ata1s(false, 0x170, "ata1s");

	drv_manager.add_driver(&ata0m);
	drv_manager.add_driver(&ata0s);
	drv_manager.add_driver(&ata1m);
	drv_manager.add_driver(&ata1s);
	
	PeripheralComponentInterconnectController pci_controller;
	pci_controller.print_devices();

	debug_write_lame("Activating all drivers!");
	drv_manager.activate_all(global_kernel_info.force);
	
	debug_write_lame("Setting global driver manager pointer!");
	global_driver_manager = &drv_manager;

	debug_write_lame("Initializing multitasking!");
	init_multitasking(mb_info);

	if(global_kernel_info.vga) {
		debug_write_lame("Setting vga mode!");
		init_vga();
	}

	debug_write_lame("Loading autoexec!");
	exec_file(global_kernel_info.autoexec);

	//write_file("ata0m", "hello.txt", "hello world");

	debug_write_lame("Setting is_init_done to true!");
	is_init_done = true;

	while(1);
}