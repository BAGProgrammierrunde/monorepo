#include "system.h"

#include "gal/gal.h"
#include "input/button.h"

#include <esp_log.h>
#include <esp_timer.h>

#define TAG "System"

// DISCUSS correct here?
TaskHandle_t System::mainTaskHandle = nullptr;
TaskHandle_t System::gameTaskHandle = nullptr;

void System::init(Game& pGame) {
    ESP_LOGI(TAG, "Initializing..");
    game           = &pGame;
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

    int64_t frameTime = esp_timer_get_time();
    int64_t deltaMicros = 0;
    int64_t deltaLow = -1;
    int64_t deltaHigh = -1;

    while (true) {
        xTaskNotifyGive(System::gameTaskHandle);
        GAL::send_active_buffer();
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        GAL::switch_frame_buffers();

        int64_t prevFrameTime = frameTime;
        frameTime = esp_timer_get_time();
        deltaMicros = frameTime - prevFrameTime;

        bool print = (prevFrameTime % 1000000 > frameTime % 1000000);
        if (deltaLow == -1 || deltaMicros < deltaLow) deltaLow = deltaMicros;
        else if (deltaHigh == -1 || deltaMicros > deltaHigh) deltaHigh = deltaMicros;

        if (print) {
            ESP_LOGI(TAG, "Frametime: [Low: %.4f ms | High: %.4f ms]", deltaLow / 1000.f, deltaHigh / 1000.f);
            deltaLow = -1;
            deltaHigh = -1;
        }
    }
}

void System::createGameTask() {
    xTaskCreatePinnedToCore(gameTask, "Game", 10000, game, 10, &System::gameTaskHandle, 1);
}

void System::gameTask(void* pvParameters) {
    Button btn1 = Button(GPIO_NUM_13);
    Button btn2 = Button(GPIO_NUM_14);
    auto* game  = static_cast<Game*>(pvParameters);
    game->init();
    Scene* scene = game->start();

    // TODO Remove game.loop and replace by game.init for a starting screen
    uint64_t lastTime = esp_timer_get_time();
    GAL::switch_frame_buffers();

    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        uint64_t currentTime = esp_timer_get_time();
        scene->update((currentTime - lastTime) / 1000000.f, btn1.isPressed());
        lastTime = currentTime;
        xTaskNotifyGive(System::mainTaskHandle);
    }
}
