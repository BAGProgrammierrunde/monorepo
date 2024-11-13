#pragma once

#include "Arduino.h"

class Button {
public: 
    Button(int);
    bool isPressed(); 
private:
    bool m_isPressed;
    int pin;
};
