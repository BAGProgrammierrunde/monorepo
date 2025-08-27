#include "game.h"

#include "esp_log.h"

#define TAG "Game"

Game::Game(ST7789 &display) : display(display) {

}

void Game::loop() {
  ESP_LOGI(TAG, "loop");
}
