#include "gal.h"
#include "game.h"
#include "button.h"

#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hal/wdt_hal.h>
#include <esp_err.h>

#define TAG "TRexGame"

TaskHandle_t mainTaskHandle    = NULL;
TaskHandle_t gameTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

void printCurrentCoreInfo(const char* name) {
    ESP_LOGI(TAG, "%s task is running on core: %d", name, xPortGetCoreID());
}

void displayTask(void* pvParameters) {
    printCurrentCoreInfo("Display");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        GAL::send_active_buffer();
        // vTaskDelay(1);
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
                            0                   // Core ID (0 or 1)
    );
}

void gameTask(void* pvParameters) {
    printCurrentCoreInfo("Game");
    Game game;
    game.init();
    // TODO Remove game.loop and replace by game.init for a starting screen
    game.loop();
    GAL::switch_frame_buffers();
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        game.loop();
        xTaskNotifyGive(mainTaskHandle);
    }
}

void createGameTask() {
    xTaskCreatePinnedToCore(gameTask,        // Task function
                            "Game",          // Task name
                            10000,              // Stack size (in words)
                            NULL,               // Task parameters (optional)
                            1,                  // Task priority
                            &gameTaskHandle, // Task handle (optional)
                            1                   // Core ID (0 or 1)
    );
}

extern "C" void app_main() {
    ESP_LOGI(TAG, "Start Dinosaur game...");

    printCurrentCoreInfo("Main");

    Button btn1 = Button(GPIO_NUM_13);
    Button btn2 = Button(GPIO_NUM_14);

    GAL::init();

    mainTaskHandle = xTaskGetCurrentTaskHandle();

    int frames         = 0;
    int64_t start_time = esp_timer_get_time();

    // DISCUSS Here or in game or in device?
    GAL::rotate(ROT_90_CW);

    // DISCUSS Clears the buffers - here or in GAL or in display?
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();
    GAL::fill_background(BLACK);
    GAL::switch_frame_buffers();

    createDisplayTask();
    createGameTask();

    while (true) {
        xTaskNotifyGive(displayTaskHandle);
        xTaskNotifyGive(gameTaskHandle);

        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        // vTaskDelay(1);
        // taskYIELD();
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

        GAL::switch_frame_buffers();

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
