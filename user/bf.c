#include <syslib.h>
#include <stdint.h>

unsigned char tape[30000] = {0};
unsigned char* ptr = tape;


void interpret(char* input) {
	char current_char;
	int i;
	int loop;

	for (i = 0; input[i] != 0; i++) {
		current_char = input[i];
		if (current_char == '>') {
			++ptr;
		} else if (current_char == '<') {
			--ptr;
		} else if (current_char == '+') {
			++*ptr;
		} else if (current_char == '-') {
			--*ptr;
		} else if (current_char == '.' ) {
			kprintf("%c", *ptr);
		} else if (current_char == ',') {
			claim_kb_handler();
			*ptr = getchar();
		} else if (current_char == '[') {
			continue;
		} else if (current_char == ']' && *ptr) {
			loop = 1;
			while (loop > 0) {
				current_char = input[--i];
				if (current_char == '[') {
					loop--;
				} else if (current_char == ']') {
					loop++;
				}
			}
		}
	}
}

void _start(){
	init_driver("bf", (driver_handler_ptr) interpret);

	while(1);
}