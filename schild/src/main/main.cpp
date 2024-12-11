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

  const auto device = Device::getInstance();
  Features::useButtonForDeepSleep(device->button1, device->button4);
  Features::lightupLEDs();
  Features::printButtonPinWhenPressed(device->button2);
  Features::printButtonPinWhenPressed(device->button3);
}

void loop() {
  Serial.print(".");
  delay(500);
}

/*
const float MAX_READOUT = 4095;
const float READOUT_CORRECTION = 50;
const float MAX_VOLTAGE = 4.2;
String batteryVoltage = "0V";
void checkBattery() {
  Serial.println("Battery Analog Read: " + String(analogRead(A0)-READOUT_CORRECTION));
  batteryVoltage = String((float((analogRead(A0)-READOUT_CORRECTION))/MAX_READOUT) * MAX_VOLTAGE) + "V";
  Serial.println("Battery Check: " + batteryVoltage);
}
*/
