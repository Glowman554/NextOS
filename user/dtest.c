#include <syslib.h>
#include <stdint.h>
#include <desktop_defs.h>

void mouse_test_handler(int button, int x, int y) {
    kprintf("B: %d, X: %d, Y: %d\n", button, x, y);
}

void _start(void){
    exec("/progs/desktop.bin");

    reset_timer_tick();
	while(get_timer_tick() < 100);

    claim_mouse_handlers();
    set_mouse_handler(&mouse_test_handler);

    int id = find_driver_by_name("desktop");
    desktop_data_t data;
    data.window_x = 20;
    data.window_y = 35;
    data.window_size_x = 200;
    data.window_size_y = 100;
    data.window_name = "dtest";
    data.function = DESKTOP_CREATE_WINDOW;

    int window_id = call_driver_handler(id, &data);

    data.xoffset = 0;
    data.yoffset = 0;
    data.size = 20;
    data.color = VGA_BLUE;
    data.window_id = window_id;
    data.function = DESKTOP_DRAW_QUAD;
    call_driver_handler(id, &data);

    data.xoffset = 40;
    data.yoffset = 40;
    data.radius = 10;
    data.function = DESKTOP_DRAW_CIRCLE;
    call_driver_handler(id, &data);

    set_vga_color(VGA_GREEN, VGA_DARKGRAY);

    data.xoffset = 30;
    data.yoffset = 0;
    data._char = 'X';
    data.function = DESKTOP_DRAW_CHAR;
    call_driver_handler(id, &data);
	
	task_exit(0);
}