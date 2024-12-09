#pragma once

#include <memory>

#include "components/PassiveBuzzer/PassiveBuzzer.h"

namespace features {
  void playMelody(const std::shared_ptr<PassiveBuzzer> &buzzer);
}
