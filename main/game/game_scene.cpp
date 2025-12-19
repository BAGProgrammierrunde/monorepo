#include "game_scene.h"

#include "assets/dino.h"
#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gal/gal.h"

#include <cmath>
#include <string>

#define TAG "Game"

#define BACKGROUND_COLOR WHITE
#define FOREGROUND_COLOR SWAP16(0x7BEF)

constexpr float speedMultiplier = 2.f;

const int ground = 240 - 36 - 9;

const uint8_t* groundTextures[] = { cactus_2, cactus_2, cactus_2 };
const unsigned int groundTextureCount = sizeof(groundTextures) / sizeof(groundTextures[0]);

GameScene::GameScene() {
    startTime = esp_timer_get_time();
}

Scene* GameScene::update(float deltaTime, bool buttonPressed) {
    float survivalSecs = getSurvivalSecs();

    GAL::fill_background(BACKGROUND_COLOR);


    if (showPlayTitle) {
        handleStartingScreen();
    } else {
        updateJumpValues(deltaTime, buttonPressed);
    }

    // --- every 3 seconds 180 pixels
    float loopSpeed = 3.f;
    float texWidth = 180.f;
    for (int i = 0; i < groundTextureCount; i++)
        GAL::draw_bytes_at(LCD_HEIGHT - (std::fmodf(survivalSecs * loopSpeed + i, groundTextureCount) * texWidth), 240 - 38 * 2 - 9, 90, 38, 1, 1, groundTextures[i], FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    // ---


    if (++shift > 180) {
        shift = 0;

        // Update ground array here
    }

    const uint8_t* texture[] = { cactus_0, cactus_1, cactus_1 };

    // GAL::draw_at(texture[0], 0, 90, 38, -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    // GAL::draw_at(texture[1], 0, 90, 38, 180 + -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    // GAL::draw_at(texture[2], 0, 90, 38, 360 + -shift, 240 - 38 * 2 - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);

    // GAL::draw(cactus_1, 90, 38, -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    // GAL::draw(cactus_1, 90, 38, 180 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    // GAL::draw(cactus_1, 90, 38, 360 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);

    // GAL::draw_bytes_at(10, 80, 90, 38, 2, 2, cactus_2, FOREGROUND_COLOR, BACKGROUND_COLOR);
    // GAL::draw_bytes_at(190, 100, 90, 38, 2, 2, cactus_2, FOREGROUND_COLOR, BACKGROUND_COLOR);

    nextStep++;
    if (nextStep == 12) {
        nextStep = 0;
    }

    float y = ((240.f - 36.f * 2.f) - 9.f) - currentY;
    GAL::draw_at(nextStep < 6 ? dino_rightstep : dino_leftstep, 0, 34, 36, 10, y, FOREGROUND_COLOR, BACKGROUND_COLOR, 2, true);

    return nullptr;
}

bool GameScene::isGrounded() {
    return (currentY <= 0);
}

float GameScene::getSurvivalSecs() {
    return (esp_timer_get_time() - startTime) / 1000000.f;
}

void GameScene::handleStartingScreen() {
    const int scale = 4;
    const std::string_view title = "Start Dino Game";
    const std::string_view startText = "Press Button to Start";
    for (int x = 0; x < title.length(); x++) {
        GAL::draw_at(font, ((int) title[x] - 33) * 18, font_width, font_height, 20 + (x * 4 * scale
            ), 30, FOREGROUND_COLOR, BACKGROUND_COLOR, scale, true);
    }

    for (int x = 0; x < startText.length(); x++) {
        GAL::draw_at(font, ((int) title[x] - 33) * 18, font_width, font_height, 20 + (x * 4 * scale
            ), 130, FOREGROUND_COLOR, BACKGROUND_COLOR, scale, true);
    }
}

void GameScene::updateJumpValues(float deltaTime, bool buttonPressed) {
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
