#pragma once
#include "Arduino.h"

class Button {
public:
    explicit Button(int pinNumber);
    bool isPressed() const;
private:
    int pin;
};
