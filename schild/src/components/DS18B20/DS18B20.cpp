//
// Created by fabian on 13.09.24.
//

#include "DS18B20.h"

#ifndef SCHILD_DS28B20_H
#define SCHILD_DS28B20_H

/// <summary>Use temperature sensor on specific pin.<br>
/// </summary>
void DS18B20::init(int pin) {
    oneWire = OneWire(pin);

    sensors.setOneWire(&oneWire);
    sensors.begin();
}

/// <summary>Gets the temperature from the sensor.<br>
/// <para>Returns the temperature as a float. If the device is not connected the temperature reads 0.0°C</para>
/// </summary>
float DS18B20::getTemperature() {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    return tempC != DEVICE_DISCONNECTED_C ? tempC : (float) 0.0;
}

/// <summary>Use temperature sensor on default pin.<br>
/// <para>The default pin is: 17</para>
/// </summary>
bool DS18B20::isConnected() {
    sensors.requestTemperatures();
    float connection = sensors.getTempCByIndex(0);

    return connection != DEVICE_DISCONNECTED_C;
}

#endif //SCHILD_DS28B20_H
