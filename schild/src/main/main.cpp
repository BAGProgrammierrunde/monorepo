#include <Arduino.h>

#include "Configuration.hpp"
#include "main/Utility.hpp"
#include "Components/Structure/Device.hpp"

#include "Features/deep_sleep/DeepSleep.hpp"
#include "Features/lightup_leds/LightupLEDs.hpp"
#include "Features/print_button_pin/PrintButtonPin.hpp"

void setup() {
    Serial.begin(BAUD_RATE);
    delay(100);
    while (!Serial) {}

    Utility::printWakeupReason();
    Serial.println("The project is built with C++ version: " + String(__cplusplus));

    const std::shared_ptr<Device> device = Device::getInstance();
    Features::useButtonForDeepSleep(device->button1, device->button4);
    Features::lightupLEDs();
    Features::printButtonPinWhenPressed(device->button2);
    Features::printButtonPinWhenPressed(device->button3);
}

void loop() {
    Serial.print(".");
    delay(500);
}