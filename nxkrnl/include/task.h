#ifndef TASK_H
#define TASK_H
#include <interrupts.h>
#include <stdint.h>
#include <mem.h>

struct cpu_state* schedule(struct cpu_state* cpu);
void init_multitasking();

#endif