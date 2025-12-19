#pragma once

#include <Arduino.h>

#include "Components/Structure/Component.hpp"

class LED : public Component<1> {
private:

public:
    LED(unsigned int _pPin);
    void setState(bool pOn);
};