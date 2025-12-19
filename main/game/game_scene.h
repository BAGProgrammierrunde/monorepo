#pragma once

#include <engine.h>
#include <cstdint>

class GameScene : public Scene {
private:
    uint64_t startTime = 0;
    int shift = 0;
    int nextStep = 0;
    float currentY = 0;
    float velocity = 0;

    bool showPlayTitle = false;

    bool isGrounded();
    float getSurvivalSecs();
    void handleStartingScreen();
    void updateJumpValues(float deltaTime, bool buttonPressed);
public:
    GameScene();
    void update(float deltaTime, bool buttonPressed) override;
};
