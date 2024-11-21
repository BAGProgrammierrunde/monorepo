#pragma once

#include "Arduino.h"

#define BUTTON_DEBOUNCE_TIME 250

typedef void (*voidFuncPtr)();

class Button {
public:
    explicit Button(int pinNumber);
    void init();
    void init(voidFuncPtr callback);
    void attach(voidFuncPtr callback);
    void detach();

    void internalCallback();
private:
    int pin;
    voidFuncPtr callback;
    bool pressed = false;
    ulong fallingTime = 0;
    ulong lastFallingTime = 0;

    static void doNothing();
};
