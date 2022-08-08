//
// Created by Mateusz Lisik on 03/08/2022.
//

#ifndef PINBALL_GPIO_H
#define PINBALL_GPIO_H


#define EM_GPIO_LEFT_PADDLE     0
#define EM_GPIO_RIGHT_PADDLE    1
#define EM_GPIO_RESET_BUTTON    2
#define EM_GPIO_START_BUTTON    3


class GPIO {
    GPIO();
    void setup();

    void shutdown();

public:
    void poll();

private:
    struct gpiod_chip *chip;
    struct gpiod_line_bulk lines;
    struct gpiod_line_bulk bulk;

    EMKey getKeyFromInputLine(int line);

};


#endif //PINBALL_GPIO_H
