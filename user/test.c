#include <stdint.h>
#include <syslib.h>

void test_task(void){
	while(1){
		kprintf("TEST ");
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

void print_initrd_file(char* file){
	int i = 0;
	struct dirent *node = 0;
	while((node = initrd_readdir(i)) != 0){
		if(strcmp(file, node->name) == 0){
			fs_node_t *fsnode = initrd_finddir(node->name);
			if((fsnode->flags & 0x7) == FS_DIRECTORY)
				kprintf("\n(driectory)\n");
			else{
				initrd_read(fsnode, 0, 50);
				uint8_t* buf = get_buffer();
				kprintf((char*) buf);
			}
		}
		i++;
	}
}

void _start(void){

	//init_task(test_task);
	
	load_initrd("/initrd.img");
	list_initrd_files();
	print_initrd_file("vaspel.txt");
	
	task_exit(0);
}