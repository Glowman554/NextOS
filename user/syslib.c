#include <stdint.h>
#include <syslib.h>
#include <stdarg.h>

void kputc(char c){
	asm("int $0x30" :: "a" (SYSCALL_PUTC), "b" (c));
}


void kputs(const char *s){
	asm("int $0x30" :: "a" (SYSCALL_PUTS), "b" (s));
}
void kputn(int n, int base){
	asm("int $0x30" :: "a" (SYSCALL_PUTN), "b" (n), "c" (base));
}
void clrscr(){
	asm("int $0x30" :: "a" (SYSCALL_CLRSCR));
}
void setcolor(uint32_t color){
	asm("int $0x30" :: "a" (SYSCALL_SETCOLOR), "b" (color));
}
int kversion(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_KVERSION));
	return input;
}
char* kvendor(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_KVENDOR));
	return (char*) input;
}
void exec(char* file){
	asm("int $0x30" :: "a" (SYSCALL_EXEC), "b" (file));
}

void init_task(void* entry){
	asm("int $0x30" :: "a" (SYSCALL_INIT_TASK), "b" (entry));
}

void task_exit(int code){
	asm("int $0x30" : : "a" (SYSCALL_TASK_EXIT), "b" (code));
	while(1);
}

void kprintf(const char* fmt, ...){
	va_list ap;
	const char* s;
	unsigned long n;

	va_start(ap, fmt);
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case 's':
					s = va_arg(ap, char*);
					kputs(s);
					break;
				case 'c':
					n = va_arg(ap, int);
					kputc(n);
					break;
				case 'd':
				case 'u':
					n = va_arg(ap, unsigned long int);
					kputn(n, 10);
					break;
				case 'x':
				case 'p':
					n = va_arg(ap, unsigned long int);
					kputn(n, 16);
					break;
				case '%':
					kputc('%');
					break;
				case '\0':
					goto out;
				default:
					kputc('%');
					kputc(*fmt);
					break;
			}
		} else {
			kputc(*fmt);
		}

		fmt++;
	}

out:
	va_end(ap);

}