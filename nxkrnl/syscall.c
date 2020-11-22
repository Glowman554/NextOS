#include <syscall.h>

struct cpu_state* syscall(struct cpu_state* cpu){
	switch (cpu->eax) {
		case SYSCALL_PUTC:
			kprintf("%c", cpu->ebx);
			break;
		case SYSCALL_PUTS:
			kprintf("%s", cpu->ebx);
			break;
		case SYSCALL_PUTN:
			if(cpu->ecx == 0xf)
				kprintf("%x", cpu->ebx);
			if(cpu->ecx == 0xa)
				kprintf("%d", cpu->ebx);
			break;
		case SYSCALL_CLRSCR:
			clrscr();
			break;
		case SYSCALL_SETCOLOR:
			setcolor(cpu->ebx);
			break;
		case SYSCALL_KVERSION:
			cpu->ebx = VERSION;
			break;
		case SYSCALL_KVENDOR:
			cpu->ebx = (uint32_t) &VENDOR;
			break;
		case SYSCALL_EXEC:
			exec_file((char*) cpu->ebx);
			break;
	}

	return cpu;
}
