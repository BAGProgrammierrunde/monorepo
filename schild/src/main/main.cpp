#include <Arduino.h>

#include "main/Configuration.hpp"
#include "Core/Structure/Device.hpp"

void setup() {
    Serial.begin(BAUD_RATE);
    Device::get().setScene<TestScene>();
}

void loop() {
    Device::get().update();
}
