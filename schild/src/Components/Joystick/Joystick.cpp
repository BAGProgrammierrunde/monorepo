#include "Joystick.hpp"

Joystick::Joystick(unsigned int pPinX, unsigned int pPinY, unsigned int _pPinButton, float pThreshold) : Component(pPinX, pPinY), button(_pPinButton) {
    pinMode(pins[0], INPUT);
    pinMode(pins[1], INPUT);
    threshold = pThreshold;
}

float Joystick::getX() {
    return readAsPercent(pins[0]);
}

float Joystick::getY() {
    return readAsPercent(pins[1]);
}

// Returns range -1 to 1
float Joystick::readAsPercent(unsigned int pPin) {
    int analogValue = analogRead(pPin); // Returns value from 0 to 4095
    return (analogValue > threshold ? analogValue / 4095.f * 2.f - 1 : 0);
    
    //if (analogValue <= 2047) return map(analogValue, 0, 2047, 0, 50 + offset);
    //else return map(analogValue, 2048, 4095, 50 + offset, 100);
}
