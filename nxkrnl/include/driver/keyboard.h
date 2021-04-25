#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <driver/driver.h>
#include <port.h>
#include <interrupt_handler.h>

class KeyboardEventHandler{
	public:
		KeyboardEventHandler();
		
		virtual void key_down(char);
		virtual void key_up(char);
};

class KeyboardDriver : public InterruptHandler, public Driver{
	private:
		Port8Bit dataport;
		Port8Bit commandport;
		
		KeyboardEventHandler* handler;

		bool l_shift;
		
	public:
		KeyboardDriver(KeyboardEventHandler *handler);
		//~KeyboardDriver();
		virtual void handle();
		virtual void activate();
		virtual bool is_presend();
		virtual char* get_name();
};

#endif