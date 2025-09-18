#include "game.h"
#include "st7789.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>

#define TAG "TRexGame"

#define SWAP16(x) (((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define WHITE SWAP16(0xFFFF)
#define BLACK SWAP16(0x0000)
#define RED SWAP16(0xF800)
#define BLUE SWAP16(0x001F)

#define DELAY(ms) vTaskDelay(ms / portTICK_PERIOD_MS)

ST7789 display;

TaskHandle_t mainTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

void printCurrentCoreInfo(const char *name) {
    ESP_LOGI(TAG, "%s task is running on core: %d", name, xPortGetCoreID());
}

void displayTask(void *pvParameters) {
    printCurrentCoreInfo("Display");
    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        display.send_active_buffer();
        xTaskNotifyGive(mainTaskHandle);
    }
}

void createDisplayTask() {
    xTaskCreatePinnedToCore(
        displayTask,        // Task function
        "Display",          // Task name
        10000,              // Stack size (in words)
        NULL,               // Task parameters (optional)
        1,                  // Task priority
        &displayTaskHandle, // Task handle (optional)
        1                   // Core ID (0 or 1)
    );
}

#define PIXEL_SIZE 2
#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * PIXEL_SIZE)

static inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return (uint16_t)((r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

/*
 * Gibt für step = 0,1,2,... eine Regenbogenfarbe in RGB565 zurück.
 * Ein kompletter Kreis hat 1536 Schritte (6 Segmente à 256).
 * Die Funktion wrappt automatisch (step darf beliebig groß sein).
 */
uint16_t rainbow16(uint32_t step) {
    uint32_t x = step % 1536; // 6 * 256
    uint8_t t = x & 0xFF;     // 0..255 "Interpolation" innerhalb des Segments
    uint8_t r, g, b;

    switch (x >> 8)
    { // Segment 0..5
    case 0:
        r = 255;
        g = t;
        b = 0;
        break; // Rot -> Gelb
    case 1:
        r = 255 - t;
        g = 255;
        b = 0;
        break; // Gelb -> Grün
    case 2:
        r = 0;
        g = 255;
        b = t;
        break; // Grün -> Cyan
    case 3:
        r = 0;
        g = 255 - t;
        b = 255;
        break; // Cyan -> Blau
    case 4:
        r = t;
        g = 0;
        b = 255;
        break; // Blau -> Magenta
    default:   // case 5
        r = 255;
        g = 0;
        b = 255 - t;
        break; // Magenta -> Rot
    }
    return rgb565(r, g, b);
}

extern "C" void app_main() {
    ESP_LOGI(TAG, "Start Dinosaur game...");

    printCurrentCoreInfo("Main");
    display.init();

    Game game(display);

    mainTaskHandle = xTaskGetCurrentTaskHandle();

    createDisplayTask();

    int frames = 0;
    int64_t start_time = esp_timer_get_time();

    display.rotate(ROT_90_CW);

    while (true)
    {
        // display.draw_color(SWAP16(rainbow16(mode)));
        display.draw_placeholder_test(RED);

        xTaskNotifyGive(displayTaskHandle);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        display.switch_frame_buffers();

        frames++;
        int64_t now = esp_timer_get_time();
        if (now - start_time >= 1000000)
        {
            ESP_LOGI(TAG, "FPS: %d (%d)", frames, esp_timer_get_time());
            frames = 0;
            start_time = now;
        }
    }
}
