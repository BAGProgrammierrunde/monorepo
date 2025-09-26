#pragma once

struct Note {
public:
    int tone;
    int durationMillis;

    Note(int pTone = 0, int pDurationMillis = 150) {
        tone = pTone;
        durationMillis = pDurationMillis;
    }
};
