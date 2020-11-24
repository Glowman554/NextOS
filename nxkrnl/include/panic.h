#ifndef PANIC_H
#define PANIC_H

#include <console.h>
#include <interrupts.h>
#include <driver/vga.h>
#include <stdbool.h>


void print_exception(int intr);

#endif