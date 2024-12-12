#pragma once

class ComponentBase {
public:
  ComponentBase() {
  }
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
  unsigned int pins[PinCount];
public:
  template<typename ... Pins>
  explicit Component(Pins... pPins) : pins{static_cast<unsigned int>(pPins)...} {
    static_assert(sizeof...(pPins) == PinCount, "Incorrect number of pins");
  }
  unsigned int getPinCount() const;
  bool usesPin(unsigned int pPin) const;
  int getPin(unsigned int pIndex) const;
};

template class Component<1>;
template class Component<2>;
template class Component<6>;
