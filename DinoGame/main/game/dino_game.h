#pragma once

#include "game/game.h"

class DinoGame : public Game {
  private:
    int shift = 0;
    int nextStep = 0;

public:
    void init() override;
    void loop() override;
};
