#include "gal.h"
#include "game.h"

#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG "TRexGame"

TaskHandle_t mainTaskHandle    = NULL;
TaskHandle_t displayTaskHandle = NULL;

void printCurrentCoreInfo(const char* name) {
    ESP_LOGI(TAG, "%s task is running on core: %d", name, xPortGetCoreID());
}

void displayTask(void* pvParameters) {
    printCurrentCoreInfo("Display");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        GAL::send_active_buffer();
        xTaskNotifyGive(mainTaskHandle);
    }
}

void createDisplayTask() {
    xTaskCreatePinnedToCore(displayTask,        // Task function
                            "Display",          // Task name
                            10000,              // Stack size (in words)
                            NULL,               // Task parameters (optional)
                            1,                  // Task priority
                            &displayTaskHandle, // Task handle (optional)
                            1                   // Core ID (0 or 1)
    );
}



extern "C" void app_main() {
    ESP_LOGI(TAG, "Start Dinosaur game...");

    printCurrentCoreInfo("Main");

    GAL::init();
    Game game;
    game.init();

    mainTaskHandle = xTaskGetCurrentTaskHandle();

    createDisplayTask();

    int frames         = 0;
    int64_t start_time = esp_timer_get_time();

    // DISCUSS Here or in game or in device?
    GAL::rotate(ROT_90_CW);

    // DISCUSS Clears the buffers - here or in GAL or in display?
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();

    while (true) {
        game.loop();

        xTaskNotifyGive(displayTaskHandle);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        GAL::switch_frame_buffers();

        frames++;
        int64_t now = esp_timer_get_time();
        if (now - start_time >= 1000000) {
            ESP_LOGI(TAG, "FPS: %d", frames);
            frames     = 0;
            start_time = now;
        }
    }
}
