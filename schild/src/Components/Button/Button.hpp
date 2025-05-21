#pragma once

#include <Arduino.h>

#include "Components/Structure/Component.hpp"
#include "Data_Types/any_callable.hpp"

class Button : public Component<1> {
private:
    //ulong fallingTime;
    ulong lastFallingTime; // Last time of millis() the button was released. Used for current debounce pause between clicks.
    me::any_callable<void()> clickFunc;

public:
    static const ulong DEBOUNCE_MILLIS = 200;

    explicit Button(unsigned int pPin);
    bool isPressed() const;
    template <typename CallableT>
    CallableT& getRawClickFunctionCallableRef();
    void setClickFunction(me::any_callable<void()> pClickFunction);
    void clearClickFunction();
    void callClickFunction();
};
