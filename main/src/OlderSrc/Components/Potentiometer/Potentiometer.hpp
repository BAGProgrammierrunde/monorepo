#pragma once

#include <Arduino.h>
#include "Components/Structure/Component.hpp"

class Potentiometer : public Component<1> {
private:

public:
    Potentiometer(unsigned int _pPin);
    float getValue() const;
};
