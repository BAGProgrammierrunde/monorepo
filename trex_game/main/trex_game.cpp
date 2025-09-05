#include "game.h"
#include "st7789.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "TRexGame"

#define SWAP16(x) (((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define WHITE   SWAP16(0xFFFF)
#define BLACK   SWAP16(0x0000)

ST7789 display;

TaskHandle_t mainTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

void printCurrentCoreInfo(const char *name) {
  ESP_LOGI(TAG, "%s task is running on core: %d", name, xPortGetCoreID());
}

void displayTask(void *pvParameters) {
  printCurrentCoreInfo("Display");
  while (true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    display.send_active_buffer();
    xTaskNotifyGive(mainTaskHandle);
  }
}

void createDisplayTask() {
  xTaskCreatePinnedToCore(
        displayTask,      // Task function
        "Display",   // Task name
        10000,       // Stack size (in words)
        NULL,        // Task parameters (optional)
        1,           // Task priority
        &displayTaskHandle, // Task handle (optional)
        1            // Core ID (0 or 1)
      );
}

#define PIXEL_SIZE     2
#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * PIXEL_SIZE)

extern "C" void app_main() {
  ESP_LOGI(TAG, "Start Dinosaur game...");

  printCurrentCoreInfo("Main");

  display.init();

  Game game(display);

  mainTaskHandle = xTaskGetCurrentTaskHandle();

  createDisplayTask();

  int frames = 0;
  int64_t start_time = esp_timer_get_time();
  int sw = 1;
  int current_pos = 0;

  while (true) {
    // display.draw_vertical_line(current_pos, WHITE);
    display.draw_cactus_1();
    xTaskNotifyGive(displayTaskHandle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    display.switch_frame_buffers();

    current_pos += sw;
    if (current_pos >= LCD_HEIGHT - 1) {
      sw = -1;
      current_pos = LCD_HEIGHT - 1;
    }
    if (current_pos <= 0) {
      sw = 1;
      current_pos = 0;
    }

    frames++;
    int64_t now = esp_timer_get_time();
    if (now - start_time >= 1000000) {
      ESP_LOGI(TAG, "FPS: %d", frames);
      frames = 0;
      start_time = now;
    }
  }
}
