#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <driver/driver.h>
#include <port.h>
#include <interrupt_handler.h>

#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

enum MouseButtons {
	LeftButton = 1,
	RightButton = 2,
	MiddleButton = 3
};

class MouseEventHandler {
	public:
		MouseEventHandler();
		virtual void on_mouse_down(uint8_t button);
		virtual void on_mouse_move(long x, long y);
};

class MouseDriver : public InterruptHandler, public Driver{
	private:
		Port8Bit dataport;
		Port8Bit commandport;
		
		MouseEventHandler* handler;

		void mouse_wait();
		void mouse_wait_input();
		void mouse_write(uint8_t value);
		uint8_t mouse_read();

		uint8_t mouse_cycle = 0;
		uint8_t mouse_packet[4];
		bool mouse_packet_ready = false;

	public:
		MouseDriver(MouseEventHandler *handler);
		//~MouseDriver();

		long x = 0;
		long y = 0;

		virtual void handle();
		virtual void activate();
		virtual bool is_presend();
		virtual char* get_name();
};

#endif