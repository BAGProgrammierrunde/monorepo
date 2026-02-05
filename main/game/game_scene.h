#pragma once

#include <engine.h>
#include <cstdint>
#include <string_view>
#include "dino.h"

class GameScene : public Scene {
  private:
    uint64_t startTime = 0;
    int shift = 0;

    Dino dino;

    bool showPlayTitle = true;

    static constexpr unsigned int groundTextureCount = 5;
    const ground_t* groundTextures[groundTextureCount] = {};

    static const ground_t* randomCactus();
    static const ground_t* randomGround();
    static const ground_t* randomCactusOrGround(int cactusChance);

    void drawText(const std::string_view& text, int x, int y, int scale);
    void drawInt(int value, int x, int y, int scale);

    float getSurvivalSecs();
    void handleStartingScreen();
    void drawScore(float survivalSecs);
    void updateGround();
    void drawGround();

  public:
    void start() override;
    void update(float deltaTime, bool buttonPressed) override;
};
