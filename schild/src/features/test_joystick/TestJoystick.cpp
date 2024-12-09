#include <Arduino.h>

#include "TestJoystick.h"
#include "features/common/Common.h"

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
    printComponentNotEnabledMessage("readJoystick", "ENABLE_JOYSTICK");
#endif
  }
}
