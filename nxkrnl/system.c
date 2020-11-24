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

void list_initrd_files(){
	int i = 0;
	struct dirent *node = 0;
	while((node = readdir_fs(fs_root, i)) != 0){
		kprintf("Found file %s\n", node->name);
		i++;
	}
}

void print_initrd_file(char* file){
	int i = 0;
	struct dirent *node = 0;
	while((node = readdir_fs(fs_root, i)) != 0){
		if(strcmp(file, node->name) == 0){
			fs_node_t *fsnode = finddir_fs(fs_root, node->name);
			if((fsnode->flags & 0x7) == FS_DIRECTORY)
				kprintf("\n(driectory)\n");
			else{
				char buf[4096];
				read_fs(fsnode, 0, 4096, (uint8_t*) buf);
				kprintf(buf);
			}
		}
		i++;
	}
}