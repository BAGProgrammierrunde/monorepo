#include <Arduino.h>
#include <driver/rtc_io.h>

#include "configuration.h"
#include "components/Structure/Component.h"
#include "components/Button/Button.h"
#include "components/Joystick/Joystick.h"
#include <components/Structure/Device.h>

#include "components/DS18B20/DS18B20.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/PassiveBuzzer/Tone.h"
#include "components/PassiveBuzzer/notes.h"
#include "components/PassiveBuzzer/PassiveBuzzer.h"
#include "features/common/Common.h"
#include "features/deep_sleep/DeepSleep.h"
#include "features/lightup_leds/LightupLEDs.h"
#include "features/melody/Melody.h"
#include "features/print_button_pin/PrintButtonPin.h"
#include "features/test_display/TestDisplay.h"

// Lange nicht an ihrer finalen Position Komponenten funktionstest-Funktionen
void setupDisplay();

void showTemperature();

void readJoystick();

void playMelody();

void playNote();

void setup() {
  Serial.begin(115000);
  delay(100);
  while (!Serial) { }

  Features::printStartMessage();

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
