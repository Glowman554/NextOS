#ifndef SYSCALL_H
#define SYSCALL_H

#include <interrupts.h>
#include <console.h>
#include <config.h>

#define SYSCALL_PUTC		0
#define SYSCALL_PUTS		1
#define SYSCALL_PUTN		2
#define SYSCALL_CLRSCR		3
#define SYSCALL_SETCOLOR	4
#define SYSCALL_KVERSION	5
#define SYSCALL_KVENDOR		6

struct cpu_state* syscall(struct cpu_state* cpu);

#endif