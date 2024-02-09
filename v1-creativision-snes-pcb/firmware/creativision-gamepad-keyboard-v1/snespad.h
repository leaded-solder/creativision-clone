#ifndef _SNESPAD_H_
#define _SNESPAD_H_

#define SNES_DATA_GPIO 22
#define SNES_LATCH_GPIO 21
#define SNES_CLOCK_GPIO 20

enum SnesButton {
    SNES_B = 0,
    SNES_Y = 1,
    SNES_SELECT = 2,
    SNES_START = 3,
    SNES_UP = 4,
    SNES_DOWN = 5,
    SNES_LEFT = 6,
    SNES_RIGHT = 7,
    SNES_A = 8,
    SNES_X = 9,
    SNES_L = 10,
    SNES_R = 11 
};

#define SNES_NUM_BUTTONS 12

struct SnesButtonState {
    bool buttons[SNES_NUM_BUTTONS];
};

class SnesPad {
public:
    SnesPad();
public:
    // latch SNES button state
    void update();
    // read last SNES button state
    SnesButtonState get();
private:
    SnesButtonState thisRead;
};

#endif