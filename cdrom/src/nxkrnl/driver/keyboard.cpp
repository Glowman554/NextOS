#include <driver/keyboard.h>

extern "C"{
	#include <console.h>
}

KeyboardEventHandler::KeyboardEventHandler(){
	
}

void KeyboardEventHandler::KeyUp(char){
	
}

void KeyboardEventHandler::KeyDown(char){
	
}

KeyboardDriver::KeyboardDriver(KeyboardEventHandler *handler) : InterruptHandler(0x21), dataport(0x60), commandport(0x64){
	this->handler = handler;
}

void KeyboardDriver::Activate(){
	while(commandport.Read() & 0x1)
		dataport.Read();
	commandport.Write(0xae);
	commandport.Write(0x20);
	uint8_t status = (dataport.Read() | 1) & ~0x10;
	commandport.Write(0x60);
	dataport.Write(status);
	dataport.Write(0xf4);
}

void KeyboardDriver::Handle(){
	uint8_t key = dataport.Read();
	if(handler == 0)
		return;
	
	if(key < 0x80){
		switch(key){
			case 0x02: handler->KeyDown('1'); break;
			case 0x03: handler->KeyDown('2'); break;
			case 0x04: handler->KeyDown('3'); break;
			case 0x05: handler->KeyDown('4'); break;
			case 0x06: handler->KeyDown('5'); break;
			case 0x07: handler->KeyDown('6'); break;
			case 0x08: handler->KeyDown('7'); break;
			case 0x09: handler->KeyDown('8'); break;
			case 0x0A: handler->KeyDown('9'); break;
			case 0x0B: handler->KeyDown('0'); break;

			case 0x10: handler->KeyDown('q'); break;
			case 0x11: handler->KeyDown('w'); break;
			case 0x12: handler->KeyDown('e'); break;
			case 0x13: handler->KeyDown('r'); break;
			case 0x14: handler->KeyDown('t'); break;
			case 0x15: handler->KeyDown('z'); break;
			case 0x16: handler->KeyDown('u'); break;
			case 0x17: handler->KeyDown('i'); break;
			case 0x18: handler->KeyDown('o'); break;
			case 0x19: handler->KeyDown('p'); break;

			case 0x1E: handler->KeyDown('a'); break;
			case 0x1F: handler->KeyDown('s'); break;
			case 0x20: handler->KeyDown('d'); break;
			case 0x21: handler->KeyDown('f'); break;
			case 0x22: handler->KeyDown('g'); break;
			case 0x23: handler->KeyDown('h'); break;
			case 0x24: handler->KeyDown('j'); break;
			case 0x25: handler->KeyDown('k'); break;
			case 0x26: handler->KeyDown('l'); break;

			case 0x2C: handler->KeyDown('y'); break;
			case 0x2D: handler->KeyDown('x'); break;
			case 0x2E: handler->KeyDown('c'); break;
			case 0x2F: handler->KeyDown('v'); break;
			case 0x30: handler->KeyDown('b'); break;
			case 0x31: handler->KeyDown('n'); break;
			case 0x32: handler->KeyDown('m'); break;
			case 0x33: handler->KeyDown(','); break;
			case 0x34: handler->KeyDown('.'); break;
			case 0x35: handler->KeyDown('-'); break;

			case 0x1C: handler->KeyDown('\n'); break;
			case 0x39: handler->KeyDown(' '); break;

			default:
				//kprintf("Keyboard 0x%x\n", key);
				break;
		}
	}
}