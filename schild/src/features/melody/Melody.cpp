#include <Arduino.h>

#include "Melody.h"
#include "features/common/Common.h"

namespace Features {
  void playMelody(const std::shared_ptr<PassiveBuzzer> &buzzer) {
#if ENABLE_BUZZER
    Tone tones[] = {
      {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 360}, {NOTE_E5, 0},
      {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 360}, {NOTE_E5, 0},
      {NOTE_E5, 80}, {NOTE_G5, 80}, {NOTE_C5, 80}, {NOTE_D5, 80},
      {NOTE_E5, 760}, {NOTE_E5, 0}, {NOTE_E5, 0}, {NOTE_E5, 0},
      {NOTE_F5, 80}, {NOTE_F5, 80}, {NOTE_F5, 80}, {NOTE_F5, 80},
      {NOTE_F5, 80}, {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 80},
      {NOTE_E5, 80}, {NOTE_D5, 80}, {NOTE_D5, 80}, {NOTE_E5, 80},
      {NOTE_D5, 360}, {NOTE_G5, 0}, {NOTE_G5, 0}, {NOTE_G5, 0}
    };

    buzzer->playNotes(tones, 31, 200);

    delay(3000);
#else
    printComponentNotEnabledMessage("playMelody", "ENABLE_BUZZER");
#endif
  }

  void playNote(const std::shared_ptr<PassiveBuzzer> &buzzer) {
#if ENABLE_BUZZER
    buzzer->playNote(NOTE_F3, 160);
    delay(1000);
    buzzer->playNote(NOTE_C5);
    delay(500);
    buzzer->stopNote();
#else
    printComponentNotEnabledMessage("playNote", "ENABLE_BUZZER");
#endif
  }
}
