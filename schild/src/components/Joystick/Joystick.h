//
// Created by fabian on 17.09.24.
//

#ifndef SCHILD_JOYSTICK_H
#define SCHILD_JOYSTICK_H

#include "Arduino.h"

class Joystick {
public:
    void init(int switchPin, int xPin, int yPin, int xOffset = 0, int yOffset = 0);
    int getX();
    int getXPercent(int offset = 0);
    int getY();
    int getYPercent(int offset = 0);
    bool getSwitch();
private:
    void setupPins();
    int getPercent(int value, int offset);
    int buttonPin;
    int xAxisPin;
    int yAxisPin;

    int xAxisOffset;
    int yAxisOffset;
};


#endif //SCHILD_JOYSTICK_H
