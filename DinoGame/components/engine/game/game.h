#pragma once

#include "system/Scene.h"

class Game {
public:
    virtual ~Game() = default;
    virtual void init() = 0;
    virtual Scene* start() = 0;
};
