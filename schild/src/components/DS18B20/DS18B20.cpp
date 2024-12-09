#include "DS18B20.h"

DS18B20::DS18B20(unsigned int pin) : Component(pin) {
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
