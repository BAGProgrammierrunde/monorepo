#pragma once

#include <Arduino.h>
#include "Components/Button/Button.hpp"
#include "Components/Structure/Component.hpp"

class Joystick : public Component<2> {
private:
    Button button;
    float threshold;

    float readCoordAsPercent(bool pXElseY) const;

public:
    Joystick(unsigned int _pPinX, unsigned int _pPinY, unsigned int _pPinButton, float pThreshold = 0);
    float getX() const;
    float getY() const;
    float getThreshold() const;
    Button* getButtonRef();
    void setThreshold(float pThreshold);
};
