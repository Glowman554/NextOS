#include <syscall.h>

uint8_t buf[4096];

struct cpu_state* syscall(struct cpu_state* cpu){
	
	bool mode = is_vga_active();
	
	switch (cpu->eax) {
		case SYSCALL_PUTC:
			if(mode)
				vga_kprintf("%c", cpu->ebx);
			else
				kprintf("%c", cpu->ebx);
			break;
		case SYSCALL_PUTS:
			if(mode)
				vga_kprintf("%s", cpu->ebx);
			else
				kprintf("%s", cpu->ebx);
			break;
		case SYSCALL_PUTN:
			if(mode){
				if(cpu->ecx == 16)
					vga_kprintf("%x", cpu->ebx);
				if(cpu->ecx == 10)
					vga_kprintf("%d", cpu->ebx);
			} else {
				if(cpu->ecx == 16)
					kprintf("%x", cpu->ebx);
				if(cpu->ecx == 10)
					kprintf("%d", cpu->ebx);
			}
			break;
		case SYSCALL_CLRSCR:
			if(mode)
				clear_vga();
			else
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
		case SYSCALL_VGA_MODE:
			init_vga();
			break;
		case SYSCALL_VGA_SETPIXEL:
			setpixel(cpu->ebx, cpu->ecx, cpu->edx);
			break;
		case SYSCALL_VGA_SETCOLOR:
			set_vga_color(cpu->ebx, cpu->ecx);
			break;
		case SYSCALL_LOAD_INITRD:
			fs_root = initialise_initrd(get_module_by_name((char*) cpu->ebx));
			break;
		case SYSCALL_INITRD_READDIR:
			cpu->ecx = (uint32_t) readdir_fs(fs_root, cpu->ebx);
			break;
		case SYSCALL_INITRD_FINDDIR:
			cpu->ecx = (uint32_t) finddir_fs(fs_root, (char*) cpu->ebx);
			break;
		case SYSCAlL_INITRD_READ:
			read_fs((fs_node_t*) cpu->edx, cpu->ebx, cpu->ecx, buf);
			break;
		case SYSCALL_GET_BUFFER:
			cpu->ebx = (uint32_t) buf;
			break;
		case SYSCALL_INIT_DRIVER:
			cpu->edx = init_driver((char*) cpu->ebx, (driver_handler_ptr) cpu->ecx);
			break;
		case SYSCALL_CALL_DRIVER:
			cpu->edx = call_driver_handler(cpu->ebx, (void*) cpu->ecx);
			break;
		case SYSCALL_FIND_DRIVER:
			cpu->ecx = find_driver_by_name((char*) cpu->ebx);
			break;
	}

	return cpu;
}
