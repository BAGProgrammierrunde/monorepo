#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>

#include "Components/Structure/Component.hpp"

class TemperatureSensor : public Component<1> {
private:
    // TODO Maybe kann man den Code von DallasTemperature hier aufnehmen - viel Boilerplate Code?
    DallasTemperature sensors;
    // TODO Wirklich benötigt? Wirft Warnings - Updates?
    OneWire oneWire;
public:
    explicit TemperatureSensor(unsigned int pin);

    float getTemperature();
    bool isConnected();
};
