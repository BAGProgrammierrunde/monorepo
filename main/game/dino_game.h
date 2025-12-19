#pragma once

#include "game/game.h"

class DinoGame : public Game {
public:
    void init() override;
    Scene* start() override;
};
