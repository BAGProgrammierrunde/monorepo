//
// Created by fabian on 13.09.24.
//

#include <Arduino.h>
#include "DallasTemperature.h"

class DS18B20 {
public:
    void init(int pin);
    float getTemperature();
    bool isConnected();
private:
    DallasTemperature sensors;
    OneWire oneWire;
};