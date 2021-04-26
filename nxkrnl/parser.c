#include <parser.h>

char parser_buffer[100][100];
struct kernel_info global_kernel_info;

struct kernel_info parse_arguments(char* args, char buffer[100][100]) {
	
	struct kernel_info kinfo;
	
	kinfo.debug = false;
	kinfo.force = false;
	kinfo.autoexec = AUTOEXEC;
	
	int count = 0;
	
	int j = 0;
	
	for(int i = 0; i <= strlen(args); i++) { 
		if(args[i] == ' ' || args[i] == '\0') {	
			buffer[count][j] = '\0';
			count++;
			j = 0;
		} else {
			buffer[count][j] = args[i];
			j++;
		}
	}
	
	for(int i = 0; i < count; i++) {
		if(i == 0) {
			continue;
		}
		
		if(strcmp(buffer[i], "--debug") == 0) {
			kinfo.debug = true;
		} else if(strcmp(buffer[i], "--force") == 0) {
			kinfo.force = true;
		} else if(strcmp(buffer[i], "--autoexec") == 0) {
			i++;
			kinfo.autoexec = buffer[i];
		} else {
			uint32_t tmp = color;
			setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
			kprintf("Parser error at %d!\n", i);
			kprintf("    -> %s\n", buffer[i]);
			setcolor(tmp);
		}
	}
	
	return kinfo;
}