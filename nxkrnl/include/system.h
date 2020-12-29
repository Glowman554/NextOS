#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <io.h>
#include <keymap.h>
#include <string.h>
#include <multiboot.h>
#include <fs/fs.h>
#include <console.h>

void reboot();
char getchar();
uint32_t get_module_by_name(char* file);

#endif