#pragma once

#include <vector>
#include "Note.hpp"

struct Melody {
public:
    std::vector<std::pair<Note, unsigned int>> notes; // <Note, PauseMillisToNextNote>

    Melody(const std::vector<std::pair<Note, unsigned int>>& pNotes = {}) : notes(pNotes) {}
};
