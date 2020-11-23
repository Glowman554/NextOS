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
		case SYSCALL_INIT_TASK:
			init_task((void*) cpu->ebx);
			break;
		case SYSCALL_TASK_EXIT:
			task_exit(cpu->ebx);
			break;
		case SYSCALL_REBOOT:
			reboot();
			break;
		case SYSCALL_GETCHAR:
			cpu->ebx = getchar();
			break;
		case SYSCALL_GET_TICK:
			cpu->ebx = get_timer_tick();
			break;
		case SYSCALL_RESET_TICK:
			reset_timer_tick();
			break;
		case SYSCALL_MULTIBOOT:
			cpu->ebx = (uint32_t) pmb_info;
			break;
	}

	return cpu;
}
