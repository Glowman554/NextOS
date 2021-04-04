#include <driver/mouse.h>

extern "C"{
	#include <task.h>
}

MouseEventHandler::MouseEventHandler() {

}

void MouseEventHandler::OnMouseDown(uint8_t button) {

}

void MouseEventHandler::OnMouseMove(long x, long y) {

}

MouseDriver::MouseDriver(MouseEventHandler *handler) : InterruptHandler(0x2c), dataport(0x60), commandport(0x64) {
    this->handler = handler;
}

void MouseDriver::MouseWait() {
    uint32_t timeout = 1000;
    while (timeout--){
        if ((commandport.read() & 0b10) == 0){
            return;
        }
    }
}

void MouseDriver::MouseWaitInput() {
    uint32_t timeout = 1000;
    while (timeout--){
        if (commandport.read() & 0b1){
            return;
        }
    }
}

void MouseDriver::MouseWrite(uint8_t value) {
    MouseWait();
    commandport.write(0xD4);
    MouseWait();
    dataport.write(value);
}

uint8_t MouseDriver::MouseRead() {
    MouseWaitInput();
    return dataport.read();
}

void MouseDriver::activate() {
    commandport.write(0xa8);
    MouseWait();
    commandport.write(0x20);
    MouseWaitInput();
    uint8_t status = dataport.read();
    status |= 0b10;
    MouseWait();
    commandport.write(0x60);
    MouseWait();
    dataport.write(status);
    MouseWrite(0xf6);
    MouseRead();
    MouseWrite(0xf4);
    MouseRead();
}

void MouseDriver::handle() {
    uint8_t data = MouseRead();

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


    bool xNegative, yNegative, xOverflow, yOverflow;

    if (mouse_packet[0] & PS2XSign){
        xNegative = true;
    } else {
        xNegative = false;
    }

    if (mouse_packet[0] & PS2YSign){
        yNegative = true;
    } else {
        yNegative = false;
    }

    if (mouse_packet[0] & PS2XOverflow){
        xOverflow = true;
    } else {
        xOverflow = false;
    }

    if (mouse_packet[0] & PS2YOverflow){
        yOverflow = true;
    } else {
        yOverflow = false;
    }

    if (!xNegative){
        x += mouse_packet[1];
        if (xOverflow){
            x += 255;
        }
    } else {
        mouse_packet[1] = 256 - mouse_packet[1];
        x -= mouse_packet[1];
        if (xOverflow){
            x -= 255;
        }
    }

    if (!yNegative){
        y -= mouse_packet[2];
        if (yOverflow){
            y -= 255;
        }
    } else {
        mouse_packet[2] = 256 - mouse_packet[2];
        y += mouse_packet[2];
        if (yOverflow){
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
        handler->OnMouseMove(x, y);

        if(mouse_packet[0] & 1) {
            handler->OnMouseDown(LeftButton);
        }
	    
        if((mouse_packet[0] >> 1) & 1) {
            handler->OnMouseDown(RightButton);
        }
		
        if((mouse_packet[0] >> 2) & 1) {
            handler->OnMouseDown(MiddleButton);
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