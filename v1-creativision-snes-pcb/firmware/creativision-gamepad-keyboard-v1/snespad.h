#ifndef _SNESPAD_H_
#define _SNESPAD_H_

#define SNES_DATA_GPIO 22
#define SNES_LATCH_GPIO 21
#define SNES_CLOCK_GPIO 20

enum SnesButtonState {
    SNES_B = 0x800,
    SNES_Y = 0x400,
    SNES_SELECT = 0x200,
    SNES_START = 0x100,
    SNES_UP = 0x80,
    SNES_DOWN = 0x40,
    SNES_LEFT = 0x20,
    SNES_RIGHT = 0x10,
    SNES_A = 0x08,
    SNES_X = 0x04,
    SNES_L = 0x02,
    SNES_R = 0x01 
};

#define SNES_NUM_BUTTONS 12

class SnesPad {
public:
    SnesPad();
public:
    // latch SNES button state
    void update();
    // read last SNES button state
    SnesButtonState get();
    // get "new" buttons this frame (we don't care for the Wizzard but it's easy enough to do)
    SnesButtonState getNew();
private:
    SnesButtonState thisRead;
    SnesButtonState previousRead;
};

#endif