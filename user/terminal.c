#include <syslib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmos_defs.h>

struct multiboot_info *mb_info;

void print_copyright(){
	setcolor(BACKGROUND_BLACK | FOREGROUND_DARKGRAY);
	kprintf("NextOS version %d Copyright (C) 2020, 2021 %s\n",kversion(), kvendor());
	kprintf("This program comes with ABSOLUTELY NO WARRANTY.\n");
	kprintf("This is free software, and you are welcome to redistribute it.\n\n");
	setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
}

void list_files(){
	if (mb_info->mbs_mods_count != 0) {
		struct multiboot_module* modules = mb_info->mbs_mods_addr;
		int i;

		for (i = 0; i < mb_info->mbs_mods_count; i++) {
			kprintf("Found module %s\n", modules[i].cmdline);
		}
	}
}

void list_initrd_files(){
	int i = 0;
	struct dirent *node = 0;
	while((node = initrd_readdir(i)) != 0){
		kprintf("Found file %s\n", node->name);
		i++;
	}
}




void print_time(){
	int id = find_driver_by_name("cmos");

	cmos_data_t data;
	data.function = CMOS_FUNCTION_READH;
	int h = call_driver_handler(id, &data);
	data.function = CMOS_FUNCTION_READM;
	int m = call_driver_handler(id, &data);
	data.function = CMOS_FUNCTION_READS;
	int s = call_driver_handler(id, &data);

	kprintf("%d:%d:%d\n", h, m, s);
}

void bf(){

	kprintf("[.bf] >>> ");
	char* in;
	claim_kb_handler();
	in = get_input();

	kprintf("\n\n");

	int i = 0;
	struct dirent *node = 0;
	while((node = initrd_readdir(i)) != 0){
		if(strcmp(in, node->name) == 0){
			fs_node_t *fsnode = initrd_finddir(node->name);
			if((fsnode->flags & 0x7) == FS_DIRECTORY)
				kprintf("\n(driectory)\n");
			else{
				initrd_read(fsnode, 0, 65536);
				uint8_t* buf = get_buffer();
				int id = find_driver_by_name("bf");
				call_driver_handler(id, (char*) buf);
			}
		}
		i++;
	}

	kprintf("\n\n");
}

void _start(){
	claim_kb_handler();
	char* in;

	int len;
	
	mb_info = get_mb_ptr();

	claim_mouse_handlers();
	
    
	while(1){
		kprintf(">> ");
		in = get_input();
		
		if(strcmp(in, "reboot")==0) reboot();
		if(strcmp(in, "debugerr")==0) task_exit(1);
		if(strcmp(in, "uname")==0) kprintf("Currently running kernel version %d\n", kversion());
		if(strcmp(in, "about")==0) print_copyright();
		if(strcmp(in, "clear")==0) clrscr();
		if(strcmp(in, "ls")==0){
			kprintf("\nMultiboot:\n\n");
			list_files();
			kprintf("\nInitrd:\n\n");
			list_initrd_files();
		}
		if(strcmp(in, "vga-init")==0) init_vga();
		if(strcmp(in, "time")==0) print_time();

		if(strcmp(in, "bf")==0) bf();
		
		if(strcmp(in, "help")==0){
			kprintf("Aviable Commands:\n");
			kprintf("help\n");
			kprintf("reboot\n");
			kprintf("debugerr\n");
			kprintf("uname\n");
			kprintf("about\n");
			kprintf("clear\n");
			kprintf("ls\n");
			kprintf("vga-init\n");
			kprintf("time\n");
			kprintf("bf\n");
		}

		len = strlen(in);

		if(in[len-1] == 'n' && in[len-2] == 'i' && in[len-3] == 'b' && in[len-4] == '.'){
			exec(in);
			task_exit(0);
		}
	}
}