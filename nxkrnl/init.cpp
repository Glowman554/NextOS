extern "C"{
	#include <gdt.h>
	#include <console.h>
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

class MouseToConsole : public MouseEventHandler {
	private:
		int8_t x, y;
	public:
		
		MouseToConsole() {
			uint16_t* VideoMemory = (uint16_t*)0xb8000;
			x = 40;
			y = 12;
			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
								| (VideoMemory[80*y+x] & 0xF000) >> 4
								| (VideoMemory[80*y+x] & 0x00FF);        
		}
		
		virtual void OnMouseMove(long xoffset, long yoffset) {
			//kprintf("X: %d, Y: %d\n", xoffset, yoffset);
			xoffset = xoffset;
			yoffset = yoffset;
			static uint16_t* VideoMemory = (uint16_t*)0xb8000;
			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
								| (VideoMemory[80*y+x] & 0xF000) >> 4
								| (VideoMemory[80*y+x] & 0x00FF);

			x += xoffset;
			if(x >= 80) x = 79;
			if(x < 0) x = 0;
			y += yoffset;
			if(y >= 25) y = 24;
			if(y < 0) y = 0;

			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
								| (VideoMemory[80*y+x] & 0xF000) >> 4
								| (VideoMemory[80*y+x] & 0x00FF);
		}
    
};



extern "C" void init(struct multiboot_info *mb_info){
	
	if(SERIAL_DEBUG) init_serial();
	
	clrscr();
	kprintf("nxkrnl %d Loading...\n", VERSION);
	//kprintf("Reporting kernel version %d\n", VERSION);
	//kprintf("Reporting kernel vendor %s\n\n", VENDOR);
	
	pmb_info = mb_info;
	pmm_init(mb_info);
	init_gdt();
	init_intr();
	 
	DriverManager drvManager;
	InterruptKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard_driver(&kbhandler);
	MouseToConsole mhandler;
	MouseDriver mouse_driver(&mhandler);
	drvManager.AddDriver(&keyboard_driver);
	drvManager.AddDriver(&mouse_driver);
	PeripheralComponentInterconnectController PCIController;
	//kprintf("Found PCI Devices:\n");
	//PCIController.PrintDevices();
	drvManager.ActivateAll();
	
	init_multitasking(mb_info);
	
	//asm volatile("int $0x1");
	
	exec_file(AUTOEXEC);
	while(1);
}