#pragma once

#include <memory>
#include "Components/Button/Button.hpp"

namespace Features {
  void printButtonPinWhenPressed(const std::shared_ptr<Button> &button);
}
