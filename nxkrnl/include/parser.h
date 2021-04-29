#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdbool.h>
#include <config.h>
#include <console.h>

struct kernel_info {
	bool debug;
	bool force;
	bool vga;
	char* autoexec;
};

extern char parser_buffer[100][100];
extern struct kernel_info global_kernel_info;

struct kernel_info parse_arguments(char* args, char buffer[100][100]);

#endif