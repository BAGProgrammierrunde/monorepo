#pragma once
#include <array>

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
    unsigned int pins[PinCount];

public:
    explicit Component(std::array<unsigned int, PinCount> pPins);
    
    unsigned int getPinCount();
    bool usesPin(unsigned int pPin);
    int getPin(unsigned int pIndex);
};