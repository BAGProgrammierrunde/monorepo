#pragma once

#include "assets/ground.h"
#include "dino.h"

#include <engine.h>
#include <string_view>

class GameScene final : public Scene {
  private:
    // TODO maybe hold Device in static global space?
    // TODO should it be accessible from everywhere
    Device* m_Device = nullptr;
    uint64_t startTime = 0;
    int m_Shift = 0;

    Dino dino;

    bool showPlayTitle = true;

    static constexpr unsigned int groundTextureCount = 5;
    const ground_t* groundTextures[groundTextureCount] = {};

    static const ground_t* randomCactus();
    static const ground_t* randomGround();
    static const ground_t* randomCactusOrGround(int cactusChance);

    void drawText(const std::string_view& text, int x, int y, int scale);
    void drawInt(int value, int x, int y, int scale);

    inline void updateShift(float survivalSeconds);
    float getSurvivalSeconds();
    inline void handleStartingScreen();
    inline void drawScore(float survivalSecs);
    inline void updateGround();
    inline void drawGround();

  public:
    void start(Device& device) override;
    void update(float deltaTime) override;
};
