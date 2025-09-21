#include "game.h"

#include "gal.h"

#include "esp_log.h"
#include "ground.h"

#define TAG "Game"

#define BACKGROUND WHITE
#define FOREGROUND SWAP16(0x7BEF)

void Game::init() {
}

void Game::loop() {
    GAL::fill_background(BACKGROUND);

    if (++shift > 270) {
        shift = 0;

        // Update ground array here
    }

    GAL::draw(cactus_1, 90, 38, -shift, BACKGROUND, FOREGROUND);
    GAL::draw(cactus_1, 90, 38, 270 + -shift, BACKGROUND, FOREGROUND);
    GAL::draw(cactus_1, 90, 38, 540 + -shift, BACKGROUND, FOREGROUND);
}
