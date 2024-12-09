#pragma once

#include <memory>

#include "components/PassiveBuzzer/PassiveBuzzer.h"

namespace Features {
  void playMelody(const std::shared_ptr<PassiveBuzzer> &buzzer);
  void playNote(const std::shared_ptr<PassiveBuzzer> &buzzer);
}
