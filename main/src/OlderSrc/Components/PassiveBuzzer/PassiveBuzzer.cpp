#include "PassiveBuzzer.hpp"

PassiveBuzzer::PassiveBuzzer(unsigned int _pPin) : Component({_pPin}) {}

void PassiveBuzzer::playNote(Note pNote) const {
    tone(pins[0], pNote.tone, pNote.durationMillis);
}

void PassiveBuzzer::playMelody(const Melody& pMelody) const {
    std::pair<Note, unsigned int> curPlayNote;
    for (int n = 0; n < pMelody.notes.size(); n++)
    {
        curPlayNote = pMelody.notes.at(n);
        playNote(curPlayNote.first);
        delay(curPlayNote.first.durationMillis + curPlayNote.second); // Wait note duration + pause
    }
}

void PassiveBuzzer::stopPlaying() const {
    noTone(pins[0]);
}
