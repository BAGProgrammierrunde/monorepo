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

    bool showPlayTitle = false;


    float getSurvivalSecs();
    void handleStartingScreen();
public:
    GameScene();
    void update(float deltaTime, bool buttonPressed) override;
};
