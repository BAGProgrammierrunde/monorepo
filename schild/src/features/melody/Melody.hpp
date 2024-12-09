#pragma once

#include <memory>
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"

namespace Features {
  void playMelody(const std::shared_ptr<PassiveBuzzer> &buzzer);
  void playNote(const std::shared_ptr<PassiveBuzzer> &buzzer);
}
