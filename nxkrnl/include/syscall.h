#ifndef SYSCALL_H
#define SYSCALL_H

#include <interrupts.h>
#include <console.h>

#define SYSCALL_PUTC 0

struct cpu_state* syscall(struct cpu_state* cpu);

#endif