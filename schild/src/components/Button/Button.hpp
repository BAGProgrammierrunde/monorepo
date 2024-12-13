#pragma once

#include <Arduino.h>
#include "Components/Structure/Component.hpp"

class Button : public Component<1> {
public:
    typedef void (*CallbackFuncPtr)(Button*);
    static const ulong DEBOUNCE_TIME = 250;

private:
    bool pressed;
    CallbackFuncPtr callbackFunc;
    ulong fallingTime;
    ulong lastFallingTime;

    static void defaultCallback(Button* pThisRef);

public:
    explicit Button(unsigned int _pPin);
    void init(); // TODO init weg -> in Konstruktor
    
    bool isPressed();
    void setCallback(CallbackFuncPtr pCallbackFunction);
    void clearCallback();
    void callCallback();
};
