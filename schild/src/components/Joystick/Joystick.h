#pragma once

#include <Arduino.h>
#include <components/Button/Button.h>
#include <components/Structure/Component.h>

class Joystick : public Component<2> {
private:
    float threshold;

    float readAsPercent(unsigned int pPin);

public:
    Button button;
    
    explicit Joystick(unsigned int pPinX, unsigned int pPinY, unsigned int _pPinButton, float pThreshold);

    float getX();
    float getY();
};
