#include <driver/serial.h>

void init_serial(){
	outb(PORT + 1, 0x00);	// Disable all interrupts
	outb(PORT + 3, 0x80);	// Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);	// Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);	//				  (hi byte)
	outb(PORT + 3, 0x03);	// 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);	// Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);	// IRQs enabled, RTS/DSR set
}

int serial_recived(){
	return inb(PORT + 5) & 1;
}

char read_serial(){
	while(serial_recived() == 0);
	return inb(PORT);
}

int is_transmit_empty(){
	return inb(PORT + 5) & 0x20;
}

void write_serial(char a){
	while(is_transmit_empty() == 0);
	outb(PORT, a);
}

void kputc_serial(char c){

	
	write_serial(c);

	if (c == '\n') {
		write_serial('\r');
		return;
	}
}

void kputs_serial(const char *s){
	while(*s){
		kputc_serial(*s++);
	}
}

void kputn_serial(unsigned long x, int base){
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
	kputs_serial(p);

}

void kprintf_serial(const char* fmt, ...){
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
					kputs_serial(s);
					break;
				case 'c':
					n = va_arg(ap, int);
					kputc_serial(n);
					break;
				case 'd':
				case 'u':
					n = va_arg(ap, unsigned long int);
					kputn_serial(n, 10);
					break;
				case 'x':
				case 'p':
					n = va_arg(ap, unsigned long int);
					kputn_serial(n, 16);
					break;
				case '%':
					kputc_serial('%');
					break;
				case '\0':
					goto out;
				default:
					kputc_serial('%');
					kputc_serial(*fmt);
					break;
			}
		} else {
			kputc_serial(*fmt);
		}

		fmt++;
	}

out:
	va_end(ap);

}