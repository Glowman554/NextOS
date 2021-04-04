#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <io.h>
#include <console.h>
#include <panic.h>
#include <driver/vga.h>

#include <driver/serial.h>
#include <stdio.h>

#define IDT_FLAG_INTERRUPT_GATE 0xe
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING3 0x60
#define IDT_ENTRIES 256

struct cpu_state {
	// Von Hand gesicherte Register
	uint32_t   eax;
	uint32_t   ebx;
	uint32_t   ecx;
	uint32_t   edx;
	uint32_t   esi;
	uint32_t   edi;
	uint32_t   ebp;

	uint32_t   intr;
	uint32_t   error;

	// Von der CPU gesichert
	uint32_t   eip;
	uint32_t   cs;
	uint32_t   eflags;
	uint32_t   esp;
	uint32_t   ss;
};

void init_intr();
void reset_timer_tick();
uint32_t get_timer_tick();

#endif