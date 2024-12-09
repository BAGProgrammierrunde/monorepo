#pragma once

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
    unsigned int pins[PinCount];

public:
  template <typename... Pins>
  explicit Component(Pins... pins) : pins{pins...} { }
    
    unsigned int getPinCount() const;
    bool usesPin(unsigned int pPin) const;
    int getPin(unsigned int pIndex);
};
