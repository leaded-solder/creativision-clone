#ifndef _SNESPAD_H_
#define _SNESPAD_H_

#define SNES_DATA_GPIO 22
#define SNES_LATCH_GPIO 21
#define SNES_CLOCK_GPIO 20

enum SnesButtonState {
    SNES_B,
    SNES_Y,
    SNES_SELECT,
    SNES_START,
    SNES_UP,
    SNES_DOWN,
    SNES_LEFT,
    SNES_RIGHT,
    SNES_A,
    SNES_X,
    SNES_L,
    SNES_R // might have to reverse this
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