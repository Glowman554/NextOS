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

void reboot(){
	asm("int $0x30" :: "a" (SYSCALL_REBOOT));
}

char getchar(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_GETCHAR));
	return (char) input;
}

uint32_t get_timer_tick(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_GET_TICK));
	return input;
}

void reset_timer_tick(){
	asm("int $0x30" :: "a" (SYSCALL_RESET_TICK));
}

struct multiboot_info* get_mb_ptr(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_MULTIBOOT));
	return (struct multiboot_info*) input;
}

void init_vga(){
	asm("int $0x30" :: "a" (SYSCALL_VGA_MODE));
}

void setpixel(int x, int y, uint32_t color){
	asm("int $0x30" :: "a" (SYSCALL_VGA_SETPIXEL), "b" (x), "c" (y), "d" (color));
}

void set_vga_color(uint32_t fgcolor, uint32_t bgcolor){
	asm("int $0x30" :: "a" (SYSCALL_VGA_SETCOLOR), "b" (fgcolor), "c" (bgcolor));
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

int strcmp(char *str1, char *str2){
	int i = 0;
	int failed = 0;
	while(str1[i] != '\0' && str2[i] != '\0'){
		if(str1[i] != str2[i]){
			failed = 1;
			break;
		}
		i++;
	}
	if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
		failed = 1;
	return failed;
}