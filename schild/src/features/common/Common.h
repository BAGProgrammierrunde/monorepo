#pragma once

#include <Arduino.h>

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

namespace Features {
  void printStartMessage();
  void printComponentNotEnabledMessage(String functionName, String defineName);
}
