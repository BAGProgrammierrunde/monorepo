#pragma once

#include <Arduino.h>
#include "Components/Structure/Component.hpp"

class Potentiometer : public Component<1> {
private:
public:
    explicit Potentiometer(unsigned int pin);

    int getValue();
};
