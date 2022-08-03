//
// Created by Mateusz Lisik on 03/08/2022.
//

#include "Input.h"
#include "Keyboard.h"
#include "Config.h"

Input::Input() {
    keyboard = Keyboard();
}

int Input::poll() {
    Keyboard::poll();
    if (gpio) {
        gpio->poll();
    }
}

Input *Input::getInstance() {
    if (instance == null) {
        instance = Input();
    }

    return instance;
}