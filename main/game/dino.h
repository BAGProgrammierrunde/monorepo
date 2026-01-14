#pragma once
#include "assets/color.h"
#include "assets/dino.h"
#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_timer.h"
#include "gal/gal.h"

#include <cmath>
#include <string>

#include <cstdint>
#include <engine.h>

#define TAG "Game"

class Dino {
  private:
    float currentY = 0;
    float velocity = 0;

    int nextStep = 0;

  public:
    void jump(float deltaTime, bool buttonPressed);

    bool isGrounded();

    void nextStepUpdate();
};
