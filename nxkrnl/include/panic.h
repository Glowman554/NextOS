#ifndef PANIC_H
#define PANIC_H

#include <console.h>
#include <interrupts.h>
#include <driver/vga.h>
#include <stdbool.h>

char* get_exception_name(int exc);
void kernel_yeet(struct cpu_state* cpu);

#endif