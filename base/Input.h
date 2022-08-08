//
// Created by Mateusz Lisik on 03/08/2022.
//

#ifndef PINBALL_INPUT_H
#define PINBALL_INPUT_H


#include "Private.h"
#include "Keyboard.h"
#include "GPIO.h"

class Input {
public:
    Input();
    ~Input();
    void poll();
    static Input * getInstance();

private:
    Keyboard* keyboard;
    GPIO* gpio;
    static Input* instance;

};


#endif //PINBALL_INPUT_H
