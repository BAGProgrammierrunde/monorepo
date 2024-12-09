#pragma once

#include <memory>
#include "Components/joystick/Joystick.hpp"

namespace Features {
  void readJoystick(const std::shared_ptr<Joystick> &joystick);
}
