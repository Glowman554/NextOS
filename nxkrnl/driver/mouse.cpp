#include <driver/mouse.h>

extern "C"{
	#include <task.h>
}

MouseEventHandler::MouseEventHandler() {

}

void MouseEventHandler::OnMouseDown(uint8_t button) {

}

void MouseEventHandler::OnMouseUp(uint8_t button) {

}

void MouseEventHandler::OnMouseMove(long x, long y) {

}

MouseDriver::MouseDriver(MouseEventHandler *handler) : InterruptHandler(0x2c), dataport(0x60), commandport(0x64) {
    this->handler = handler;
}

void MouseDriver::MouseWait() {
    uint32_t timeout = 1000;
    while (timeout--){
        if ((commandport.Read() & 0b10) == 0){
            return;
        }
    }
}

void MouseDriver::MouseWaitInput() {
    uint32_t timeout = 1000;
    while (timeout--){
        if (commandport.Read() & 0b1){
            return;
        }
    }
}

void MouseDriver::MouseWrite(uint8_t value) {
    MouseWait();
    commandport.Write(0xD4);
    MouseWait();
    dataport.Write(value);
}

uint8_t MouseDriver::MouseRead() {
    MouseWaitInput();
    return dataport.Read();
}

void MouseDriver::Activate() {
    commandport.Write(0xa8);
    MouseWait();
    commandport.Write(0x20);
    MouseWaitInput();
    uint8_t status = dataport.Read();
    status |= 0b10;
    MouseWait();
    commandport.Write(0x60);
    MouseWait();
    dataport.Write(status);
    MouseWrite(0xf6);
    MouseRead();
    MouseWrite(0xf4);
    MouseRead();
}

void MouseDriver::Handle() {
    uint8_t data = dataport.Read();

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

    long x_old = x;
    long y_old = y;

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

    //kprintf("x: %d, y: %d\n", x, y);

    if(handler != 0) {
        handler->OnMouseMove(x - x_old, y - y_old);
    }

    mouse_packet_ready = false;
}