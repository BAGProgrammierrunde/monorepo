#include "dino_game.h"

#include "game_scene.h"

void DinoGame::init() {
}

Scene* DinoGame::start() {
    game_scene* scene = new game_scene();
    return scene;
}
