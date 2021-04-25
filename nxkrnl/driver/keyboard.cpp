#include <driver/keyboard.h>

extern "C"{
	#include <console.h>
	#include <keymap.h>
	#include <task.h>
}

KeyboardEventHandler::KeyboardEventHandler(){
	
}

void KeyboardEventHandler::key_up(char){
	
}

void KeyboardEventHandler::key_down(char){
	
}

KeyboardDriver::KeyboardDriver(KeyboardEventHandler *handler) : InterruptHandler(0x21), dataport(0x60), commandport(0x64){
	this->handler = handler;
	this->l_shift = false;
}

void KeyboardDriver::activate(){
	while(commandport.read() & 0x1)
		dataport.read();
	commandport.write(0xae);
	commandport.write(0x20);
	uint8_t status = (dataport.read() | 1) & ~0x10;
	commandport.write(0x60);
	dataport.write(status);
	dataport.write(0xf4);
}

void KeyboardDriver::handle(){
	uint8_t key = dataport.read();
	uint8_t charcode = 0;
	if(handler == 0) {

	} else {
		switch (key) {
			case 0x2a:
				this->l_shift = true;
				break;
			case 0xaa:
				this->l_shift = false;
				break;
			default:
				charcode = keymap_de(key, this->l_shift);
				if(charcode != 0) {
					handler->key_down(charcode);
				}
				break;
		}
	}
}

bool KeyboardDriver::is_presend() {
	return true;
}

char* KeyboardDriver::get_name() {
	return "keyboard";
}