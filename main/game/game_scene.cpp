#include "game_scene.h"

#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "gal/gal.h"
#include "dino.h"
#include "assets/color.h"

#include <cstdio>
#include <string>

#define TAG "Game"

constexpr float speedMultiplier = 2.f;
constexpr float scoreMultiplier = 7.f;

void GameScene::start() {
    startTime = esp_timer_get_time();
    for (unsigned int i = 0; i < groundTextureCount; ++i) {
        groundTextures[i] = cactus_0;
    }
}

void GameScene::update(float deltaTime, bool buttonPressed) {
    float survivalSecs = getSurvivalSecs();

    GAL::fill_background(BACKGROUND_COLOR);

    // Start Bildschrim
    if (!buttonPressed && showPlayTitle) {
        handleStartingScreen();
    } else if (buttonPressed) {
        showPlayTitle = false;
    }

    drawScore(survivalSecs);

    if (++shift > 180) {
        shift = 0;

        updateGround();
    }

    drawGround();

    // Dino Jump und update
    dino.jump(deltaTime, buttonPressed);
    dino.nextStepUpdate();
}

void GameScene::drawScore(float survivalSecs) {
    drawInt(static_cast<int>(survivalSecs * scoreMultiplier), 320 - 10, 10, 4);
}

const uint8_t* GameScene::randomGroundTexture() {
    return (esp_random() & 1U) == 0 ? cactus_0 : cactus_1;
}

void GameScene::updateGround() {
    groundTextures[0] = groundTextures[1];
    groundTextures[1] = groundTextures[2];
    groundTextures[2] = randomGroundTexture();
}

void GameScene::drawGround() {
    GAL::draw_at(groundTextures[0], 0, 90, 38, -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw_at(groundTextures[1], 0, 90, 38, 180 + -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw_at(groundTextures[2], 0, 90, 38, 360 + -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
}

void GameScene::drawText(const std::string_view& text, int x, int y, int scale) {
    for (int i = 0; i < static_cast<int>(text.length()); ++i) {
        int characterBitIndex = static_cast<int>(text[i]) - 33;
        if (characterBitIndex < 0) continue;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, x + (i * (font_width + 1) * scale), y, FOREGROUND_COLOR, BACKGROUND_COLOR,
                     scale, true);
    }
}

void GameScene::drawInt(int value, int x, int y, int scale) {
    int count = 1;
    int temp = value;
    while (temp >= 10) {
        temp /= 10;
        ++count;
    }

    const int step = (static_cast<int>(font_width) + 1) * scale;
    const int width = (count * step) - scale;
    const int startX = x - width + 1;

    int divisor = 1;
    for (int i = 1; i < count; ++i) {
        divisor *= 10;
    }

    const int digitOffset = 48 - 33; // '0' in font table
    for (int i = 0; i < count; ++i) {
        const int digit = (value / divisor) % 10;
        const int characterBitIndex = digitOffset + digit;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, startX + (i * step), y, FOREGROUND_COLOR, BACKGROUND_COLOR, scale, true);
        divisor /= 10;
    }
}

float GameScene::getSurvivalSecs() {
    return (esp_timer_get_time() - startTime) / 1000000.f;
}

void GameScene::handleStartingScreen() {
    drawText("Dino Game", 40, 30, 4);
    drawText("Press Button to Start", 20, 100, 3);
}
