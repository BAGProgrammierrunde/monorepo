#pragma once

#include <memory>

#include "components/joystick/Joystick.h"

namespace Features {
  void readJoystick(const std::shared_ptr<Joystick> &joystick);
}
