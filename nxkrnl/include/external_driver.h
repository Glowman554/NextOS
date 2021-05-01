#ifndef EXTERNEL_DRIVER_H
#define EXTERNEL_DRIVER_H


#include <stdio.h>
#include <string.h>
#include <console.h>
#include <driver/serial.h>
#include <task.h>

typedef int (*driver_handler_ptr)(void*);

typedef struct {
	driver_handler_ptr driver_handler;
	char* name;
} driver_info_t;

int init_driver(char* name, driver_handler_ptr driver_handler, bool no_free);
int call_driver_handler(int id, void* data);
int find_driver_by_name(char* name);
#endif