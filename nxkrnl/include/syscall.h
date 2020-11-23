#ifndef SYSCALL_H
#define SYSCALL_H

#include <interrupts.h>
#include <console.h>
#include <config.h>
#include <exec.h>
#include <task.h>
#include <system.h>

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
#define SYSCALL_REBOOT		10
#define SYSCALL_GETCHAR		11
#define SYSCALL_GET_TICK	12
#define SYSCALL_RESET_TICK	13
#define SYSCALL_MULTIBOOT	14

struct cpu_state* syscall(struct cpu_state* cpu);

#endif