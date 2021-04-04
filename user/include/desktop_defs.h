#ifndef DESKTOP_DEFS_H
#define DESKTOP_DEFS_H

#define DESKTOP_CREATE_WINDOW 0
#define DESKTOP_DRAW_QUAD 1
#define DESKTOP_DRAW_CIRCLE 2
#define DESKTOP_DRAW_CHAR 3

typedef struct {
	int function;
	int window_x;
	int window_y;
	int window_size_x;
	int window_size_y;
	char* window_name;

	int window_id;
	int xoffset;
	int yoffset;
	int size;
	int color;
	int radius;

	char _char;
} desktop_data_t;

typedef struct {
	int window_x;
	int window_y;
	int window_size_x;
	int window_size_y;
} window_t;

#endif