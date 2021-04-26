#include <info_line.h>

void drawinfo() {

	scrole = false;

	uint32_t old_color = color;

	char buffer[75];

	sprintf(buffer, "Running proceses: %d, Timer tick: 0x%x, Current pid: %d", proccount, get_timer_tick(), task_states[current_task].pid);

	int old_x = getx();
	int old_y = gety();

	sety(24);
	setx(0);

	setcolor(BACKGROUND_LIGHTGRAY | FOREGROUND_WHITE);

	for(int i = 0; i < 80; i++) {
		kputc(' ');
	}

	setx(0);

	kputs(buffer);

	sety(old_y);
	setx(old_x);

	setcolor(old_color);

	scrole = true;
}