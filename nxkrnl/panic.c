#include <panic.h>

char* get_exception_name(int exc){
	switch(exc){
		case 0x0:
			return("Divide by Zero");
			break;
		case 0x1:
			return("Debug");
			break;
		case 0x2:
			return("Non Maskable Interrupt");
			break;
		case 0x3:
			return("Breakpoint");
			break;
		case 0x4:
			return("Overflow");
			break;
		case 0x5:
			return("Bound Range");
			break;
		case 0x6:
			return("Invalid Opcode");
			break;
		case 0x7:
			return("Device Not Available");
			break;
		case 0x8:
			return("Double Fault");
			break;
		case 0x9:
			return("Coprocessor Segment Overrun");
			break;
		case 0xa:
			return("Invalid TSS");
			break;
		case 0xb:
			return("Segment not Present");
			break;
		case 0xc:
			return("Stack Fault");
			break;
		case 0xd:
			return("General Protection");
			break;
		case 0xe:
			return("Page Fault");
			break;
		case 0x10:
			return("x87 Floating Point");
			break;
		case 0x11:
			return("Alignment Check");
			break;
		case 0x12:
			return("Machine Check");
			break;
		case 0x13:
			return("SIMD Floating Point");
			break;
		case 0x1e:
			return("Security-sensitive event in Host");
			break;
		default:
			return("Reserved");
			break;
	}
}

void kernel_yeet(struct cpu_state* cpu) {
	if(is_vga_active()){
		set_vga_color(VGA_WHITE, VGA_BLUE);
		clear_vga();
		vga_kprintf("Kernel PANIC:\nExcpetion 0x%x -> %s\n", cpu->intr, get_exception_name(cpu->intr));
			
		vga_kprintf("eax: 0x%x, ebx: 0x%x\necx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
		vga_kprintf("esi: 0x%x, edi: 0x%x\nebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
		vga_kprintf("intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
		vga_kprintf("eip: 0x%x, cs 0x%x\neflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
		vga_kprintf("esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);
			
		set_vga_color(VGA_YELLOW, VGA_BLUE);
		vga_kprintf("\nTo debug the problem please look in the serial console COM1!\n");
	} else {
		setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
		kprintf("Kernel PANIC: Excpetion 0x%x -> %s\n", cpu->intr, get_exception_name(cpu->intr));
						

		kprintf("\neax: 0x%x, ebx: 0x%x, ecx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
		kprintf("esi: 0x%x, edi: 0x%x, ebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
		kprintf("intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
		kprintf("eip: 0x%x, cs 0x%x, eflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
		kprintf("esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);
			
		setcolor(BACKGROUND_BLACK | FOREGROUND_YELLOW);
		kprintf("\nTo debug the problem please look in the serial console COM1!\n");
	}

	kputs_serial(yeet0);
	kputs_serial(yeet1);
	kputs_serial(yeet2);
	kputs_serial(yeet3);
	kputs_serial(yeet4);
	kputs_serial(yeet5);

	kprintf_serial("\nKernel PANIC: Excpetion 0x%x -> %s\n", cpu->intr, get_exception_name(cpu->intr));
						

	kprintf_serial("\neax: 0x%x, ebx: 0x%x, ecx: 0x%x, edx: 0x%x\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
	kprintf_serial("esi: 0x%x, edi: 0x%x, ebp: 0x%x\n", cpu->esi, cpu->edi, cpu->ebp);
	kprintf_serial("intr: 0x%x, error: 0x%x\n", cpu->intr, cpu->error);
	kprintf_serial("eip: 0x%x, cs 0x%x, eflags: 0x%x\n", cpu->eip, cpu->cs, cpu->eflags);
	kprintf_serial("esp: 0x%x, ss: 0x%x\n", cpu->esp, cpu->ss);

	kprintf_serial("Running kernel version %d\n", VERSION);

	char buffer = 0;

	while (1) {
		kprintf_serial("KDB >> ");
		buffer = read_serial();
		kprintf_serial("%c\n", buffer);

		switch (buffer) {
			case 'h':
				kprintf_serial("c -> continue execution!\n");
				kprintf_serial("r -> reboot system!\n");
				kprintf_serial("i -> show info about the running task!\n");
				kprintf_serial("t -> clear current task!\n");
				kprintf_serial("s -> stop kernel debugger!\n");
				break;
			case 'c':
				goto _return;
				break;
			case 'r':
				reboot();
				break;
			case 'i':
				if(task_states[current_task].active) {
					kprintf_serial("Task slot: %d\n", current_task);
					kprintf_serial("PID: %d, stack: 0x%x, user stack: 0x%x\n", task_states[current_task].pid, task_states[current_task].stack, task_states[current_task].user_stack);
					kprintf_serial("KB: 0x%x, MMH: 0x%x, MBH: 0x%x\n", task_states[current_task].kb, task_states[current_task].mmh, task_states[current_task].mbh);
				} else {
					kprintf_serial("Task in task slot %d not active!\n", current_task);
				}
				break;
			case 't':
				if(task_states[current_task].active) {
					task_exit(2);
				} else {
					kprintf_serial("Already dead!\n");
				}
				break;
			case 's':
				goto _stop;
				break;
			default:
				kprintf_serial("Unknown command!\n");
				break;
		}
	}
	
_stop:
	while(1){
		asm volatile("cli; hlt");
	}

_return:
	return;
}