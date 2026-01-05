#include "game/game_scene.h"

#include <engine.h>

#define TAG "Main"

extern "C" void app_main() {
    System::init();
    System::start<GameScene>();
}
