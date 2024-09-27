#include "Potentiometer.h"

void Potentiometer::init(int potentiometerPin) { 
    this->potentiometerPin = potentiometerPin;
}

int Potentiometer::getValue() {
    return analogRead(potentiometerPin);
}
