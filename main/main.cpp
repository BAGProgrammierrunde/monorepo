#include <engine.h>

#include "game/game_scene.h"

#define TAG "Main"

extern "C" void app_main() {
    GameScene gameScene;
    System::init();
    System::run(&gameScene);
}
