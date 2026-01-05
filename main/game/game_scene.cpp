#include "game_scene.h"

#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gal/gal.h"
#include "dino.h"
#include "assets/color.h"

#include <cmath>
#include <string>

#define TAG "Game"

constexpr float speedMultiplier = 2.f;

const int ground = 240 - 36 - 9;

const uint8_t* groundTextures[]       = { cactus_2, cactus_2, cactus_2 };
const unsigned int groundTextureCount = sizeof(groundTextures) / sizeof(groundTextures[0]);

void GameScene::start() {
    startTime = esp_timer_get_time();
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

    // --- every 3 seconds 180 pixels
    float loopSpeed = 3.f;
    float texWidth  = 180.f;
    for (int i = 0; i < groundTextureCount; i++)
        GAL::draw_bytes_at(LCD_HEIGHT - (std::fmodf(survivalSecs * loopSpeed + i, groundTextureCount) * texWidth), 240 - 38 * 2 - 9, 90, 38,
                           2, 2, groundTextures[i], FOREGROUND_COLOR, BACKGROUND_COLOR);
    // ---

    if (++shift > 180) {
        shift = 0;

        // Update ground array here
    }
}

float GameScene::getSurvivalSecs() {
    return (esp_timer_get_time() - startTime) / 1000000.f;
}

void GameScene::handleStartingScreen() {
    const int scale                  = 4;
    const std::string_view title     = "Start Dino Game";
    const std::string_view startText = "Press Button to Start";
    for (int x = 0; x < title.length(); x++) {
        int characterBitIndex = static_cast<int>(title[x]) - 33;
        if (characterBitIndex < 0) continue;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, 20 + (x * (font_width  + 1) * scale), 30, FOREGROUND_COLOR, BACKGROUND_COLOR,
                     scale, true);
    }

    for (int x = 0; x < startText.length(); x++) {
        int characterBitIndex = static_cast<int>(title[x]) - 33;
        if (characterBitIndex < 0) continue;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, 20 + (x * (font_width  + 1) * scale), 130, FOREGROUND_COLOR,
                     BACKGROUND_COLOR, scale, true);
    }
}
