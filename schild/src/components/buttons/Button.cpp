#include "Button.h"

Button::Button(int pinNumber) {
    pin = pinNumber;
    pinMode(pin, INPUT_PULLUP); 
}

bool Button::isPressed() {
    return digitalRead(pin) == LOW;
}
