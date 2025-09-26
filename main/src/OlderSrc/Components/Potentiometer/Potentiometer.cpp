#include "Potentiometer.hpp"

Potentiometer::Potentiometer(unsigned int _pPin) : Component({_pPin}) {
    pinMode(pins[0], INPUT);
}

float Potentiometer::getValue() const {
    return (analogRead(pins[0]) / 4096.f); // TODO: Check if 4096 is REALLY our poti's maximum analog read value!
}
