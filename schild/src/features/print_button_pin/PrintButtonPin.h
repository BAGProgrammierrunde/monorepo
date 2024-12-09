#pragma once

#include <memory>

#include "components/Button/Button.h"

namespace Features {
  void printButtonPinWhenPressed(const std::shared_ptr<Button> &button);
}
