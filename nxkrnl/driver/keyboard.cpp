#include <driver/keyboard.h>

extern "C"{
	#include <console.h>
	#include <keymap.h>
	#include <task.h>
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
	if(handler == 0) {

	} else if(key < 0x80){
		handler->KeyDown(keymap_de(key));
	}
}

bool KeyboardDriver::is_presend() {
	return true;
}

char* KeyboardDriver::get_name() {
	return "keyboard";
}