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

void print_exception(int intr){
	setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
	if(is_vga_active()){
		set_vga_color(VGA_WHITE, VGA_BLUE);
		clear_vga();
		vga_kprintf("Kernel PANIC:\nExcpetion 0x%x -> %s\n", intr, get_exception_name(intr));
	} else {
		kprintf("Kernel PANIC: Excpetion 0x%x -> %s\n", intr, get_exception_name(intr));
    }
}