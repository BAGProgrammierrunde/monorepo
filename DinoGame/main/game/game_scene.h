#pragma once

#include "system/scene.h"

class game_scene : public Scene {
private:
    int score = 0;
    int shift = 0;
    int nextStep = 0;
    float currentY = 0;
    float velocity = 0;

    bool showPlayTitle = true;

    bool isGrounded();
public:
    Scene* update(float deltaTime, bool buttonPressed) override;
};
