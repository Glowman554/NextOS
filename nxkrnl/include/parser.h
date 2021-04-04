#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdbool.h>
#include <config.h>
#include <console.h>

struct kernel_info {
	bool debug;
	bool fe;
	bool force;
	bool dump_syscall;
	bool auto_format;
	char* autoexec;
	char* format_label;
};

extern char parser_buffer[100][100];
extern struct kernel_info global_kernel_info;

struct kernel_info parse_arguments(char* args, char buffer[100][100]);

#endif