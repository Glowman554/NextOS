#include <syslib.h>
#include <stdint.h>
#include <stdbool.h>

void print_copyright(){
	setcolor(BACKGROUND_BLACK | FOREGROUND_DARKGRAY);
	kprintf("NextOS version %d Copyright (C) 2020 %s\n",kversion(), kvendor());
	kprintf("This program comes with ABSOLUTELY NO WARRANTY.\n");
	kprintf("This is free software, and you are welcome to redistribute it.\n\n");
	setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
}

void _start(){
	bool reading = true;
    
	int len = 0;
	char in[100];
	
    
	while(1){
		kprintf(">>");
		while(reading){
			in[len] = getchar();
			kprintf("%c", in[len]);
			if(in[len] == 10){
				in[len] = '\0';
				reading = false;
			}else{
				len++;
			}
			
			reset_timer_tick();
			while(get_timer_tick() < 3);
		}
		
		if(strcmp(in, "reboot")==0) reboot();
		if(strcmp(in, "debugerr")==0) task_exit(1);
		if(strcmp(in, "uname")==0) kprintf("Currently running kernel version %d\n", kversion());
		if(strcmp(in, "about")==0) print_copyright();
		if(strcmp(in, "clear")==0) clrscr();
		
		if(strcmp(in, "help")==0){
			kprintf("Aviable Commands:\n");
			kprintf("help\n");
			kprintf("reboot\n");
			kprintf("debugerr\n");
			kprintf("uname\n");
			kprintf("about\n");
			kprintf("clear\n");
		}
		
		if(in[len-1] == 'n' && in[len-2] == 'i' && in[len-3] == 'b' && in[len-4] == '.'){
			exec(in);
			task_exit(0);
		}
		
		reading = true;
		len = 0;
	}
}