#include "Button.hpp"

Button::Button(unsigned int _pPin) : Component({_pPin}) {
    pinMode(pins[0], INPUT_PULLUP);
    pressed = false;
    callbackFunc = &defaultCallback;
    fallingTime = 0;
    lastFallingTime = 0;
}

void Button::init() {
    attachInterruptArg(pins[0], [](void* pArg) -> void IRAM_ATTR {
        Button* thisRef = (Button*)pArg;
        thisRef->fallingTime = millis();
        if (thisRef->fallingTime - thisRef->lastFallingTime > Button::DEBOUNCE_TIME)
        {
            xTaskCreate(
                [](void* pThreadArg) -> void {
                    ((Button*)pThreadArg)->callCallback();
                    vTaskDelete(NULL);
                },
                "ButtonCallback", // Name of the task (e.g. for debugging)
                2048, // Stack size (bytes)
                thisRef, // Parameter to pass
                1, // Task priority
                NULL // Task handle
            );

            thisRef->lastFallingTime = thisRef->fallingTime;
        }
    }, this, FALLING);
}

void Button::defaultCallback(Button* pThisRef) {
    Serial.println("Button on pin " + String(pThisRef->getPin(0)) + " clicked");
}

bool Button::isPressed() {
    return pressed;
}

void Button::setCallback(CallbackFuncPtr pCallbackFunction) {
    callbackFunc = pCallbackFunction;
}

void Button::clearCallback() {
    callbackFunc = &defaultCallback;
}

void Button::callCallback() {
    callbackFunc(this);
}
