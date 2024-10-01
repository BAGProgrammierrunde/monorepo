//
// Created by fabian on 01.10.24.
//

#ifndef SCHILD_PASSIVEBUZZER_H
#define SCHILD_PASSIVEBUZZER_H

#include <Arduino.h>
#include "notes.h"

class PassiveBuzzer {
public:
    void init(int passiveBuzzerPin);
    void playNote(int note, int duration = 0);
    void stopNote();
    void playNotes(int notes[], int durations[]);
private:
    int passiveBuzzerPin;
};


#endif //SCHILD_PASSIVEBUZZER_H
