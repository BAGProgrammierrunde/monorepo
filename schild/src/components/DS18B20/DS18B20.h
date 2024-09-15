//
// Created by fabian on 13.09.24.
//

#include <Arduino.h>

class DS18B20 {
public:
    float getTemperature();
    void init();
    void init(int pin);
    bool isConnected();
};