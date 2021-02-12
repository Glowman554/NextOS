#include <syslib.h>
#include <stdint.h>
#include <desktop_defs.h>

void mouse_test_handler(int button, int x, int y) {
    if(x > 20 && x < 40 && y > 40 && y < 60) {
        reboot();
    }
}

//x 20 y 40
//x 40 60

void _start(void){
    exec("/progs/desktop.bin");

    reset_timer_tick();
	while(get_timer_tick() < 100);

    int id = find_driver_by_name("desktop");
    desktop_data_t data;
    data.window_x = 20;
    data.window_y = 35;
    data.window_size_x = 200;
    data.window_size_y = 100;
    data.window_name = "Visual Reboot";
    data.function = DESKTOP_CREATE_WINDOW;

    int window_id = call_driver_handler(id, &data);

    data.xoffset = 10;
    data.yoffset = 10;
    data.radius = 10;
    data.color = VGA_RED;
    data.window_id = window_id;
    data.function = DESKTOP_DRAW_CIRCLE;
    call_driver_handler(id, &data);

    claim_mouse_handlers();
    set_mouse_handler(&mouse_test_handler);
	
	task_exit(0);
}