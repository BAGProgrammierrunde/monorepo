#pragma once

#include "assets/dino.h"

#include <engine.h>
#include <esp_log.h>

#define TAG "Game"

class Dino {
  private:
    static inline float s_Force = 6.f;
    static inline float s_Gravity = 20.f;

    bool m_Alive = true;

    int m_CurrentY = 0;

    float m_JumpHeight = 0;
    float m_Velocity = 0;

    int m_CurrentStep = 0;

    bool isGrounded();

  public:
    void handleJump(float deltaTime, bool buttonPressed);
    void updateStep();
    void drawDino();
    bool checkCollision(Device& device);
};
