#pragma once

#include <Arduino.h>
#include "components/Structure/Component.h"

class Button : public Component<1> {
public:
    typedef void (*CallbackFuncPtr)(ulong);
    static const ulong DEBOUNCE_TIME = 250;

private:
    bool pressed;
    CallbackFuncPtr callbackFunc;
    ulong fallingTime;
    ulong lastFallingTime;

    static void defaultCallback(ulong pClickTime);

public:
    explicit Button(unsigned int pPin);
    void init();

    bool isPressed();

    void setCallback(CallbackFuncPtr pCallbackFunction);
    void clearCallback();
    void callCallback();
};
