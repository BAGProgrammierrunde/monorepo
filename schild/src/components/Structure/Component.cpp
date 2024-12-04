#include "Component.h"

template <unsigned int PinCount>
Component::Component(unsigned int pPins[PinCount]) : ComponentBase() {
    pins = pPins;
}

template <unsigned int PinCount>
unsigned int Component::getPinCount() {
    return PinCount;
}

template <unsigned int PinCount>
bool Component::usesPin(unsigned int pPin) {
    for (int i = 0; i < PinCount; i++)
        if (pins[i] == pPin) return true;
    return false;
}

template <unsigned int PinCount>
int Component::getPin(unsigned int pIndex) {
    return (pIndex < PinCount ? pins[pIndex] : -1);
}

template <unsigned int PinCount>
void Component::setPins(unsigned int pPins[PinCount]) {
    pins = pPins;
}

template <unsigned int PinCount>
void Component::setPin(unsigned int pIndex, unsigned int pPin) {
    if (pIndex < PinCount) pins[pIndex] = pPins;
}