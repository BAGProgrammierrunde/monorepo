#include <engine.h>

#include "game/dino_game.h"

#define TAG "Main"

extern "C" void app_main() {
    System system;
    DinoGame dinoGame;
    system.init(dinoGame);
    system.run();
}
