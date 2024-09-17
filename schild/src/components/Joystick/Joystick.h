//
// Created by fabian on 17.09.24.
//

#ifndef SCHILD_JOYSTICK_H
#define SCHILD_JOYSTICK_H


class Joystick {
public:
    void init(int xOffset = 0, int yOffset = 0);
    void init(int switchPin, int xPin, int yPin, int xOffset = 0, int yOffset = 0);
    int getX();
    int getXPercent(int offset = 0);
    int getY();
    int getYPercent(int offset = 0);
    bool getSwitch();
private:
    void setupPins();
    int getPercent(int value, int offset);
};


#endif //SCHILD_JOYSTICK_H
