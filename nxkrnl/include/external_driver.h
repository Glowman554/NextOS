#ifndef EXTERNEL_DRIVER_H
#define EXTERNEL_DRIVER_H

#include <string.h>

typedef int (*driver_handler_ptr)(void*);

typedef struct {
	driver_handler_ptr driver_handler;
	char* name;
} driver_info_t;

int init_driver(char* name, driver_handler_ptr driver_handler);
int call_driver_handler(int id, void* data);
int find_driver_by_name(char* name);
#endif