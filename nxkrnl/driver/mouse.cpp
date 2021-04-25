#include <driver/mouse.h>

extern "C"{
	#include <task.h>
}

MouseEventHandler::MouseEventHandler() {

}

void MouseEventHandler::on_mouse_down(uint8_t button) {

}

void MouseEventHandler::on_mouse_move(long x, long y) {

}

MouseDriver::MouseDriver(MouseEventHandler *handler) : InterruptHandler(0x2c), dataport(0x60), commandport(0x64) {
	this->handler = handler;
}

void MouseDriver::mouse_wait() {
	uint32_t timeout = 1000;
	while (timeout--){
		if ((commandport.read() & 0b10) == 0){
			return;
		}
	}
}

void MouseDriver::mouse_wait_input() {
	uint32_t timeout = 1000;
	while (timeout--){
		if (commandport.read() & 0b1){
			return;
		}
	}
}

void MouseDriver::mouse_write(uint8_t value) {
	mouse_wait();
	commandport.write(0xD4);
	mouse_wait();
	dataport.write(value);
}

uint8_t MouseDriver::mouse_read() {
	mouse_wait_input();
	return dataport.read();
}

void MouseDriver::activate() {
	commandport.write(0xa8);
	mouse_wait();
	commandport.write(0x20);
	mouse_wait_input();
	uint8_t status = dataport.read();
	status |= 0b10;
	mouse_wait();
	commandport.write(0x60);
	mouse_wait();
	dataport.write(status);
	mouse_write(0xf6);
	mouse_read();
	mouse_write(0xf4);
	mouse_read();
}

void MouseDriver::handle() {
	uint8_t data = mouse_read();

	//kprintf("%d", mouse_cycle);
	
	switch(mouse_cycle) {
		case 0:
			if(mouse_packet_ready)
				break;
			if ((data & 0b00001000) == 0)
				break;
			mouse_packet[0] = data;
			mouse_cycle++;
			break;
		case 1:
			if(mouse_packet_ready)
				break;
			mouse_packet[1] = data;
			mouse_cycle++;
			break;
		case 2:
			if(mouse_packet_ready)
				break;
			mouse_packet[2] = data;
			mouse_packet_ready = true;
			mouse_cycle = 0;
			break;
	}

	if(!mouse_packet_ready)
		return;


	bool x_negative, y_negative, x_overflow, y_overflow;

	if (mouse_packet[0] & PS2XSign){
		x_negative = true;
	} else {
		x_negative = false;
	}

	if (mouse_packet[0] & PS2YSign){
		y_negative = true;
	} else {
		y_negative = false;
	}

	if (mouse_packet[0] & PS2XOverflow){
		x_overflow = true;
	} else {
		x_overflow = false;
	}

	if (mouse_packet[0] & PS2YOverflow){
		y_overflow = true;
	} else {
		y_overflow = false;
	}

	if (!x_negative){
		x += mouse_packet[1];
		if (x_overflow){
			x += 255;
		}
	} else {
		mouse_packet[1] = 256 - mouse_packet[1];
		x -= mouse_packet[1];
		if (x_overflow){
			x -= 255;
		}
	}

	if (!y_negative){
		y -= mouse_packet[2];
		if (y_overflow){
			y -= 255;
		}
	} else {
		mouse_packet[2] = 256 - mouse_packet[2];
		y += mouse_packet[2];
		if (y_overflow){
			y += 255;
		}
	}

	if(x > 320) {
		x = 320;
	}

	if(x < 0) {
		x = 0;
	}

	if(y > 200) {
		y = 200;
	}

	if(y < 0) {
		y = 0;
	}

	//kprintf("X: %d Y: %d\n", x, y);

	if(handler != 0) {
		handler->on_mouse_move(x, y);

		if(mouse_packet[0] & 1) {
			handler->on_mouse_down(LeftButton);
		}
		
		if((mouse_packet[0] >> 1) & 1) {
			handler->on_mouse_down(RightButton);
		}
		
		if((mouse_packet[0] >> 2) & 1) {
			handler->on_mouse_down(MiddleButton);
		}

	}

	mouse_packet_ready = false;
}

bool MouseDriver::is_presend() {
	return true;
}

char* MouseDriver::get_name() {
	return "mouse";
}