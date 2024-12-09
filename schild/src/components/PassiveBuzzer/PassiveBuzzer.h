// TODO Umstellen auf Component
#pragma once

#include <Arduino.h>
#include "notes.h"
#include "Tone.h"

class PassiveBuzzer {
public:
    void init(int passiveBuzzerPin);
    void playNote(int note, int duration = 0);
    void playNote(Tone tone);
    void stopNote();
    void playNotes(int notes[], int durations[], int numberOfNotes, int pause);
    void playNotes(Tone tones[], int numberOfNotes, int pause);
private:
    int passiveBuzzerPin;
};
