#include <console.h>
#include <interrupts.h>
#include <panic.h>

void print_exception(int intr){
	setcolor(BACKGROUND_BLACK | FOREGROUND_RED);
	kprintf("Kernel PANIC: Excpetion 0x%x -> ", intr);
	
	switch(intr){
		case 0x0:
			kprintf("Divide by Zero");
			break;
		case 0x1:
			kprintf("Debug");
			break;
		case 0x2:
			kprintf("Non Maskable Interrupt");
			break;
		case 0x3:
			kprintf("Breakpoint");
			break;
		case 0x4:
			kprintf("Overflow");
			break;
		case 0x5:
			kprintf("Bound Range");
			break;
		case 0x6:
			kprintf("Invalid Opcode");
			break;
		case 0x7:
			kprintf("Device Not Available");
			break;
		case 0x8:
			kprintf("Double Fault");
			break;
		case 0x9:
			kprintf("Coprocessor Segment Overrun");
			break;
		case 0xa:
			kprintf("Invalid TSS");
			break;
		case 0xb:
			kprintf("Segment not Present");
			break;
		case 0xc:
			kprintf("Stack Fault");
			break;
		case 0xd:
			kprintf("General Protection");
			break;
		case 0xe:
			kprintf("Page Fault");
			break;
		case 0x10:
			kprintf("x87 Floating Point");
			break;
		case 0x11:
			kprintf("Alignment Check");
			break;
		case 0x12:
			kprintf("Machine Check");
			break;
		case 0x13:
			kprintf("SIMD Floating Point");
			break;
		case 0x1e:
			kprintf("Security-sensitive event in Host");
			break;
		default:
			kprintf("Reserved");
			break;
	}
	kputc('\n');
}