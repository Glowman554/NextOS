#include <exec.h>


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