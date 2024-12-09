#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <memory>
#include "Components/Button/Button.hpp"

namespace Features {
  void testDisplay(const std::shared_ptr<Button> &button);
}
