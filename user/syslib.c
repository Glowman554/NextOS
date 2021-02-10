#include <syslib.h>

char buf;

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

char getchar_old(){
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

void load_initrd(char* file){
	asm("int $0x30" :: "a" (SYSCALL_LOAD_INITRD), "b" (file));
}

struct dirent* initrd_readdir(int index){
	register uint32_t input asm("ecx");
	asm("int $0x30" :: "a" (SYSCALL_INITRD_READDIR), "b" (index));
	return (struct dirent*) input;
}

fs_node_t* initrd_finddir(char* file){
	register uint32_t input asm("ecx");
	asm("int $0x30" :: "a" (SYSCALL_INITRD_FINDDIR), "b" (file));
	return (fs_node_t*) input;
}

void initrd_read(fs_node_t *fsnode, uint32_t offset, uint32_t size){
	asm("int $0x30" :: "a" (SYSCAlL_INITRD_READ), "b" (offset), "c" (size), "d" (fsnode));
}

uint8_t* get_buffer(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_GET_BUFFER));
	return (uint8_t*) input;
}

int init_driver(char* name, driver_handler_ptr driver_handler){
	register uint32_t input asm("edx");
	asm("int $0x30" :: "a" (SYSCALL_INIT_DRIVER), "b" (name), "c" (driver_handler));
	return input;
}

int call_driver_handler(int id, void* data){
	register uint32_t input asm("edx");
	asm("int $0x30" :: "a" (SYSCALL_CALL_DRIVER), "b" (id), "c" (data));
	return input;
}

int find_driver_by_name(char* name){
	register uint32_t input asm("ecx");
	asm("int $0x30" :: "a" (SYSCALL_FIND_DRIVER), "b" (name));
	return input;
}

void draw_char(char c, int x, int y){
	asm("int $0x30" :: "a" (SYSCALL_DRAW_CHAR), "b" (c), "c" (x), "d" (y));
}

void setx(int i){
	asm("int $0x30" :: "a" (SYSCALL_SETX), "b" (i));
}

void sety(int i){
	asm("int $0x30" :: "a" (SYSCALL_SETY), "b" (i));
}

int getx(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_GETX));
	return input;
}

int gety(){
	register uint32_t input asm("ebx");
	asm("int $0x30" :: "a" (SYSCALL_GETY));
	return input;
}

void set_kb_handler(kb_handler handler) {
	asm("int $0x30" :: "a" (SYSCALL_SET_KB_HANDLER), "b" (handler));
}

void kb_intr_handler(char key) {
	buf = key;
	if(key != '\n' && key != '\b')
		kprintf("%c", key);
}

void claim_kb_handler() {
	set_kb_handler(&kb_intr_handler);
}

char getchar() {
	buf = 0;
	while(buf == 0);
	return buf;
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

char in[100];

char* get_input() {

	for(int i = 0; i < 100; i++)
		in[i] = 0;

	bool reading = true;
	int len = 0;

	int x;

	while(reading){
		in[len] = getchar();
		x = getx();

		if(in[len] == 10){
			in[len] = '\0';
			reading = false;
			kprintf("\n");
		} else if(in[len] == '\b') {
			if(len == 0) {
				continue;
			}
			in[len] = 0;
			len--;
			setx(x - 1);
			kputc(' ');
			setx(x - 1);
		} else {
			len++;
		}
	}

	return in;
}

int strlen(char *src){
	int i = 0;
	while (*src++)
		i++;
	return i;
}