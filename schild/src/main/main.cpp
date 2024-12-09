#include <Arduino.h>
#include <driver/rtc_io.h>

#include "Configuration.hpp"
//#include "Utility.hpp"
#include "Components/Structure/Component.hpp"
#include "Components/Button/Button.hpp"
#include "Components/Joystick/Joystick.hpp"
#include "Components/Display/Display.hpp"
#include "Components/Structure/Device.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "Components/potentiometer/Potentiometer.hpp"
#include "Components/PassiveBuzzer/Tone.hpp"
#include "Components/PassiveBuzzer/notes.hpp"
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"

/*#include "Features/deep_sleep/DeepSleep.hpp"
#include "Features/lightup_leds/LightupLEDs.hpp"
#include "Features/melody/Melody.hpp"
#include "Features/print_button_pin/PrintButtonPin.hpp"
#include "Features/test_display/TestDisplay.hpp"*/

void setup() {
  Serial.begin(115200);
  delay(100);
  while (!Serial) {}

  //Utility::printWakeupReason();
  Serial.println("The project is built with C++ version: " + String(__cplusplus));

  const auto device = Device::getInstance();
  /*Features::useButtonForDeepSleep(device->button1, device->button4);
  Features::lightupLEDs();
  Features::printButtonPinWhenPressed(device->button2);
  Features::printButtonPinWhenPressed(device->button3);*/
}

void loop() {
  Serial.print(".");
  delay(500);
}
