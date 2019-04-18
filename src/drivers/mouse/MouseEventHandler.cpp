#include <drivers/mouse/MouseEventHandler.hpp>

MouseEventHandler::MouseEventHandler() {}

void MouseEventHandler::onMouseDown(uint8_t button) {
    printf("Mouse down event.\n");
}

void MouseEventHandler::onMouseUp(uint8_t button) {
    printf("Mouse up event.\n");
}

void MouseEventHandler::onActivate() {
    uint16_t* videoMemory = (uint16_t*) 0xb8000;
    //TODO: What are these values? If it is the screen size, can we make this dynamic?
    x = 40;
    y = 12;
    videoMemory[80 * y + x] = (videoMemory[80 * y + x] & 0x0F00) << 4
                            | (videoMemory[80 * y + x] & 0xF000) >> 4
                            | (videoMemory[80 * y + x] & 0x00FF);        
}

void MouseEventHandler::onMouseMove(int xoffset, int yoffset) {
    static uint16_t* videoMemory = (uint16_t*) 0xb8000;
    videoMemory[80 * y + x] = (videoMemory[80 * y + x] & 0x0F00) << 4
                            | (videoMemory[80 * y + x] & 0xF000) >> 4
                            | (videoMemory[80 * y + x] & 0x00FF);
    //TODO: See comment above re: screen size
    x += xoffset;
    if (x >= 80) {
        x = 79;
    }
    if (x < 0) {
        x = 0;
    }
    y += yoffset;
    if (y >= 25) {
        y = 24;
    }
    if (y < 0) {
        y = 0;
    }

    videoMemory[80 * y + x] = (videoMemory[80 * y + x] & 0x0F00) << 4
                            | (videoMemory[80 * y + x] & 0xF000) >> 4
                            | (videoMemory[80 * y + x] & 0x00FF);
}