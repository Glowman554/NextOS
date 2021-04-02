#include <console.h>

int x = 0;
int y = 0;
char* video = (char*) VIDEO_MEM;
uint32_t color = BACKGROUND_BLACK | FOREGROUND_WHITE;

bool scrole = true;

void setx(int i) {
	x = i;
}

void sety(int i) {
	y = i;
}

int getx() {
	return x;
}

int gety() {
	return y;
}

void setcolor(uint32_t c){
	color = c;
}

void kputc(char c){
	if ((c == '\n') || (x > 79)) {
		x = 0;
		y++;
	}
	
	//if(SERIAL_DEBUG) write_serial(c);

	if (c == '\n') {
	//	if(SERIAL_DEBUG) write_serial('\r');
		return;
	}

	if (scrole && y > 22) {
		int i;
		for (i = 0; i < 2 * 22 * 80; i++) {
			video[i] = video[i + 160];
		}

		for (; i < 2 * 25 * 80; i++) {
			video[i] = 0;
		}
		y--;
	}

	video[2 * (y * 80 + x)] = c;
	video[2 * (y * 80 + x) + 1] = color;

	x++;
	
	setcursor(y * 80 + x);

}

void kputs(const char *s){
	while(*s){
		kputc(*s++);
	}
}

void clrscr(){
	int i;
	for (i = 0; i < 25 * 80; i++) {
		video[2*i] = ' ';
		video[2*i+1] = BACKGROUND_BLACK | FOREGROUND_WHITE;
	}

	x = y = 0;

}

void kputn(unsigned long x, int base){
	char buf[65];
	const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	char* p;
	
	if(base > 36) return;
	
	p = buf + 64;
	*p = '\0';
	do {
		*--p = digits[x % base];
		x /= base;
	} while (x);
	kputs(p);

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

void setcursor(uint16_t pos){
	outb(0x3d4, 14);
	outb(0x3d5, pos >> 8);
	outb(0x3d4, 15);
	outb(0x3d5, pos);
}