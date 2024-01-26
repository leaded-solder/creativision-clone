#include "pico/stdlib.h"

#include "snespad.h"

void SnesPad::update() {
    // Store previous read
    this->previousRead = this->thisRead;

    // Cycle latch on the controller
    gpio_put(SNES_LATCH_GPIO, 1);
    gpio_put(SNES_LATCH_GPIO, 0);

    // Clock the shift register in
    int value = 0;

    // We're just going to read the 12 basic buttons, if you want to
    // implement something like the mouse you can extend this
    for(unsigned char i = 0; i < SNES_NUM_BUTTONS; ++i) {
        gpio_put(SNES_CLOCK_GPIO, 0);

        if(gpio_get(SNES_DATA_GPIO)) {
            value = 0x1 | (value << 1);
        }
        else {
            value = 0x0 | (value << 1);
        }

        gpio_put(SNES_CLOCK_GPIO, 1);
    }

    this->thisRead = (SnesButtonState)value;
}

SnesButtonState SnesPad::get() {
    return this->thisRead;
}

SnesButtonState SnesPad::getNew() {
    return (SnesButtonState)(this->thisRead ^ this->previousRead);
}

SnesPad::SnesPad() {
    this->thisRead = (SnesButtonState)0;
    this->previousRead = (SnesButtonState)0;
}