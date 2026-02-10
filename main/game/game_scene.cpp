#include "game_scene.h"

#include "assets/color.h"
#include "assets/font.h"
#include "assets/ground.h"
#include "dino.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "gal/gal.h"

#include <string>

#define TAG "Game"

constexpr float speedMultiplier = 2.f;
constexpr float scoreMultiplier = 7.f;

void GameScene::start(Device& device) {
    m_Device = &device;
    startTime = esp_timer_get_time();
    for (unsigned int i = 0; i < groundTextureCount; ++i) {
        groundTextures[i] = &grounds[0];
    }
}

void GameScene::update(float deltaTime) {
    float survivalSecs = getSurvivalSecs();

    GAL::fill_background(BACKGROUND_COLOR);

    // Start Bildschrim
    // TODO Add boolean for start screen is visible
    if (!m_Device->isButtonAPressed() && showPlayTitle) {
        handleStartingScreen();
    } else if (m_Device->isButtonAPressed()) {
        showPlayTitle = false;
    }

    drawScore(survivalSecs);

    // TODO magic numbers
    shift += survivalSecs / 50.f + 3;

    // TODO magic number
    if (shift >= 90) {
        shift = shift % 90;

        updateGround();
    }

    drawGround();

    // Dino Jump und update
    dino.handleJump(deltaTime, m_Device->isButtonAPressed());
    dino.updateStep();
    if (dino.checkCollision(*m_Device)) {
        ESP_LOGI(TAG, "Game Over!");
        // TODO draw dead dino
        // TODO just for debugging
        // m_Device->getDisplay().waitASec = true;
    }
    dino.drawDino();
}

void GameScene::drawScore(float survivalSecs) {
    drawInt(static_cast<int>(survivalSecs * scoreMultiplier), 320 - 10, 10, 4);
}

const ground_t* GameScene::randomCactus() {
    return &grounds[esp_random() % 4 + 2];
}

const ground_t* GameScene::randomGround() {
    return &grounds[esp_random() & 1U];
}

const ground_t* GameScene::randomCactusOrGround(int cactusChance) {
    return esp_random() % 101 < cactusChance ? randomCactus() : randomGround();
}

void GameScene::updateGround() {
    int count = 0;
    for (unsigned int i = 0; i < groundTextureCount - 1; ++i) {
        groundTextures[i] = groundTextures[i + 1];
        if (groundTextures[i]->isCactus) {
            ++count;
        }
    }
    groundTextures[groundTextureCount - 1] = randomCactusOrGround(count == 0 ? 100 : count == 1 ? 75 : 0);
}

void GameScene::drawGround() {
    constexpr int scale = 1;
    for (int i = 0; i < groundTextureCount; ++i) {
        GAL::draw_at(groundTextures[i]->texture, 0, 90, 38, i * 90 - shift, 240 - 38 * scale - 9, FOREGROUND_COLOR, BACKGROUND_COLOR, scale, true);
    }
}

void GameScene::drawText(const std::string_view& text, int x, int y, int scale) {
    for (int i = 0; i < static_cast<int>(text.length()); ++i) {
        int characterBitIndex = static_cast<int>(text[i]) - 33;
        if (characterBitIndex < 0)
            continue;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, x + (i * (font_width + 1) * scale), y, FOREGROUND_COLOR,
                     BACKGROUND_COLOR, scale, true);
    }
}

void GameScene::drawInt(int value, int x, int y, int scale) {
    int count = 1;
    int temp  = value;
    while (temp >= 10) {
        temp /= 10;
        ++count;
    }

    const int step   = (static_cast<int>(font_width) + 1) * scale;
    const int width  = (count * step) - scale;
    const int startX = x - width + 1;

    int divisor = 1;
    for (int i = 1; i < count; ++i) {
        divisor *= 10;
    }

    const int digitOffset = 48 - 33; // '0' in font table
    for (int i = 0; i < count; ++i) {
        const int digit             = (value / divisor) % 10;
        const int characterBitIndex = digitOffset + digit;
        GAL::draw_at(font, characterBitIndex * 18, font_width, font_height, startX + (i * step), y, FOREGROUND_COLOR, BACKGROUND_COLOR,
                     scale, true);
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
