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

    // Dino Jump und update
    dino.jump(deltaTime, buttonPressed);
    dino.nextStepUpdate();

    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.0f", survivalSecs * scoreMultiplier);
        const std::string_view text = buffer;
        const int scale = 4;
        const int textWidth = static_cast<int>(text.length()) * (static_cast<int>(font_width) + 1) * scale;
        const int x = 320 - textWidth - 6;
        drawText(text, x, 6, scale);
    }

    if (++shift > 180) {
        shift = 0;

        updateGround();
    }

    drawGround();
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

float GameScene::getSurvivalSecs() {
    return (esp_timer_get_time() - startTime) / 1000000.f;
}

void GameScene::handleStartingScreen() {
    drawText("Dino Game", 40, 30, 4);
    drawText("Press Button to Start", 20, 100, 3);
}
