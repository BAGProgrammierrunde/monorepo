#pragma once

#include <memory>

#include "components/Button/Button.h"

namespace features {
  void useButtonForDeepSleep(const std::shared_ptr<Button> &deepSleepButton, const std::shared_ptr<Button> &wakeupButton);
}
