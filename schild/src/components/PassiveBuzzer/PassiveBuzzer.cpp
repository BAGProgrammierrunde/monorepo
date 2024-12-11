#include "PassiveBuzzer.hpp"

/// <summary>Constructs the buzzer with specified pin.<br>
/// <para><br>
/// pin: The pin for the buzzer.<br>
/// </para>
/// </summary>
PassiveBuzzer::PassiveBuzzer(unsigned int pin) : Component({pin}) {
}

/// <summary>Plays a note. If no duration is given the note will play until "stopNote()" is called!<br>
/// <para><br>
/// note: The note. !notes.h can be used!<br>
/// duration: (Optional) The duration the note should be played in milliseconds.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNote(int note, int duration) const {
    tone(pins[0], note, duration);
}

/// <summary>Plays a note.<br>
/// <para><br>
/// playTone: The tone with optional duration.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNote(Tone playTone) const {
    this->playNote(playTone.note, playTone.duration);
}

/// <summary>Stops playing the currently playing note.<br>
/// </summary>
void PassiveBuzzer::stopNote() const {
    noTone(pins[0]);
}

/// <summary>Plays notes with durations and a pause in between.<br>
/// <para><br>
/// notes: The notes to be played.<br>
/// durations: The durations the notes should be played for.<br>
/// numberOfNotes: The number of notes to be played.<br>
/// pause: The pause length between notes.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNotes(const int *notes, const int *durations, const int numberOfNotes, const int pause) const {
    for (int currentNote = 0; currentNote < numberOfNotes; currentNote++) {
        this->playNote(notes[currentNote], durations[currentNote]);

        delay(pause);
    }
}

/// <summary>Plays notes with durations and a pause in between.<br>
/// <para><br>
/// tones: The tones to be played with optional duration.<br>
/// numberOfNotes: The number of notes to be played.<br>
/// pause: The pause length between notes.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNotes(const Tone *tones, const int numberOfNotes, const int pause) const {
    for (int currentNote = 0; currentNote < numberOfNotes; currentNote++) {
        if (tones[currentNote].duration > 0) {
            this->playNote(tones[currentNote].note, tones[currentNote].duration);
        }

        delay(pause);
    }
}
