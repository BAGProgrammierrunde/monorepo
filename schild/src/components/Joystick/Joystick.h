//
// Created by fabian on 17.09.24.
//

#ifndef SCHILD_JOYSTICK_H
#define SCHILD_JOYSTICK_H


class Joystick {
public:
    void init();
    void init(int switchPin, int xPin, int yPin);
    int getX();
    int getXPercent();
    int getY();
    int getYPercent();
    int getSwitch();
private:
    void setupPins();
};


#endif //SCHILD_JOYSTICK_H
