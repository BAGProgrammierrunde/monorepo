#pragma once

#include <Arduino.h>

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

namespace features {
  void printStartMessage();
  void printComponentNotEnabledMessage(String functionName, String defineName);
}
