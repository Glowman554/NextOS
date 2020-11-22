#ifndef EXEC_H
#define EXEC_H

#include <task.h>
#include <multiboot.h>
#include <console.h>
#include <string.h>

void exec_file(char* file);
void list_files();

#endif