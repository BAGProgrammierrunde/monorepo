#pragma once

#include <Arduino.h>
#include "Components/Button/Button.hpp"
#include "Components/Structure/Component.hpp"

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
