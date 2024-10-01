//
// Created by fabian on 01.10.24.
//

#include "PassiveBuzzer.h"

void PassiveBuzzer::init(int passiveBuzzerPin) {
    this -> passiveBuzzerPin = passiveBuzzerPin;
}

void PassiveBuzzer::playNote(int note, int duration) {
    tone(passiveBuzzerPin, note);
}

void PassiveBuzzer::stopNote() {
    noTone(passiveBuzzerPin);
}

void PassiveBuzzer::playNotes(int *notes, int *durations) {
    // TODO: In Objekt auslagern? Bzw beides anbieten?
}