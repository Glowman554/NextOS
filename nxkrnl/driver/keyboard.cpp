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
	if(handler == 0) {

	} else if(key < 0x80){
		handler->KeyDown(keymap_de(key));
	}
}