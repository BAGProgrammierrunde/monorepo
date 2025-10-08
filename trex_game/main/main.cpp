#include "button.h"
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

void waitForButtonPress(Button& button, const char* processName, const char* id) {
    while (true) {
            if (button.isPressed()) {
                ESP_LOGI(TAG, "%s process is done", processName);
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "%s", id);
        }
}

void displayTask(void* pvParameters) {
    printCurrentCoreInfo("Display");
    Button btn2 = Button(GPIO_NUM_14);
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        waitForButtonPress(btn2, "Display", "2");


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

void app_main2() {
    ESP_LOGI(TAG, "Start Dinosaur game...");

    mainTaskHandle = xTaskGetCurrentTaskHandle();

    printCurrentCoreInfo("Main");

    Button btn1 = Button(GPIO_NUM_13);

    createDisplayTask();

    while (true) {
        xTaskNotifyGive(displayTaskHandle);

        waitForButtonPress(btn1, "Game", "1");

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(TAG, "LOOP......");
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP_LOGI(TAG, "LOOP DONE");
    }
}
