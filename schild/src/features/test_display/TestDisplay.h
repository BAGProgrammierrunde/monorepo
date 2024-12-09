#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <memory>

#include "components/Button/Button.h"
#include "features/common/Common.h"

namespace features {
  void testDisplay(const std::shared_ptr<Button> &button);
}
