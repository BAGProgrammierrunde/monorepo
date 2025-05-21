#include "Button.hpp"

#include "Data_Types/any_callable.hpp"

Button::Button(unsigned int pPin) : Component({pPin}) {
    pinMode(pins[0], INPUT_PULLUP);
    lastFallingTime = 0;

    attachInterruptArg(pins[0], [](void* pArg) -> void IRAM_ATTR {
        Button* button = static_cast<Button*>(pArg);
        ulong fallingTime = millis();
        if (fallingTime - button->lastFallingTime > Button::DEBOUNCE_MILLIS)
        {
            xTaskCreate(
                [](void* pTaskArg) -> void {
                    static_cast<Button*>(pTaskArg)->callClickFunction();
                    vTaskDelete(NULL);
                },
                "ButtonCallback", // Name of the task (e.g. for debugging)
                2048, // Stack size (bytes)
                button, // Parameter to pass
                1, // Task priority
                NULL // Task handle
            );

            button->lastFallingTime = fallingTime;
        }
    }, this, FALLING);
}

bool Button::isPressed() const {
    return (digitalRead(pins[0]) == LOW);
}

template <typename CallableT>
CallableT& Button::getRawClickFunctionCallableRef() {
    return clickFunc.getRawCallableRef<CallableT>();
}

void Button::setClickFunction(me::any_callable<void()> pClickFunction) {
    clickFunc = pClickFunction;
}

void Button::clearClickFunction() {
    clickFunc.clear();
}

void Button::callClickFunction() {
    clickFunc();
}
