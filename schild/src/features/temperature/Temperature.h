#pragma once

#include <memory>

#include "components/Display/Display.h"

namespace Features {
  void printButtonPinWhenPressed(const std::shared_ptr<Display> &display);
}
