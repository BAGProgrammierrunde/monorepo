#pragma once

class Game {
public:
    virtual ~Game() = default;
    virtual void init() = 0;
    virtual void loop() = 0;
};
