#include <syscall.h>

struct cpu_state* syscall(struct cpu_state* cpu){
	switch (cpu->eax) {
		case SYSCALL_PUTC:
			kprintf("%c", cpu->ebx);
			break;
	}

	return cpu;
}
