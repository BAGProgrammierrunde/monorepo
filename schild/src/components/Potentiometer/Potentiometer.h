// TODO Umstellen auf Component
#pragma once

#include <Arduino.h>

class Potentiometer {
public:
    void init(int potentiometerPin);
    int getValue();
private:    
    int potentiometerPin;
};
