#include "Component.h"

template<unsigned int PinCount>
Component<PinCount>::Component(std::array<unsigned int, PinCount> pPins) : ComponentBase() {
    pins = pPins;
}

template <unsigned int PinCount>
unsigned int Component<PinCount>::getPinCount() {
    return PinCount;
}

template <unsigned int PinCount>
bool Component<PinCount>::usesPin(unsigned int pPin) {
    for (int i = 0; i < PinCount; i++)
        if (pins[i] == pPin) return true;
    return false;
}

template <unsigned int PinCount>
int Component<PinCount>::getPin(unsigned int pIndex) {
    return (pIndex < PinCount ? pins[pIndex] : -1);
}
