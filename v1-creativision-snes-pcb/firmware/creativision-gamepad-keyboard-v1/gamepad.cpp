#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

#include "ps2read.pio.h"

SnesPad *pad = NULL;

#define PIN_MATRIX_A 0 // "pin 2," right joystick   (~PA0)
#define PIN_MATRIX_B 1 // "pin 1," right joystick   (~PA1)
#define PIN_MATRIX_C 2 // "pin 10," left joystick   (~PA2)
#define PIN_MATRIX_D 3 // "pin 9," left joystick    (~PA3)
// the select lines are mutually exclusive

#define PIN_PS2_DATA 28
#define PIN_PS2_CLOCK 27

// Pins GPIO0..GPIO3 inclusive (active low)
#define INPUT_MATRIX_SELECTS 0x0000f
// Pins GPIO4..GPIO19 inclusive (active high)
#define OUTPUT_MATRIX_MASK 0xffff0

unsigned short last_matrix_row; // current matrix row being read

// matrix is A-H, A-H (16 bits)

// GPIO4 -> Right A
// GPIO5 -> Right B
// GPIO6 -> Right C
// GPIO7 -> Right D
// GPIO8 -> Right E
// GPIO9 -> Right F
// GPIO10 -> Right G
// GPIO11 -> Right H

// GPIO12 -> Left A
// GPIO13 -> Left B
// GPIO14 -> Left C
// GPIO15 -> Left D
// GPIO16 -> Left E
// GPIO17 -> Left F
// GPIO18 -> Left G
// GPIO19 -> Left H

// TODO: keyboard/gamepad state
// some kind of keymap structure that is an array to set pins from, where each item is a pointer into the "key state" array
// then we can just drive the mask

// TODO: it probably makes sense to make the keycode a u8 (for row index) and then u16 (for bitmask)

/**
 * The entire matrix for all rows (bitmask for RIGHT-LEFT, see above)
 */
unsigned short full_matrix[4];

void loop(PIO& pio, uint& sm) {
    pad->update();

    SnesButtonState state = pad->get();

    // TODO: Set/reset appropriate matrix pins for controller
    
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

    // TODO: Read PS/2 keyboard as well (ideally interrupt-free, blocking-free) - detect keyup, keydown
    
    // Detect PA0..PA3 inputs changing and then offer up a new matrix
    unsigned short new_matrix_row = 0;
    for(unsigned short i = 0; i < 4; ++i) {
        // It's active low, so we're inverting it so the active row is active
        new_matrix_row = ~(gpio_get_all() & INPUT_MATRIX_SELECTS);
    }

    if(last_matrix_row != new_matrix_row) {        
        printf("Requested output has changed, now %i\n", new_matrix_row);
        last_matrix_row = new_matrix_row;
    }

    // Change outputs (if needed, this will also pick up key changes during a row)
    gpio_put_masked(OUTPUT_MATRIX_MASK, full_matrix[new_matrix_row] << 4);

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

    for(unsigned char i = 0; i < 4; ++i) {
        full_matrix[i] = 0;
    }

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
