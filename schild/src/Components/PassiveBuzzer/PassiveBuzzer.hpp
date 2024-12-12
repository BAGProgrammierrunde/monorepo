#pragma once

#include <Arduino.h>
#include "Tone.hpp"
#include "Components/Structure/Component.hpp"

class PassiveBuzzer : public Component<1> {
private:
public:
    explicit PassiveBuzzer(unsigned int pin);

    void playNote(int note, int duration = 0) const;
    void playNote(Tone tone) const;
    void stopNote() const;
    void playNotes(const int notes[], const int durations[], int numberOfNotes, int pause) const;
    void playNotes(const Tone tones[], int numberOfNotes, int pause) const;
};
