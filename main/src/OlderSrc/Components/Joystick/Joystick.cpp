#include "Joystick.hpp"

Joystick::Joystick(unsigned int _pPinX, unsigned int _pPinY, unsigned int _pPinButton, float pThreshold) : Component({_pPinX, _pPinY}), button(_pPinButton) {
    pinMode(pins[0], INPUT);
    pinMode(pins[1], INPUT);
    threshold = pThreshold;
}

float Joystick::readCoordAsPercent(bool pXElseY) const { // Returns range -1 to 1
    int analogValue = analogRead(pins[!pXElseY]); // Returns value from 0 to 4095
    return (analogValue > threshold ? (analogValue / 4095.f) * 2.f - 1 : 0);
}

float Joystick::getX() const {
    return readCoordAsPercent(true);
}

float Joystick::getY() const {
    return readCoordAsPercent(false);
}

float Joystick::getThreshold() const {
    return threshold;
}

Button* Joystick::getButtonRef() {
    return &button;
}

void Joystick::setThreshold(float pThreshold) {
    threshold = pThreshold;
}