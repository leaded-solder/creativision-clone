#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

#include "ps2read.pio.h"

SnesPad *pad = NULL;

#define PIN_MATRIX_A 0 // "pin 2," right joystick   (PA0)
#define PIN_MATRIX_B 1 // "pin 1," right joystick   (PA1)
#define PIN_MATRIX_C 2 // "pin 10," left joystick   (PA2)
#define PIN_MATRIX_D 3 // "pin 9," left joystick    (PA3)

#define PIN_PS2_DATA 28
#define PIN_PS2_CLOCK 27

unsigned short last_matrix_row; // current matrix row being read

// TODO: keyboard/gamepad state
// some kind of keymap structure that is an array to set pins from, where each item is a pointer into the "key state" array

void loop(PIO& pio, uint& sm) {
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

    // TODO: Read PS/2 keyboard as well (ideally interrupt-free, blocking-free)
    
    // Detect PA0..PA3 inputs changing and then offer up a new matrix
    unsigned short new_matrix_row = 0;
    for(unsigned short i = 0; i < 4; ++i) {
        new_matrix_row = (new_matrix_row << 1) | gpio_get(PIN_MATRIX_A + i);
    }

    if(last_matrix_row != new_matrix_row) {
        // TODO: Change outputs
        printf("Requested output has changed, now %i\n", new_matrix_row);
        last_matrix_row = new_matrix_row;
    }

    // probably the way to do this is some kind of table, so that individual characters
    // will turn on parts of the gpio flags (basically the same table that was made for the PDF)
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
    // TODO: Keyboard reading and state

    // TODO: Activate outputs
    // TODO: Set outputs high or low depending on ptr to keyboard state/SNES state
    // TODO: Test SNES controller

    stdio_init_all();

    // TODO: Turn off all outputs

    last_matrix_row = 0xff; // force it to re-initialize on first pull

    // load PS/2 parsing PIO
    // https://raspico.blogspot.com/2022/05/using-pio-to-interface-ps2-keyboard.html
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ps2read_program);
    ps2read_program_init(pio, sm, offset, PIN_PS2_CLOCK);
    pio_sm_set_enabled(pio, sm, true);

    // initialize SNES
    pad = new SnesPad();

    while(true) {
        loop(pio, sm);
    }

    return 0;
}
