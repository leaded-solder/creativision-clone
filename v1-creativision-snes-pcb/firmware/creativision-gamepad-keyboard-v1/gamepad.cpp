#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

SnesPad *pad = NULL;

void loop() {
    pad->update();

    SnesButtonState state = pad->get();
    
    if(state.buttons[SNES_B]) {
        // if this is wrong, the enum's backwards
        puts("Button B is down");
    }
    if(state.buttons[SNES_LEFT]) {
        puts("Button Left is down");
    }
    if(state.buttons[SNES_L]) {
        puts("Left Shoulder is down");
    }
    

    // right shoulder seems to be stuck? is my reading off?

    // TODO: Detect PA0..PA3 inputs changing and then offer up a new matrix
}

int main()
{
    /*
        Pin assignments (TODO: Move this into a header)
        PS/2 clock  - GPIO 27
        PS/2 data   - GPIO 28
        SNES data   - GPIO 22
        SNES latch  - GPIO 21
        SNES clock  - GPIO 20

        Wizz PA0    - GPIO 0 ("pin 2," right joystick)
        Wizz PA1    - GPIO 1 ("pin 1," right joystick)
        Wizz PA2    - GPIO 2 ("pin 10," left joystick)
        Wizz PA3    - GPIO 3 ("pin 9," left joystick)

        Wizz RJoy   - GPIO 4..11 inclusive for A-H
        Wizz LJoy   - GPIO 12..19 inclusive for A-H
    */

    // TODO: Identify RJoy/LJoy matrix inputs changing
    // TODO: Test SNES controller
    // TODO: Keyboard reading and state

    stdio_init_all();


    // init SNES
    puts("Hiya");

    pad = new SnesPad();

    while(true) {
        loop();
    }

    return 0;
}
