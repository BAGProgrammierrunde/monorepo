#include "Button.h"

Button::Button(const int pinNumber) {
    pin = pinNumber;
    pinMode(pin, INPUT_PULLUP);
    callback = doNothing;
}

void Button::init() {
    attachInterruptArg(pin, [](void* arg) -> void {
        static_cast<Button*>(arg)->internalCallback();
    }, this, FALLING);
}

void Button::init(const voidFuncPtr callback) {
    init();
    attach(callback);
}

void Button::attach(const voidFuncPtr callback) {
    this->callback = callback;
}

void Button::detach() {
    this->callback = doNothing;
}

void Button::doNothing() {
    Serial.println("button currently doing nothing");
}

void IRAM_ATTR Button::internalCallback() {
    fallingTime = millis();
    if (fallingTime - lastFallingTime > BUTTON_DEBOUNCE_TIME)
    {
        callback();
        lastFallingTime = fallingTime;
    }
}
