//
// Created by fabian on 08.10.24.
//

#ifndef SCHILD_TONE_H
#define SCHILD_TONE_H

// TODO Nutzen von pragma once
class Tone {
public:
    int note;
    int duration;
    Tone(int note, int duration = 0) {
        this -> note = note;
        this -> duration = duration;
    }
};


#endif //SCHILD_TONE_H
