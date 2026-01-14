#pragma once

#include <Arduino.h>
#include <DallasTemperature.h> // Includes OneWire.h too

#include "Components/Structure/Component.hpp"

class TemperatureSensor : public Component<1> {
private:
    // TODO Maybe kann man den Code von den Libraries hier selbst implementieren und die dann rausschmeißen..
    // Muss man mal reinschauen wie die Funktionieren und ob das in machbarem Rahmen geht.
    OneWire oneWire;
    DallasTemperature sensor;
    unsigned int mySensorIndex;

    static inline unsigned int internalTempSensorCount = 0;

public:
    TemperatureSensor(unsigned int _pPin);
    float getTemperature(bool pCelsiusElseFahrenheit = true);
    //bool isConnected();
};
