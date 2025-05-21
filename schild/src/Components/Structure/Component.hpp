#pragma once

#include <array>

// No seperate .cpp implementation file because of template base class.
// See: https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
    unsigned int pins[PinCount];

public:
    explicit Component(const unsigned int (&pPins)[PinCount]) { // const reference to also accept rvalues like {1, 2, 3}
        std::copy(std::begin(pPins), std::end(pPins), std::begin(pins));
    }

    unsigned int getPinCount() const {
        return PinCount;
    }

    int getPin(unsigned int pIndex) const {
        return (pIndex < PinCount ? pins[pIndex] : -1);
    }

    bool usesPin(unsigned int pPin) const {
        for (int i = 0; i < PinCount; i++)
            if (pins[i] == pPin) return true;
        return false;
    }
};


/*
//template class Component<1>;
//template class Component<2>;
//template class Component<6>;

// Component.cpp:

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
*/