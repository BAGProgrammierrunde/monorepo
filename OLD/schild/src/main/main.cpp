#include <Arduino.h>

#include "main/Configuration.hpp"
#include "Core/Structure/Device.hpp"

void setup() {
    Device::get().setScene<TestScene>();
}

void loop() {
    Device::get().update();
}
