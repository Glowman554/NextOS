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

struct dirent* fsroot_readdir(int index){
	register uint32_t input asm("ecx");
	asm("int $0x30" :: "a" (SYSCALL_FSROOT_READDIR), "b" (index));
	return (struct dirent*) input;
}

fs_node_t* fsroot_finddir(char* file){
	register uint32_t input asm("ecx");
	asm("int $0x30" :: "a" (SYSCALL_FSROOT_FINDDIR), "b" (file));
	return (fs_node_t*) input;
}

void fsroot_read(fs_node_t *fsnode, uint32_t offset, uint32_t size){
	asm("int $0x30" :: "a" (SYSCAlL_FSROOT_READ), "b" (offset), "c" (size), "d" (fsnode));
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

void set_mouse_handlers(mouse_move_handler h1, mouse_button_handler h2) {
	asm("int $0x30" :: "a" (SYSCALL_SET_MOUSE_HANDLER), "b" (h1), "c" (h2));
}

char getpixel(int x, int y) {
	register char c asm("edx");
	asm("int $0x30" : : "a" (SYSCALL_GETPIXEL), "b" (x), "c" (y));
	return c;
}

void kb_intr_handler(char key) {
	buf = key;
}

char pixbuf[15] = {0};
bool reloadp = false;

int x, y = 0;

void redraw() {
	setpixel(x, y, 15);
	setpixel(x + 1, y, 15);
	setpixel(x, y + 1, 15);
	setpixel(x + 1, y + 1, 15);
	setpixel(x + 2, y, 15);
	setpixel(x, y + 2, 15);
	setpixel(x + 2, y + 2, 15);
	setpixel(x + 1, y + 2, 15);
	setpixel(x + 2, y + 1, 15);
	setpixel(x + 3, y, 15);
	setpixel(x, y + 3, 15);
	setpixel(x + 4, y + 4, 15);
	setpixel(x + 5, y + 5, 15);
	setpixel(x + 3, y + 3, 15);
	setpixel(x + 6, y + 6, 15);

}

void loadpixel(){
	setpixel(x, y, pixbuf[0]);
	setpixel(x + 1, y, pixbuf[1]);
	setpixel(x, y + 1, pixbuf[2]);
	setpixel(x + 1, y + 1, pixbuf[3]);
	setpixel(x + 2, y, pixbuf[4]);
	setpixel(x, y + 2, pixbuf[5]);
	setpixel(x + 2, y + 2, pixbuf[6]);
	setpixel(x + 1, y + 2, pixbuf[7]);
	setpixel(x + 2, y + 1, pixbuf[8]);
	setpixel(x + 3, y, pixbuf[9]);
	setpixel(x, y + 3, pixbuf[10]);
	setpixel(x + 4, y + 4, pixbuf[11]);
	setpixel(x + 5, y + 5, pixbuf[12]);
	setpixel(x + 3, y + 3, pixbuf[13]);
	setpixel(x + 6, y + 6, pixbuf[14]);

}

void savepixel() {
	pixbuf[0] = getpixel(x, y);
	pixbuf[1] = getpixel(x + 1, y);
	pixbuf[2] = getpixel(x, y + 1);
	pixbuf[3] = getpixel(x + 1, y + 1);
	pixbuf[4] = getpixel(x + 2, y);
	pixbuf[5] = getpixel(x, y + 2);
	pixbuf[6] = getpixel(x + 2, y + 2);
	pixbuf[7] = getpixel(x + 1, y + 2);
	pixbuf[8] = getpixel(x + 2, y + 1);
	pixbuf[9] = getpixel(x + 3, y);
	pixbuf[10] = getpixel(x, y + 3);
	pixbuf[11] = getpixel(x + 4, y + 4);
	pixbuf[12] = getpixel(x + 5, y + 5);
	pixbuf[13] = getpixel(x + 3, y + 3);
	pixbuf[14] = getpixel(x + 6, y + 6);
}

void mouse_intr_move_handler(long x_new, long y_new) {
	//kprintf("x: %d y: %d\n", x, y);

	loadpixel();
	x = x_new;
	y = y_new;
	savepixel();
	redraw();
}

mouse_handler handler = 0;

void mouse_intr_button_handler(int button) {
	//kprintf("Button: %d\n", button);
	if(handler != 0)
		(*(handler))(button, x, y);
}

void set_mouse_handler(mouse_handler h) {
	handler = h;
}

void claim_kb_handler() {
	set_kb_handler(&kb_intr_handler);
}

void claim_mouse_handlers() {
	set_mouse_handlers(mouse_intr_move_handler, mouse_intr_button_handler);
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
			kprintf("%c", in[len]);
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