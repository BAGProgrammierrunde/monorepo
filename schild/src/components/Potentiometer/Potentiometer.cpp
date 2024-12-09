#include "Potentiometer.h"

Potentiometer::Potentiometer(unsigned int pin) : Component(pin) {
    pinMode(pin, INPUT);
}

int Potentiometer::getValue() {
    return analogRead(pins[0]);
}
