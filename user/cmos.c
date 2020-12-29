#include <stdint.h>
#include <syslib.h>
#include <driverlib.h>
#include <cmos_defs.h>

uint8_t cmos_read(uint8_t address){
	if (address < 10){
		outb(0x70, 0xa);

		while((inb(0x71) & (1 << 7)) != 0){
			asm volatile("nop");
		}
	}

	outb(0x70, address);
	return inb(0x71);
}

static uint8_t convert(uint8_t num){
	if((cmos_read(0xb) & (1 << 2)) == 0){
		return (num & 0xf) + ((num >> 4) & 0xf) * 10;
	} else {
		return num;
	}
}

int driver_handler(cmos_data_t* data){
	int temp = 0;
	switch(data->function){
		case CMOS_FUNCTION_READH:
			temp = cmos_read(0x04);
			return convert(temp);
			break;
		case CMOS_FUNCTION_READM:
			temp = cmos_read(0x02);
			return convert(temp);
			break;
		case CMOS_FUNCTION_READS:
			temp = cmos_read(0x00);
			return convert(temp);
			break;
	}
	return -1;
}

void _start(void){
	init_driver("cmos", (driver_handler_ptr) driver_handler);
	
	task_exit(0);
}