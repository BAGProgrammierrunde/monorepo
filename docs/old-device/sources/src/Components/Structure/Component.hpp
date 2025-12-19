#pragma once

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
    Component(const unsigned int (&pPins)[PinCount]) { // const reference to also accept rvalues like {1, 2, 3}
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
