#include "system.h"

#include "gal/gal.h"
#include "input/button.h"

#include <esp_log.h>
#include <esp_timer.h>

#define TAG "System"

// DISCUSS correct here?
TaskHandle_t System::mainTaskHandle = nullptr;
TaskHandle_t System::gameTaskHandle = nullptr;

void System::init(Game &pGame) {
    ESP_LOGI(TAG, "Initializing..");
    game = &pGame;
    mainTaskHandle = xTaskGetCurrentTaskHandle();

    // DISCUSS where should we initialize GAL?
    GAL::init();

    // DISCUSS Here or in game or in device?
    GAL::rotate(ROT_90_CW);

    // DISCUSS Clears the buffers - here or in GAL or in display?
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
}

void System::run() {
    ESP_LOGI(TAG, "Run..");
    createGameTask();

    int frames         = 0;
    int64_t start_time = esp_timer_get_time();

    while (true) {
        xTaskNotifyGive(System::gameTaskHandle);

        GAL::send_active_buffer();

        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

        GAL::switch_frame_buffers();

        Button btn1 = Button(GPIO_NUM_13);
        Button btn2 = Button(GPIO_NUM_14);

        frames++;
        int64_t now = esp_timer_get_time();
        if (now - start_time >= 1000000) {
            ESP_LOGI(TAG, "FPS: %d ", frames);
            frames     = 0;
            start_time = now;

            if (btn1.isPressed()) {
                ESP_LOGI(TAG, "Button 1 pressed");
            }
            if (btn2.isPressed()) {
                ESP_LOGI(TAG, "Button 2 pressed");
            }
        }
    }
}

void System::createGameTask() {
    xTaskCreatePinnedToCore(gameTask, "Game", 10000, game, 10, &System::gameTaskHandle, 1);
}

void System::gameTask(void* pvParameters) {
    auto *game = static_cast<Game*>(pvParameters);
    game->init();
    // TODO Remove game.loop and replace by game.init for a starting screen
    game->loop();
    GAL::switch_frame_buffers();
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        game->loop();
        xTaskNotifyGive(System::mainTaskHandle);
    }
}
