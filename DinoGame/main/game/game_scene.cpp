#include "game_scene.h"

#include "assets/dino.h"
#include "assets/font.h"
#include "assets/ground.h"
#include "esp_log.h"
#include "gal/gal.h"

#include <string>

#define TAG "Game"

#define BACKGROUND_COLOR WHITE
#define FOREGROUND_COLOR SWAP16(0x7BEF)

const int ground = 240 - 36 - 9;

Scene* game_scene::update(float deltaTime, bool buttonPressed) {
    GAL::fill_background(BACKGROUND_COLOR);

    if (showPlayTitle) {
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

    if (++shift > 180) {
        shift = 0;

        // Update ground array here
    }

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

    GAL::draw(cactus_1, 90, 38, -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw(cactus_1, 90, 38, 180 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw(cactus_1, 90, 38, 360 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);

    nextStep++;
    if (nextStep == 12) {
        nextStep = 0;
    }

    float y = ((240.f - 36.f * 2.f) - 9.f) - currentY;
    GAL::draw_at(nextStep < 6 ? dino_rightstep : dino_leftstep, 0, 34, 36, 10, y, FOREGROUND_COLOR, BACKGROUND_COLOR, 2, true);

    return nullptr;
}

bool game_scene::isGrounded() {
    return (currentY <= 0);
}
