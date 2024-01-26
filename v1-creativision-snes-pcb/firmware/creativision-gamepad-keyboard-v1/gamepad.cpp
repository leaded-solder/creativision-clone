#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

SnesPad pad;

void loop() {
    pad.update();

    SnesButtonState state = pad.get();
    
    if((state & SNES_B) != 0) {
        puts("Button B is down");
    }
    
    // TODO: test new state
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

        Wizz PA0    - GPIO 0
        Wizz PA1    - GPIO 1
        Wizz PA2    - GPIO 2
        Wizz PA3    - GPIO 3

        Wizz RJoy   - GPIO 4..11 inclusive for A-H
        Wizz LJoy   - GPIO 12..19 inclusive for A-H
    */

    // TODO: Identify RJoy/LJoy matrix inputs changing
    // TODO: SNES controller reading and state
    // TODO: Keyboard reading and state

    stdio_init_all();

    // init SNES

    while(true) {
        loop();
    }

    return 0;
}
