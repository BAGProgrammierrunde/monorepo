#include "display.h"

#include <array>
#include <esp_log.h>

#define TAG "ST7789"

#define PIN_NUM_MOSI GPIO_NUM_11 // SDA
#define PIN_NUM_CLK  GPIO_NUM_12 // SCK
#define PIN_NUM_CS   GPIO_NUM_10
#define PIN_NUM_DC   GPIO_NUM_17
#define PIN_NUM_RST  GPIO_NUM_18

#define CLOCK_SPEED_HZ (80 * 1000 * 1000)

void Display::init() {
    initBuffers();
    initDriver();
}

uint16_t Display::getPixel(int index) {
    return next_frame_buffer[index];
}

void Display::setPixel(int index, uint16_t color) {
    next_frame_buffer[index] = color;
}

void Display::setFrame(uint16_t color) {
    std::fill_n(next_frame_buffer, screenSize, color);
}

void Display::setOrientation(ST7789::orientation_t orientation, uint16_t width, uint16_t height) {
    // INFO Our display abstracts useBgr here
   driver.setOrientation(orientation, false, width, height);
}

void Display::switchFrameBuffers() {
    uint16_t* tmp_buffer = active_frame_buffer;
    active_frame_buffer  = next_frame_buffer;
    next_frame_buffer    = tmp_buffer;
}

void Display::sendActiveBuffer() {
    // TODO just for debugging
    bool tmp = waitASec;
    driver.sendDataQueued(active_frame_buffer);
    if (tmp) {
        waitASec = false;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Display::initDriver() {
    driver.init();
}

void Display::initBuffers() {
    ESP_LOGI(TAG, "BUFFER_SIZE = %d", bufferSize);
    ESP_LOGI(TAG, "Free heap: %d", heap_caps_get_free_size(MALLOC_CAP_DMA));
    for (int i = 0; i < 2; ++i) {
        frame_buffers[i] = static_cast<uint16_t*>(heap_caps_malloc(bufferSize, MALLOC_CAP_DMA));
        if (!frame_buffers[i]) {
            ESP_LOGE(TAG, "Failed to allocate buffer %d", i);
            assert(frame_buffers[i]);
        }
    }

    active_frame_buffer = frame_buffers[0];
    next_frame_buffer   = frame_buffers[1];
}
