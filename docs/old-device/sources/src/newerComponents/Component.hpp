#pragma once

class ComponentBase {
public:
    ComponentBase() {}
};

template <unsigned int PinCount>
class Component : public ComponentBase {
protected:
    const std::array<gpio_num_t, PinCount> pins;

public:
    template <typename ...Pins>
    Component(Pins... pPins) : pins({pPins...}) {
        
    }

    constexpr unsigned int getPinCount() const {
        return PinCount;
    }

    template <unsigned int Index>
    constexpr gpio_num_t getPin() const {
        return pins[Index];
    }

    constexpr gpio_num_t getPin(unsigned int pIndex) const {
        return pins[pIndex];
    }

    bool usesPin(gpio_num_t pPin) const {
        for (int i = 0; i < PinCount; i++)
            if (pins[i] == pPin) return true;
        return false;
    }
};


/*template <gpio_num_t ...Pins>
class Component : public ComponentBase {
protected:

public:
    Component() {}

    // All pin methods here could be static too, but I think that would defeat the entire purpose (from outside) since you'd always have to provide the entire type with all the pins (i.e. Joystick<14,16,17>)

    constexpr unsigned int getPinCount() const {
        return sizeof...(Pins);
    }

    template <unsigned int Index>
    constexpr gpio_num_t getPin() const {
        return std::get<Index>(std::make_tuple(Pins...));
    }

    template <gpio_num_t Pin>
    constexpr bool usesPin() const {
        return ((Pin == Pins) || ...);
    }

    constexpr bool usesPin(gpio_num_t pPin) const {
        return ((pPin == Pins) || ...);
    }
};*/
