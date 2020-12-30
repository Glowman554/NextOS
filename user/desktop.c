#include <stdint.h>
#include <syslib.h>
#include <desktop_defs.h>

window_t windows[16];
int window_index = 0;

int strlen(char *src){
	int i = 0;
	while (*src++)
		i++;
	return i;
}

int create_window(int window_x, int window_y, int window_size_x, int window_size_y, char* name){
    for(int x = 0; x < window_size_x + 2; x++){
        for(int y = 0; y < 15; y++){
            setpixel(window_x + x, window_y - y, VGA_GREEN);
        }
        setpixel(window_x + x, window_y + window_size_y + 1, VGA_GREEN);
	}

    for(int y = 0; y < window_size_y + 2; y++){
        setpixel(window_x, window_y + y, VGA_GREEN);
        setpixel(window_x + window_size_x + 1, window_y + y, VGA_GREEN);
    }

    int len = strlen(name);
    for(int i = 0; i < len; i++){
        draw_char(name[i], window_x + 1 + (i * 8), window_y - 14);
    }

    window_t window;
    window.window_size_x = window_size_x;
    window.window_size_y = window_size_y;
    window.window_x = window_x;
    window.window_y = window_y;

    windows[window_index] = window;
    window_index++;
    return window_index - 1;
}

void draw_quad(int id, int xorig, int yorig, int size, int color){
    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            setpixel(x + xorig + windows[id].window_x + 1, y + yorig + windows[id].window_y + 1, color);
        }
    }
}

void drawCircle(int xc, int yc, int x, int y, int color) { 
    setpixel(xc+x, yc+y, color); 
    setpixel(xc-x, yc+y, color); 
    setpixel(xc+x, yc-y, color); 
    setpixel(xc-x, yc-y, color); 
    setpixel(xc+y, yc+x, color); 
    setpixel(xc-y, yc+x, color); 
    setpixel(xc+y, yc-x, color); 
    setpixel(xc-y, yc-x, color); 
}

void circleBres(int xc, int yc, int r, int color){ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawCircle(xc, yc, x, y, color); 
    while (y >= x){
        x++; 
        if (d > 0){ 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } else
            d = d + 4 * x + 6; 
        drawCircle(xc, yc, x, y, color); 
    }
}

int driver_handler(desktop_data_t* data){
    switch (data->function){
        case DESKTOP_CREATE_WINDOW:
            return create_window(data->window_x, data->window_y, data->window_size_x, data->window_size_y, data->window_name);
            break;
        case DESKTOP_DRAW_QUAD:
            draw_quad(data->window_id, data->xoffset, data->yoffset, data->size, data->color);
            break;
        case DESKTOP_DRAW_CIRCLE:
            circleBres(data->xoffset + windows[data->window_id].window_x + 1, data->yoffset + windows[data->window_id].window_y + 1, data->radius, data->color);
            break;
        case DESKTOP_DRAW_CHAR:
            draw_char(data->_char, data->xoffset + windows[data->window_id].window_x + 1, data->yoffset + windows[data->window_id].window_y + 1);
            break;

    }
	return -1;
}

void _start(void){
    init_vga();
    set_vga_color(VGA_GREEN, VGA_DARKGRAY);
    clrscr();

    for(int x = 0; x < 320; x++){
        for(int y = 0; y < 15; y++){
            setpixel(x, y, VGA_GREEN);
        }
	}

    set_vga_color(VGA_DARKGRAY, VGA_GREEN);
    kprintf("NextOS %d from %s", kversion(), kvendor());


	init_driver("desktop", (driver_handler_ptr) driver_handler);

	task_exit(0);
}