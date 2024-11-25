#include "Button.h"

Button::Button(const int pinNumber) {
    pin = pinNumber;
    pinMode(pin, INPUT_PULLUP);
    callback = doNothing;
}

void Button::init() {
    attachInterruptArg(pin, [](void *arg) -> void {
        static_cast<Button *>(arg)->internalCallback();
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

void Button::doCallback() {
    callback();
}

void _internalInternal(void* arg) {
    Serial.println("do callback..");
    static_cast<Button *>(arg)->doCallback();
    Serial.println("do callback done");
    vTaskDelete(NULL);
}

void IRAM_ATTR Button::internalCallback() {
    fallingTime = millis();
    if (fallingTime - lastFallingTime > BUTTON_DEBOUNCE_TIME) {
        Serial.println("Start doCallback task..");
        xTaskCreate(
            _internalInternal, // Function name of the task
            "ButtonCallback", // Name of the task (e.g. for debugging)
            2048, // Stack size (bytes)
            this, // Parameter to pass
            1, // Task priority
            NULL // Task handle
        );
        Serial.println("doCallback task created");

        // callback();
        lastFallingTime = fallingTime;
    }
}
