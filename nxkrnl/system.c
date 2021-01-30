#include <system.h>

void reboot(){
	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	asm volatile ("hlt");
}

char getchar(){
	uint8_t in;
	char inbuff = 0;
	while(inbuff == 0){
		in = inb(0x60);
		inbuff = keymap_de(in);
	}
	return inbuff;
}

uint32_t get_module_by_name(char* file){
	if (pmb_info->mbs_mods_count != 0){
		struct multiboot_module* modules = pmb_info->mbs_mods_addr;
		
		int i;
		for(i = 0; i < pmb_info->mbs_mods_count; i++){
			if(strcmp(modules[i].cmdline, file)==0){
				return modules[i].mod_start;
			}
		}
	}
	return 0;
}

void exec_file(char* file){
	if (pmb_info->mbs_mods_count != 0){
		struct multiboot_module* modules = pmb_info->mbs_mods_addr;
		//kprintf("Loading file %s\n", file);
		
		int i;
		for(i = 0; i < pmb_info->mbs_mods_count; i++){
			//kprintf(0xf, "Found module %s at position %d with len %d\n", buf, i, strlen(modules[i].cmdline));
			if(strcmp(modules[i].cmdline, file)==0){
				init_elf((void*) modules[i].mod_start);
			}
		}
	}
}