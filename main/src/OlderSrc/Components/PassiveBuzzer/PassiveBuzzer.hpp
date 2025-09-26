#pragma once

#include <Arduino.h>
#include "Note.hpp"
#include "Melody.hpp"
#include "Components/Structure/Component.hpp"

class PassiveBuzzer : public Component<1> {
private:

public:
    PassiveBuzzer(unsigned int _pPin);
    void playNote(Note pNote) const;
    void playMelody(const Melody& pMelody) const;
    void stopPlaying() const;
};
