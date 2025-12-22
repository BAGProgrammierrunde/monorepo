#pragma once

#include <engine.h>
#include <cstdint>

class GameScene : public Scene {
private:
    uint64_t startTime = 0;
    int shift = 0;
    
    // float currentY = 0;
    // float velocity = 0;

    bool showPlayTitle = false;

    #define BACKGROUND_COLOR WHITE
    #define FOREGROUND_COLOR SWAP16(0x7BEF)

    float getSurvivalSecs();
    void handleStartingScreen();
public:
    GameScene();
    void update(float deltaTime, bool buttonPressed) override;
};
