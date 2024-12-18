#include "Button.hpp"

#include "data/any_callable.hpp"

Button::Button(unsigned int pin) : Component({pin}) {
    pinMode(pins[0], INPUT_PULLUP);
    pressed = false;
    fallingTime = 0;
    lastFallingTime = 0;

    attachInterruptArg(pins[0], [](void* arg) -> void IRAM_ATTR {
        Button* button = static_cast<Button*>(arg);
        button->fallingTime = millis();
        if (button->fallingTime - button->lastFallingTime > Button::DEBOUNCE_TIME) {
            xTaskCreate(
                [](void* arg) -> void {
                    Button* button = static_cast<Button*>(arg);
                    button->callCallback();
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

bool Button::isPressed() {
    return pressed;
}

void Button::setCallback(me::any_callable<void ()> pCallbackFunction) {
    callbackFunc = pCallbackFunction;
}

void Button::clearCallback() {
    callbackFunc.clear();
}

void Button::callCallback() {
    callbackFunc();
}
