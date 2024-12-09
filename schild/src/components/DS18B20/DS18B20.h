#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>

#include "components/Structure/Component.h"

class DS18B20 : public Component<1> {
private:
    // TODO Maybe kann man den Code von DallasTemperature hier aufnehmen - viel Boilerplate Code?
    DallasTemperature sensors;
    // TODO Wirklich benötigt? Wirft Warnings - Updates?
    OneWire oneWire;
public:
    explicit DS18B20(unsigned int pin);

    float getTemperature();
    bool isConnected();
};
