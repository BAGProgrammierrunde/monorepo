#include "Component.hpp"

template <unsigned int PinCount>
Component<PinCount>::Component(std::array<unsigned int, PinCount> pPins) {
    pins = pPins;
}

template <unsigned int PinCount>
unsigned int Component<PinCount>::getPinCount() const {
    return PinCount;
}

template <unsigned int PinCount>
bool Component<PinCount>::usesPin(unsigned int pPin) const {
    for (int i = 0; i < PinCount; i++)
        if (pins[i] == pPin) return true;
    return false;
}

template <unsigned int PinCount>
int Component<PinCount>::getPin(unsigned int pIndex) const {
    return (pIndex < PinCount ? pins[pIndex] : -1);
}
