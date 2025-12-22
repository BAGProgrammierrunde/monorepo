#pragma once

#include <engine.h>
#include <cstdint>
#include "dino.h"

class GameScene : public Scene {
  private:
    uint64_t startTime = 0;
    int shift = 0;

    // float currentY = 0;
    // float velocity = 0;

    Dino dino;

    bool showPlayTitle = true;


    float getSurvivalSecs();
    void handleStartingScreen();

  public:
    void start() override;
    void update(float deltaTime, bool buttonPressed) override;
};
