#include <Arduino.h>

#include "TestJoystick.hpp"
#include "main/Utility.hpp"

namespace Features {
  void readJoystick(const std::shared_ptr<Joystick> &joystick) {
#if ENABLE_JOYSTICK
    Serial.print("Y is: ");
    Serial.print(joystick->getY());

    Serial.print(" and X is: ");
    Serial.print(joystick->getX());

    Serial.print(" and Button is: ");
    Serial.println(joystick->button.isPressed() ? "ON" : "OFF");
#else
    Utility::printComponentNotEnabledMessage("readJoystick", "ENABLE_JOYSTICK");
#endif
  }
}
