#include "Component.h"

// TODO Why this resulting in: undefined reference to `Component<1u>::Component<unsigned int>(unsigned int)'
// template<unsigned int PinCount>
// template<typename ... T>
// Component<PinCount>::Component(T... ts) : pins{ts...} {
// }

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

template<unsigned int PinCount>
int Component<PinCount>::getPin(unsigned int pIndex) {
    return pIndex < PinCount ? pins[pIndex] : -1;
}
