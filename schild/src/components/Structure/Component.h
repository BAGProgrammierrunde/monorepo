#pragma once

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
private:
    unsigned int pins[PinCount];

public:
    Component(unsigned int pPins[PinCount]);
    
    unsigned int getPinCount();
    bool usesPin(unsigned int pPin);
    int getPin(unsigned int pIndex);
};