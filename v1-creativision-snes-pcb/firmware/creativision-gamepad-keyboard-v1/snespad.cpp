#include "pico/stdlib.h"

#include "snespad.h"

// Timing taken from https://github.com/MickGyver/DaemonBite-Retro-Controllers-USB/blob/master/SNESControllersUSB/SNESControllersUSB.ino
#define SLEEP_LATCH 12
#define SLEEP_CLOCK 6

void SnesPad::update() {
    // Cycle latch on the controller
    gpio_put(SNES_LATCH_GPIO, 1);
    sleep_us(SLEEP_LATCH);
    gpio_put(SNES_LATCH_GPIO, 0);
    sleep_us(SLEEP_CLOCK);

    // Clock the shift register in now
    // button B is always sent immediately
    this->thisRead.buttons[SNES_B] = !gpio_get(SNES_DATA_GPIO);

    // SNES expects full 16 cycles
    for(unsigned char i = 1; i < 16; ++i) {
        gpio_put(SNES_CLOCK_GPIO, 0);
        sleep_us(SLEEP_CLOCK);

        if(i < SNES_NUM_BUTTONS) {
            this->thisRead.buttons[i] = !gpio_get(SNES_DATA_GPIO);
        }

        gpio_put(SNES_CLOCK_GPIO, 1);
        sleep_us(SLEEP_CLOCK);
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
    gpio_put(SNES_CLOCK_GPIO, 1);
}