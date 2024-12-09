#pragma once

#include <Arduino.h>
#include <components/Structure/Component.h>

class Potentiometer : public Component<1> {
private:
public:
    explicit Potentiometer(unsigned int pin);

    int getValue();
};
