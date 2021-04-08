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

#include <fe/fe_runner.h>

struct multiboot_info *pmb_info;
bool is_init_done = false;

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

DriverManager* global_driver_manager;

void write_file(char* driver_name, char* file_name, char* file_content) {
	AdvancedTechnologyAttachment* ata = (AdvancedTechnologyAttachment*) global_driver_manager->find_driver_by_name(driver_name);

	if(ata->is_presend()) {
		NextFS fs = NextFS(ata);
		if(fs.is_next_fs()) {
			char buffer[1000];
			sprintf(buffer, "Writing file %s to %s!", file_name, driver_name);
			debug_write(buffer);

			fs.new_text_file(file_name, file_content);
		}
	}
}

extern "C" void dump_kernel_panic(cpu_state* cpu) {
	char buffer1[200];
	char buffer2[4096];
	char buffer3[64];
	char buffer4[16];

	sprintf(buffer2, "Kernel panic -> %s\n", get_exception_name(cpu->intr));
	sprintf(buffer1, "eax: 0x%x, ebx: 0x%x, ecx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
	strcat(buffer2, buffer1);
	sprintf(buffer1, "esi: 0x%x, edi: 0x%x, ebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
	strcat(buffer2, buffer1);
	sprintf(buffer1, "intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
	strcat(buffer2, buffer1);
	sprintf(buffer1, "eip: 0x%x, cs 0x%x, eflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
	strcat(buffer2, buffer1);
	sprintf(buffer1, "esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);
	strcat(buffer2, buffer1);

	int id = find_driver_by_name("cmos");

	if(id != -1) { 
		struct cmos_data_t{
			int function;
		};

		cmos_data_t data;
		data.function = 0;
		int h = call_driver_handler(id, &data);
		data.function = 1;
		int m = call_driver_handler(id, &data);
		data.function = 2;
		int s = call_driver_handler(id, &data);

		sprintf(buffer4, "%d:%d:%d", h, m, s);
	} else {
		sprintf(buffer4, "null");
	}

	sprintf(buffer3, "%s.%d@%s.panic", VENDOR, VERSION, buffer4);

	write_file("ata0m", buffer3, buffer2);
	write_file("ata0s", buffer3, buffer2);
	write_file("ata1m", buffer3, buffer2);
	write_file("ata1s", buffer3, buffer2);
}

extern "C" void init(struct multiboot_info *mb_info){
	
	init_serial();

	clrscr();
	kprintf("nxkrnl %d Loading...\n\n", VERSION);
	//kprintf("Reporting kernel version %d\n", VERSION);
	//kprintf("Reporting kernel vendor %s\n\n", VENDOR);
	
	global_kernel_info = parse_arguments((char*) mb_info->mbs_cmdline, parser_buffer);
	
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
	drvManager.add_driver(&keyboard_driver);

	debug_write("Adding mouse driver!");
	InterruptMouseEventHandler mshandler;
	MouseDriver mouse_driver(&mshandler);
	drvManager.add_driver(&mouse_driver);

	debug_write("Adding pit driver!");
	ProgrammableIntervalTimerDriver pit_driver(5);
	drvManager.add_driver(&pit_driver);

	debug_write("Adding ata drivers!");
	AdvancedTechnologyAttachment ata0m(true, 0x1F0, "ata0m");
	AdvancedTechnologyAttachment ata0s(false, 0x1F0, "ata0s");
	AdvancedTechnologyAttachment ata1m(true, 0x170, "ata1m");
	AdvancedTechnologyAttachment ata1s(false, 0x170, "ata1s");

	drvManager.add_driver(&ata0m);
	drvManager.add_driver(&ata0s);
	drvManager.add_driver(&ata1m);
	drvManager.add_driver(&ata1s);
	
	PeripheralComponentInterconnectController PCIController;
	PCIController.print_devices();

	debug_write("Activating all drivers!");
	drvManager.activate_all(global_kernel_info.force);
	
	debug_write("Setting global driver manager pointer!");
	global_driver_manager = &drvManager;

	debug_write("Initializing multitasking!");
	init_multitasking(mb_info);
	
	//asm volatile("int $0x1");

	if(global_kernel_info.fe) {
		debug_write("Running main.fe!");
		extern const char fe_main[];

		kprintf("\nRunnning main.fe now!\n");
		run_fe((char*) fe_main);
	}

	debug_write("Loading autoexec!");
	exec_file(global_kernel_info.autoexec);

	debug_write("Setting is_init_done to true!");
	is_init_done = true;

	while(1);
}