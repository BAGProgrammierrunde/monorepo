//
// Created by fabian on 01.10.24.
//

#include "PassiveBuzzer.h"

/// <summary>Initializes the buzzer with specified pins.<br>
/// <para><br>
/// passiveBuzzerPin: The pin for the buzzer.<br>
/// </para>
/// </summary>
void PassiveBuzzer::init(int passiveBuzzerPin) {
    this->passiveBuzzerPin = passiveBuzzerPin;
}

/// <summary>Plays a note. If no duration is given the note will play until "stopNote()" is called!<br>
/// <para><br>
/// note: The note. !notes.h can be used!<br>
/// duration: (Optional) The duration the note should be played in milliseconds.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNote(int note, int duration) {
    tone(passiveBuzzerPin, note, duration);
}

/// <summary>Plays a note.<br>
/// <para><br>
/// playTone: The tone with optional duration.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNote(Tone playTone) {
    this->playNote(playTone.note, playTone.duration);
}

/// <summary>Stops playing the currently playing note.<br>
/// </summary>
void PassiveBuzzer::stopNote() {
    noTone(passiveBuzzerPin);
}

/// <summary>Plays notes with durations and a pause in between.<br>
/// <para><br>
/// notes: The notes to be played.<br>
/// durations: The durations the notes should be played for.<br>
/// numberOfNotes: The number of notes to be played.<br>
/// pause: The pause length between notes.<br>
/// </para>
/// </summary>
void PassiveBuzzer::playNotes(int *notes, int *durations, int numberOfNotes, int pause) {
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
void PassiveBuzzer::playNotes(Tone *tones, int numberOfNotes, int pause) {
    for (int currentNote = 0; currentNote < numberOfNotes; currentNote++) {
        if (tones[currentNote].duration > 0) {
            this->playNote(tones[currentNote].note, tones[currentNote].duration);
        }

        delay(pause);
    }
}
