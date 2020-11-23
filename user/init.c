#include <stdint.h>
#include <syslib.h>

void _start(void){
	clrscr();
	setcolor(BACKGROUND_BLACK | FOREGROUND_DARKGRAY);
	kprintf("NextOS version %d Copyright (C) 2020 %s\n",kversion(), kvendor());
	kprintf("This program comes with ABSOLUTELY NO WARRANTY.\n");
	kprintf("This is free software, and you are welcome to redistribute it.\n\n");
	setcolor(BACKGROUND_BLACK | FOREGROUND_WHITE);
	exec("/terminal.bin");
	task_exit(0);
}