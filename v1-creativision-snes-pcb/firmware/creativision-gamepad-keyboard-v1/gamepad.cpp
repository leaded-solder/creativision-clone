#include <stdio.h>
#include "pico/stdlib.h"

#include "snespad.h"

#include "ps2read.pio.h"

SnesPad *pad = NULL;

#define PIN_MATRIX_A 3 // "pin 2," right joystick   (~PA0)
#define PIN_MATRIX_B 2 // "pin 1," right joystick   (~PA1)
#define PIN_MATRIX_C 1 // "pin 10," left joystick   (~PA2)
#define PIN_MATRIX_D 0 // "pin 9," left joystick    (~PA3)
// the select lines are mutually exclusive

#define PIN_PS2_DATA 28
#define PIN_PS2_CLOCK 27

#define PIN_LED 25

// Pins GPIO0..GPIO3 inclusive (active low, 0 = selected)
#define INPUT_MATRIX_SELECTS 0x0000f
// Pins GPIO4..GPIO19 inclusive (active low, 0 = pressed)
#define OUTPUT_MATRIX_MASK 0xffff0

unsigned short last_matrix_row; // current matrix row being read

// matrix is A-H, A-H (16 bits)

// GPIO4 -> Right A (pin 3)
// GPIO5 -> Right B (pin 4)
// GPIO6 -> Right C (pin 5)
// GPIO7 -> Right D
// GPIO8 -> Right E
// GPIO9 -> Right F
// GPIO10 -> Right G
// GPIO11 -> Right H

// GPIO12 -> Left A (pin 1)
// GPIO13 -> Left B (pin 2)
// GPIO14 -> Left C (pin 3)
// GPIO15 -> Left D (pin 4)
// GPIO16 -> Left E (pin 5)
// GPIO17 -> Left F (pin 6)
// GPIO18 -> Left G (pin 7)
// GPIO19 -> Left H (pin 8)

// TODO: keyboard/gamepad state
// some kind of keymap structure that is an array to set pins from, where each item is a pointer into the "key state" array
// then we can just drive the mask

// TODO: it probably makes sense to make the keycode a u8 (for row index) and then u16 (for bitmask)

/**
 * The entire matrix for all rows (bitmask for RIGHT-LEFT, see above)
 */
unsigned short full_matrix[4];

void trouble() {
    // WE HAVE TROUBLE
    gpio_put(PIN_LED, 1);
}

// Button masks, relative to the "button mask" portion of GPIO (shifted)
#define MASK_RIGHT_A 0x0001
#define MASK_RIGHT_B 0x0002
#define MASK_RIGHT_C 0x0004
#define MASK_RIGHT_D 0x0008
#define MASK_RIGHT_E 0x0010
#define MASK_RIGHT_F 0x0020
#define MASK_RIGHT_G 0x0040
#define MASK_RIGHT_H 0x0080

#define MASK_LEFT_A 0x0100
#define MASK_LEFT_B 0x0200
#define MASK_LEFT_C 0x0400
#define MASK_LEFT_D 0x0800
#define MASK_LEFT_E 0x1000
#define MASK_LEFT_F 0x2000
#define MASK_LEFT_G 0x4000
#define MASK_LEFT_H 0x8000

// also buggy
#define SET_MATRIX2(row, mask) full_matrix[row] &= ~(mask)
// buggy
#define UNSET_MATRIX2(row, mask) full_matrix[row] |= (mask)

void loop(PIO& pio, uint& sm) {
    pad->update();

    SnesButtonState state = pad->get();

    // TODO: Set/reset appropriate matrix pins for controller
    //printf("PA2 matrix = %04X\n", full_matrix[PIN_MATRIX_C]);

    if(state.buttons[SNES_B]) {
        // Fire left (right joystick) PA3 to left-H
        SET_MATRIX2(PIN_MATRIX_D, MASK_LEFT_H);
    }
    else {
        UNSET_MATRIX2(PIN_MATRIX_D, MASK_LEFT_H);
    }

    /*

    if(state.buttons[SNES_A]) {
        // Fire Right (right joystick:) PA3 to left-H
        SET_MATRIX2(PIN_MATRIX_D, MASK_LEFT_H);
    }
    else {
        UNSET_MATRIX2(PIN_MATRIX_D, MASK_LEFT_H);
    }

    if(state.buttons[SNES_X]) {
        // Fire right (left joystick:) pin 3 (F) to pin 2 (PA0)
        SET_MATRIX2(PIN_MATRIX_A, MASK_RIGHT_F);
    }
    else {
        UNSET_MATRIX2(PIN_MATRIX_A, MASK_RIGHT_F);
    }

    if(state.buttons[SNES_Y]) {
        // Fire left (left joystick:) pin 3 (F) to pin 1 (PA1)
        SET_MATRIX2(PIN_MATRIX_B, MASK_RIGHT_F);
    }
    else {
        UNSET_MATRIX2(PIN_MATRIX_B, MASK_RIGHT_F);
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
        // Send Z - pin 5 (E) and pin 7 (G) on left controller to PA2
        // TODO: what's the bit math here?
    }
    else {

    }

    */

    // TODO: Read PS/2 keyboard as well (ideally interrupt-free, blocking-free) - detect keyup, keydown
    // FIXME: Sometimes the thing is asking for nothing at all (PA0..PA3 all high)

    // Detect PA0..PA3 inputs changing and then offer up a new matrix
    unsigned short new_matrix_row = 0;
    // It's active low, so we're inverting it so the active row is active
    
    //gpio_set_dir_out_masked(OUTPUT_MATRIX_MASK);

    new_matrix_row = ~(gpio_get_all()) & INPUT_MATRIX_SELECTS; // HACK - probably buggy too
    if(new_matrix_row & 0x01) { new_matrix_row = 0; }
    else if(new_matrix_row & 0x02) { new_matrix_row = 1; }
    else if(new_matrix_row & 0x04) { new_matrix_row = 2; }    
    else if(new_matrix_row & 0x08) { new_matrix_row = 3; } // TURBO HACK
    else {
        //gpio_set_dir_in_masked(OUTPUT_MATRIX_MASK);
    }

    if(last_matrix_row != new_matrix_row) {        
        printf("Requested output has changed, now %i\n", new_matrix_row);
        last_matrix_row = new_matrix_row;
        gpio_put(PIN_LED, 1);
        gpio_put_masked(OUTPUT_MATRIX_MASK, full_matrix[last_matrix_row] << 4); // HACK
    }
    else {
        gpio_put(PIN_LED, 0);
    }
    
    
    // Change outputs (if needed, this will also pick up key changes during a row)

    // TODO: Endianness? Do we need to change the order of the matrix to LEFTRIGHT instead of RIGHTLEFT bytes?
    // remember that this is also active low

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
    
    stdio_init_all();

    printf("Hello, world\n");

    // Set directions
    gpio_init_mask(INPUT_MATRIX_SELECTS | OUTPUT_MATRIX_MASK);

    // debug led
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    
    gpio_set_dir_in_masked(INPUT_MATRIX_SELECTS);
    gpio_pull_up(PIN_MATRIX_A);
    gpio_pull_up(PIN_MATRIX_B);
    gpio_pull_up(PIN_MATRIX_C);
    gpio_pull_up(PIN_MATRIX_D);
    gpio_set_dir_out_masked(OUTPUT_MATRIX_MASK);

    // Pull everything high. Is this better done with pullups?
    gpio_put_masked(OUTPUT_MATRIX_MASK, OUTPUT_MATRIX_MASK);

    // Clear matrix state
    for(unsigned char i = 0; i < 4; ++i) {
        full_matrix[i] = 0xffff; // it's active low.
    }

    last_matrix_row = 0x0; // force it to re-initialize on first pull

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
