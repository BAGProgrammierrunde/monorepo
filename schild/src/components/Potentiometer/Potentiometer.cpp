#include "Potentiometer.h"

void Potentiometer::init(int potentiometerPin) { 
    pinMode(potentiometerPin, INPUT);
    this->potentiometerPin = potentiometerPin;
}

int Potentiometer::getValue() {
    return analogRead(potentiometerPin);
}
