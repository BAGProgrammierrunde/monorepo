// TODO Umstellen auf Component
#pragma once

#include <Arduino.h>
#include "DallasTemperature.h"

class DS18B20 {
public:
    void init(int pin);
    float getTemperature();
    bool isConnected();
private:
    // TODO Maybe kann man den Code von DallasTemperature hier aufnehmen - viel Boilerplate Code?
    DallasTemperature sensors;
    // TODO Wirklich benötigt? Wirft Warnings - Updates?
    OneWire oneWire;
};
