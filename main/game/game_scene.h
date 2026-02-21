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

    bool m_IsInStartScreen = true;
    bool m_IsGameOver = false;

    static constexpr unsigned int groundTextureCount = 5;
    const ground_t* groundTextures[groundTextureCount] = {};

    static const ground_t* randomCactus();
    static const ground_t* randomGround();
    static const ground_t* randomCactusOrGround(int cactusChance);

    void drawText(const std::string_view& text, int x, int y, int scale);
    void drawInt(int value, int x, int y, int scale);

    inline bool handleGameOverScreen();
    inline void updateShift(float survivalSeconds);
    float getSurvivalSeconds();
    static inline void resetScreen();
    inline void handleStartScreen();
    inline void drawStartScreen();
    inline void drawScore(float survivalSecs);
    inline void handleGround();
    inline void updateGround();
    inline void drawGround();
    inline bool handleDino(float deltaTime);
    inline void handleGameOver(bool collided);

  public:
    void start(Device& device) override;
    void update(float deltaTime) override;
};
