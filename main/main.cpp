#include <engine.h>

#include "game/game_scene.h"


#define TAG "Main"

extern "C" void app_main() {
    System::init();
    System::start<GameScene>();
}
