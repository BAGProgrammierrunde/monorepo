#include "dino_game.h"

#include <engine.h>

#include "assets/dino.h"
#include "assets/ground.h"
#include "esp_log.h"

#define TAG "Game"

#define BACKGROUND_COLOR WHITE
#define FOREGROUND_COLOR SWAP16(0x7BEF)

void DinoGame::init() {
}

void DinoGame::loop() {
    GAL::fill_background(BACKGROUND_COLOR);

    if (++shift > 180) {
        shift = 0;

        // Update ground array here
    }

    GAL::draw(cactus_1, 90, 38, -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw(cactus_1, 90, 38, 180 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);
    GAL::draw(cactus_1, 90, 38, 360 + -shift, FOREGROUND_COLOR, BACKGROUND_COLOR, 2);

    nextStep++;
    if (nextStep == 12) {
        nextStep = 0;
    }

    GAL::draw(nextStep < 6 ? dino_rightstep : dino_leftstep, 34, 36, 10, FOREGROUND_COLOR, BACKGROUND_COLOR, 2, true);
    
    // GAL::draw(cactus_1, 90, 38, 270 + -shift, BACKGROUND, FOREGROUND, 1);
    // GAL::draw(cactus_1, 90, 38, 360 + -shift, BACKGROUND, FOREGROUND, 1);
}
