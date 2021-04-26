#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <io.h>
#include <keymap.h>
#include <string.h>
#include <multiboot.h>
#include <fs/fs.h>
#include <console.h>
#include <task.h>

#include <stdio.h>
#include <driver/serial.h>

void reboot();
uint32_t get_module_by_name(char* file);
void exec_file(char* file);


#endif