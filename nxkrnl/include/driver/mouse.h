#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <driver/driver.h>
#include <port.h>
#include <interrupt_handler.h>

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000010
#define PS2Rightbutton 0b00000100
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

class MouseEventHandler {
    public:
        MouseEventHandler();
        virtual void OnMouseDown(uint8_t button);
        virtual void OnMouseUp(uint8_t button);
        virtual void OnMouseMove(int x, int y);
};

class MouseDriver : public InterruptHandler, public Driver{
    private:
		Port8Bit dataport;
		Port8Bit commandport;
		
        MouseEventHandler* handler;

        void MouseWait();
        void MouseWaitInput();
        void MouseWrite(uint8_t value);
        uint8_t MouseRead();

        uint8_t mouse_cycle = 0;
        uint8_t mouse_packet[4];
        bool mouse_packet_ready = false;

    public:
        MouseDriver(MouseEventHandler *handler);
        ~MouseDriver();
        virtual void Handle();
		virtual void Activate();
};

#endif