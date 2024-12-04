#include "Button.h"

Button::Button(unsigned int pPin) : Component({pPin}) {
    pinMode(pins[0], INPUT_PULLUP);
    pressed = false;
    callbackFunc = &defaultCallback;
    fallingTime = 0;
    lastFallingTime = 0;
}

void Button::init() {
    attachInterruptArg(pin, [](void* arg) -> void IRAM_ATTR {
        Button* button = static_cast<Button*>(arg);
        button->fallingTime = millis();
        if (button->fallingTime - button->lastFallingTime > Button::DEBOUNCE_TIME)
        {
            xTaskCreate(
                [](void* arg) -> void {
                    static_cast<Button *>(arg)->callCallback();
                    vTaskDelete(NULL);
                },
                "ButtonCallback", // Name of the task (e.g. for debugging)
                2048, // Stack size (bytes)
                button, // Parameter to pass
                1, // Task priority
                NULL // Task handle
            );

            button->lastFallingTime = button->fallingTime;
        }
    }, this, FALLING);
}

void Button::defaultCallback(ulong pClickTime) {
    Serial.println(pClickTime + "Button clicked");
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
    callbackFunc(millis());
}
