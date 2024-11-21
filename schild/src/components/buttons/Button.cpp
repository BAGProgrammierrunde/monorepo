#include "Button.h"

Button::Button(const int pinNumber) {
    pin = pinNumber;
    pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() const {
    return digitalRead(pin) == LOW;
}
