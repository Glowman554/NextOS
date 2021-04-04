#include <parser.h>

char parser_buffer[100][100];
struct kernel_info global_kernel_info;

struct kernel_info parse_arguments(char* args, char buffer[100][100]) {
	
	struct kernel_info kinfo;
	
	kinfo.debug = false;
	kinfo.fe = false;
	kinfo.force = false;
	kinfo.auto_format = false;
	kinfo.dump_syscall = false;
	kinfo.format_label = "uwu";
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
		} else if(strcmp(buffer[i], "--fe") == 0) {
			kinfo.fe = true;
		} else if(strcmp(buffer[i], "--force") == 0) {
			kinfo.force = true;
		} else if(strcmp(buffer[i], "--ds") == 0) {
			kinfo.dump_syscall = true;
		} else if(strcmp(buffer[i], "--af") == 0) {
			kinfo.auto_format = true;
		} else if(strcmp(buffer[i], "--fl") == 0) {
			i++;
			kinfo.format_label = buffer[i];
		} else if(strcmp(buffer[i], "--autoexec") == 0) {
			i++;
			kinfo.autoexec = buffer[i];
		} else {
			setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
			kprintf("Parser error can't continue booting!");
			while(1);
		}
	}
	
	return kinfo;
}