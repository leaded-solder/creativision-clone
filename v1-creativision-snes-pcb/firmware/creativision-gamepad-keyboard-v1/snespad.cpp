#include "pico/stdlib.h"

#include "snespad.h"

void SnesPad::update() {
    // Cycle latch on the controller
    gpio_put(SNES_LATCH_GPIO, 1);
    sleep_us(12);
    gpio_put(SNES_LATCH_GPIO, 0);
    sleep_us(6);

    // Clock the shift register in now

    // We're just going to read the 12 basic buttons, if you want to
    // implement something like the mouse you can extend this
    for(unsigned char i = 0; i < SNES_NUM_BUTTONS; ++i) {
        gpio_put(SNES_CLOCK_GPIO, 0);
        sleep_us(6);

        this->thisRead.buttons[i] = !gpio_get(SNES_DATA_GPIO);

        gpio_put(SNES_CLOCK_GPIO, 1);
        sleep_us(6);
    }
}

SnesButtonState SnesPad::get() {
    return this->thisRead;
}

SnesPad::SnesPad() {
    for(unsigned int i = 0; i < SNES_NUM_BUTTONS; ++i) {
        thisRead.buttons[i] = false;
    }

    gpio_set_dir(SNES_LATCH_GPIO, GPIO_OUT);
    gpio_set_dir(SNES_CLOCK_GPIO, GPIO_OUT);
    gpio_set_dir(SNES_DATA_GPIO, GPIO_IN);

    gpio_put(SNES_LATCH_GPIO, 0);
    gpio_put(SNES_CLOCK_GPIO, 0);
}