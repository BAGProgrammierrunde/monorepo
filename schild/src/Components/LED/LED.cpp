#include "LED.hpp"

LED::LED(unsigned int _pPin) : Component({_pPin}) {
    pinMode(pins[0], OUTPUT);
}

void LED::setState(bool pOn) {
    digitalWrite(pins[0], pOn);
}
