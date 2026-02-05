#include "system.h"

#include "gal/gal.h"
#include "input/button.h"

#include <esp_log.h>
#include <esp_timer.h>
#include <climits>

#define TAG "System"

void System::init() {
    ESP_LOGI(TAG, "Initializing..");
    mainTaskHandle = xTaskGetCurrentTaskHandle();

    // DISCUSS where should we initialize GAL?
    GAL::init();

    // DISCUSS Here or in game or in device?
    // GAL::rotate(ROT_90_CW);
    GAL::set_orientation(LANDSCAPE);

    // DISCUSS Clears the buffers - here or in GAL or in display?
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
}

void System::start() {
    ESP_LOGI(TAG, "Starting..");
    createGameTask();

    int64_t frameTime   = esp_timer_get_time();
    int64_t deltaMicros = 0;
    int64_t deltaLow    = LLONG_MAX;
    int64_t deltaHigh   = LLONG_MIN;
    int16_t frames      = 0;

    while (true) {
        xTaskNotifyGive(System::gameTaskHandle);
        GAL::send_active_buffer();
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        GAL::switch_frame_buffers();

        int64_t prevFrameTime = frameTime;
        frameTime             = esp_timer_get_time();
        deltaMicros           = frameTime - prevFrameTime;

        bool print = (prevFrameTime % 1000000 > frameTime % 1000000);
        if (deltaMicros < deltaLow)
            deltaLow = deltaMicros;
        else if (deltaMicros > deltaHigh)
            deltaHigh = deltaMicros;

        frames++;

        if (print) {
            ESP_LOGI(TAG, "Frametime: %dfps [Low: %.4f ms | High: %.4f ms]", frames, deltaLow / 1000.f, deltaHigh / 1000.f);
            deltaLow  = LLONG_MAX;
            deltaHigh = LLONG_MIN;
            frames    = 0;
        }
    }
}

void System::createGameTask() {
    xTaskCreatePinnedToCore(gameTask, "Game", 10000, nullptr, 10, &System::gameTaskHandle, 1);
}

void System::gameTask(void* pvParameters) {
    Button btn1 = Button(GPIO_NUM_13);
    Button btn2 = Button(GPIO_NUM_14);

    // TODO Remove game.loop and replace by game.init for a starting screen
    uint64_t lastTime = esp_timer_get_time();
    GAL::switch_frame_buffers();

    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        uint64_t currentTime = esp_timer_get_time();
        if (delayedSceneSwitchFunc.has()) {
            delayedSceneSwitchFunc();
            delayedSceneSwitchFunc.clear();
        }
        assert(scene.has());
        if (scene.has()) {
            scene->update((currentTime - lastTime) / 1000000.f, btn1.isPressed());
        } else {
            ESP_LOGW(TAG, "No active scene set; skipping update");
        }
        lastTime = currentTime;
        xTaskNotifyGive(System::mainTaskHandle);
    }
}
