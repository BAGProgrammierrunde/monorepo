#pragma once

#include <memory>
#include "Components/Display/Display.hpp"

namespace Features {
  void printButtonPinWhenPressed(const std::shared_ptr<Display> &display);
}
