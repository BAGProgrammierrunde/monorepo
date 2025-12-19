#include "dino_game.h"

#include "game_scene.h"

void DinoGame::init() {
}

Scene* DinoGame::start() {
    GameScene* scene = new GameScene();
    return scene;
}
