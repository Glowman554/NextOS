#include <syslib.h>
#include <stdint.h>
#include <desktop_defs.h>
#include <cmos_defs.h>

void tostring(char str[], int num){
	int i; 
	int rem;
	int len = 0;
	int n;
 
	n = num;
	while (n != 0){
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++){
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
}

void print_string(int driver_id, int window_id, int xoff, int yoff, char* str) {

	desktop_data_t data;
	data.function = DESKTOP_DRAW_CHAR;
	data.window_id = window_id;

	int len = strlen(str);
	for(int i = 0; i < len; i++){
		data.xoffset = xoff + 1 + (i * 8);
		data.yoffset = yoff;
		data._char = str[i];
		call_driver_handler(driver_id, &data);
	}
}

void _start(void){
	exec("/progs/desktop.bin");

	while(find_driver_by_name("desktop") == -1);

	int desktop_id = find_driver_by_name("desktop");
	int cmos_id = find_driver_by_name("cmos");
	desktop_data_t data;
	data.window_x = 100;
	data.window_y = 50;
	data.window_size_x = 120;
	data.window_size_y = 30;
	data.window_name = "Timer";
	data.function = DESKTOP_CREATE_WINDOW;

	int window_id = call_driver_handler(desktop_id, &data);
	
	claim_mouse_handlers();

	set_vga_color(VGA_BLACK, VGA_DARKGRAY);

	print_string(desktop_id, window_id, 0, 0, "Current Time:");

	while(1){
		cmos_data_t c_data;
		c_data.function = CMOS_FUNCTION_READH;
		int h = call_driver_handler(cmos_id, &c_data);
		c_data.function = CMOS_FUNCTION_READM;
		int m = call_driver_handler(cmos_id, &c_data);
		c_data.function = CMOS_FUNCTION_READS;
		int s = call_driver_handler(cmos_id, &c_data);
		
		char h_s[16];
		char m_s[16];
		char s_s[16];

		tostring(h_s, h);
		tostring(m_s, m);
		tostring(s_s, s);
		print_string(desktop_id, window_id, 0, 14, "0 :0 :0 ");
		print_string(desktop_id, window_id, 0, 14, h_s);
		print_string(desktop_id, window_id, 8 * 3, 14, m_s);
		print_string(desktop_id, window_id, 8 * 6, 14, s_s);

		reset_timer_tick();
		while(get_timer_tick() < 5);		
	}
	task_exit(0);
}
