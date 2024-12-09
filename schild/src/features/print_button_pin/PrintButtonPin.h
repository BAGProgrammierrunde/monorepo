#pragma once

#include <memory>

#include "components/Button/Button.h"

namespace features {
  void printButtonPinWhenPressed(const std::shared_ptr<Button> &button);
}
