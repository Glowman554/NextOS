#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <multiboot.h>
#include <string.h>

#include <stdio.h>
#include <driver/serial.h>

#define NULL ((void*) 0)

#define BITMAP_SIZE 32768

void pmm_init(struct multiboot_info* mb_info);
void* pmm_alloc(void);
void pmm_free(void* page);


#endif