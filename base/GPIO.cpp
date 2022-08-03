//
// Created by Mateusz Lisik on 03/08/2022.
//

#include "GPIO.h"
#include <gpiod.h>
#include "Keyboard.h"
#include "Config.h"

GPIO::GPIO() {
    Config::getInstance().get
}

void GPIO::setup() {
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        cerr << "Cannot open gpio chip" << endl;
    }
}

void GPIO::poll() {
    int values[4];
    if (gpiod_line_get_value_bulk(&bulk, values) == -1) {
        //todo error handling
    }

    for (int i = 0; i < 4; i++) {
        Keyboard::generate(getKeyFromInputLine(i, values[i]));
    }
}

int GPIO::getKeyFromInputLine(int line) {
    switch (line) {
        case EM_GPIO_RIGHT_PADDLE:
            return 0;
        case EM_GPIO_LEFT_PADDLE:
            return 0;
        case EM_GPIO_RESET_BUTTON:
            return 0;
        case EM_GPIO_START_BUTTON:
            return 0;
        default:
            return -1;
    }
}


void GPIO::shutdown() {
    if (chip) {
        gpiod_chip_close(chip);
    }

    if (lines) {
        gpiod_line_release_bulk(&lines);
    }
}