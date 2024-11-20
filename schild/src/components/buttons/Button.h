#pragma once
#include "Arduino.h"

class Button {
public:
    Button(int pinNumber);  
    bool isPressed();      

private:
    int pin; 
};
