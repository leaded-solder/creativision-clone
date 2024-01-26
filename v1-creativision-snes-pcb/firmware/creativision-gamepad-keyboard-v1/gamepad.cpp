#include <stdio.h>
#include "pico/stdlib.h"

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


    puts("Hello, world!");

    return 0;
}
