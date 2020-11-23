#ifndef SYSCALL_H
#define SYSCALL_H

#include <interrupts.h>
#include <console.h>
#include <config.h>
#include <exec.h>
#include <task.h>

#define SYSCALL_PUTC		0
#define SYSCALL_PUTS		1
#define SYSCALL_PUTN		2
#define SYSCALL_CLRSCR		3
#define SYSCALL_SETCOLOR	4
#define SYSCALL_KVERSION	5
#define SYSCALL_KVENDOR		6
#define SYSCALL_EXEC		7
#define SYSCALL_INIT_TASK	8
#define SYSCALL_TASK_EXIT	9

struct cpu_state* syscall(struct cpu_state* cpu);

#endif