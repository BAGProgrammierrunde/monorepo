#include "TemperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(unsigned int _pPin) : Component({_pPin}) {
    oneWire = OneWire(pins[0]);
    sensor.setOneWire(&oneWire);
    sensor.begin();

    mySensorIndex = internalTempSensorCount;
    internalTempSensorCount++;
}

float TemperatureSensor::getTemperature(bool pCelsiusElseFahrenheit) {
    sensor.requestTemperatures();
    return (pCelsiusElseFahrenheit ? sensor.getTempCByIndex(mySensorIndex) : sensor.getTempFByIndex(mySensorIndex));
}

/*bool TemperatureSensor::isConnected() {
    sensor.requestTemperatures();
    return (sensor.getTempCByIndex(mySensorIndex) != DEVICE_DISCONNECTED_C);
}*/
