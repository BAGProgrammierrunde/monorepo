#pragma once

#include <memory>
#include "Components/Button/Button.hpp"

namespace Features {
  void useButtonForDeepSleep(const std::shared_ptr<Button> &deepSleepButton, const std::shared_ptr<Button> &wakeupButton);
}
