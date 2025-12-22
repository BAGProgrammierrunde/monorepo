#pragma once
#include "dino.h"
#include "assets/color.h"
#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gal/gal.h"

#include <cmath>
#include <string>

#include <cstdint>
#include <engine.h>

#define TAG "Game"

float currentY = 0;
float velocity = 0;

int nextStep = 0;

void Dino::jump(float deltaTime, bool buttonPressed) {
    if (isGrounded()) {
        if (buttonPressed) {
            velocity = 9;
            currentY += velocity * deltaTime;
            ESP_LOGI(TAG, "Jump!");
        }
    } else {
        velocity -= 27.f * deltaTime;
        currentY += velocity;

        if (isGrounded()) {
            currentY = 0;
            velocity = 0;
        }
    }
}

bool Dino::isGrounded() {
    return (currentY <= 0);
}

void Dino::nextStepUpdate() {
    nextStep++;
    if (nextStep == 12) {
        nextStep = 0;
    }

    float y = ((240.f - 36.f * 2.f) - 9.f) - currentY;
    GAL::draw_at(nextStep < 6 ? dino_rightstep : dino_leftstep, 0, 34, 36, 10, y, FOREGROUND_COLOR, BACKGROUND_COLOR, 2, true);
}
