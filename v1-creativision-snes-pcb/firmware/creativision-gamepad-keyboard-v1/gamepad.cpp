#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

SnesPad *pad = NULL;

#define PIN_MATRIX_A 0 // "pin 2," right joystick   (PA0)
#define PIN_MATRIX_B 1 // "pin 1," right joystick   (PA1)
#define PIN_MATRIX_C 2 // "pin 10," left joystick   (PA2)
#define PIN_MATRIX_D 3 // "pin 9," left joystick    (PA3)

unsigned short last_matrix_row; // current matrix row being read

void loop() {
    pad->update();

    SnesButtonState state = pad->get();
    
    if(state.buttons[SNES_B]) {
        puts("Button B is down");
    }
    if(state.buttons[SNES_A]) {
        puts("Button A is down");
    }
    if(state.buttons[SNES_X]) {
        puts("Button X is down");
    }
    if(state.buttons[SNES_Y]) {
        puts("Button Y is down");
    }
    if(state.buttons[SNES_LEFT]) {
        puts("Button Left is down");
    }
    if(state.buttons[SNES_L]) {
        puts("Left Shoulder is down");
    }
    if(state.buttons[SNES_R]) {
        puts("Right Shoulder is down");
    }
    if(state.buttons[SNES_SELECT]) {
        puts("Select is down");
    }
    if(state.buttons[SNES_START]) {
        puts("Start is down");
    }

    // TODO: Read PS/2 keyboard as well (ideally interrupt-free)
    
    // Detect PA0..PA3 inputs changing and then offer up a new matrix
    unsigned short new_matrix_row = 0;
    for(unsigned short i = 0; i < 4; ++i) {
        new_matrix_row = (new_matrix_row << 1) | gpio_get(PIN_MATRIX_A + i);
    }

    if(last_matrix_row != new_matrix_row) {
        // TODO: Change outputs
        printf("Output has changed, now %i\n", new_matrix_row);
        last_matrix_row = new_matrix_row;
    }
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

    // TODO: Turn off all outputs

    last_matrix_row = 0xff; // force it to re-initialize on first pull

    // init SNES
    puts("Hiya");

    pad = new SnesPad();

    while(true) {
        loop();
    }

    return 0;
}
