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

    static constexpr unsigned int groundTextureCount = 3;
    const uint8_t* groundTextures[groundTextureCount] = {};

    static const uint8_t* randomGroundTexture();

    float getSurvivalSecs();
    void handleStartingScreen();
    void updateGround();
    void drawGround();

  public:
    void start() override;
    void update(float deltaTime, bool buttonPressed) override;
};
