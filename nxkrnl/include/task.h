#ifndef TASK_H
#define TASK_H
#include <interrupts.h>
#include <stdint.h>
#include <mem.h>
#include <elf.h>

struct cpu_state* schedule(struct cpu_state* cpu);
void init_multitasking(struct multiboot_info* mb_info);
struct task* init_task(void* entry);
int init_elf(void* image);

#endif