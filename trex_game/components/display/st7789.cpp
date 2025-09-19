#include "st7789.h"

#include <array>
#include <driver/gpio.h>
#include <esp_log.h>
#include <string.h>

#define TAG "ST7789"

#define PIN_NUM_MOSI GPIO_NUM_11
#define PIN_NUM_CLK  GPIO_NUM_12
#define PIN_NUM_CS   GPIO_NUM_10
#define PIN_NUM_DC   GPIO_NUM_17
#define PIN_NUM_RST  GPIO_NUM_18
#define PIN_NUM_BCKL GPIO_NUM_8

#define CLOCK_SPEED_HZ (80 * 1000 * 1000)

#define MAX_CHUNK_BYTES    30720
#define DISPLAY_PIXEL_SIZE 2

#define DISPLAY_BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * DISPLAY_PIXEL_SIZE)

void ST7789::setPixel(int index, uint16_t color) {
    next_frame_buffer[index] = color;
}

void ST7789::setFrame(uint16_t color) {
    const size_t n = LCD_WIDTH * LCD_HEIGHT;
    std::fill_n(next_frame_buffer, n, color);
}

void ST7789::init() {
    ESP_LOGI(TAG, "Initialize display ST7789..");
    init_buffers();
    spi_init();
    st7789_init();
    ESP_LOGI(TAG, "Initialization DONE");
}

static volatile bool spi_ready = true;

void IRAM_ATTR ST7789::spi_post_cb(spi_transaction_t* trans) {
    spi_ready = true;
}

void ST7789::rotate(rotation_t rotation) {
    st7789_set_rotation(rotation, false, LCD_WIDTH, LCD_HEIGHT, 0, 0);
}

void ST7789::switch_frame_buffers() {
    uint16_t* tmp_buffer = active_frame_buffer;
    active_frame_buffer  = next_frame_buffer;
    next_frame_buffer    = tmp_buffer;
}

void ST7789::send_active_buffer() {
    size_t offset = 0;
    spi_transaction_t t[5];
    int queued         = 0;
    size_t total_words = LCD_WIDTH * LCD_HEIGHT;
    gpio_set_level(PIN_NUM_DC, 1);
    while (total_words > 0) {
        size_t chunk_words = total_words > (MAX_CHUNK_BYTES / DISPLAY_PIXEL_SIZE) ? (MAX_CHUNK_BYTES / DISPLAY_PIXEL_SIZE) : total_words;

        t[queued] = {
            .length    = chunk_words * DISPLAY_PIXEL_SIZE * 8,
            .user      = (void*)1,
            .tx_buffer = active_frame_buffer + offset,
        };

        spi_ready = false;
        ESP_ERROR_CHECK(spi_device_queue_trans(spi, &t[queued], portMAX_DELAY));

        offset += chunk_words;
        total_words -= chunk_words;
        queued++;
    }
    for (int i = 0; i < queued; ++i) {
        spi_transaction_t* rtrans;
        ESP_ERROR_CHECK(spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY));
    }
    spi_ready = true;
}

void ST7789::st7789_send_cmd(uint8_t cmd) {
    spi_transaction_t t = { .length = 8, .user = (void*)0, .tx_buffer = &cmd };
    gpio_set_level(PIN_NUM_DC, 0);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::st7789_send_data(const void* data, int len) {
    if (len == 0)
        return;

    spi_transaction_t t = { .length = (uint16_t)(len * 8), .user = (void*)1, .tx_buffer = data };
    gpio_set_level(PIN_NUM_DC, 1);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    st7789_send_cmd(0x2A);
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    st7789_send_data(data, 4);

    st7789_send_cmd(0x2B);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    st7789_send_data(data, 4);

    st7789_send_cmd(0x2C);
}

void ST7789::st7789_set_rotation(uint8_t rot, bool use_bgr, uint16_t w, uint16_t h, uint16_t x_off, uint16_t y_off) {
    static const uint8_t ROT2MAD[4] = { 0x00, 0x60, 0xC0, 0xA0 };
    uint8_t mad                     = ROT2MAD[rot & 3] | (use_bgr ? 0x08 : 0); // D3=RGB/BGR
    st7789_send_cmd(0x36);
    st7789_send_data(&mad, 1);

    // Breite/Höhe für 90/270 tauschen
    uint16_t dw = (rot & 1) ? h : w;
    uint16_t dh = (rot & 1) ? w : h;

    // Fenster inkl. panel-spezifischer Offsets setzen
    set_address_window(x_off, y_off, x_off + dw - 1, y_off + dh - 1);
}

void ST7789::st7789_init() {
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    st7789_send_cmd(0x36);
    uint8_t data1[] = { 0x00 };
    st7789_send_data(data1, sizeof(data1));

    st7789_send_cmd(0x3A);
    uint8_t data2[] = { 0x05 };
    st7789_send_data(data2, sizeof(data2));
#if CONFIG_DISPLAY_INVERSION
    st7789_send_cmd(0x21);
#endif
    st7789_send_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));
    st7789_send_cmd(0x29);

    gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_BCKL, 1);

    set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}

void ST7789::spi_init() {
    spi_bus_config_t buscfg = { .mosi_io_num     = PIN_NUM_MOSI,
                                .miso_io_num     = -1,
                                .sclk_io_num     = PIN_NUM_CLK,
                                .quadwp_io_num   = -1,
                                .quadhd_io_num   = -1,
                                .max_transfer_sz = MAX_CHUNK_BYTES };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .mode           = 0,
        .clock_speed_hz = CLOCK_SPEED_HZ,
        .spics_io_num   = PIN_NUM_CS,
        .queue_size     = 5,
    };

    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));
}

void ST7789::init_buffers() {
    ESP_LOGI(TAG, "BUFFER_SIZE = %d", DISPLAY_BUFFER_SIZE);
    ESP_LOGI(TAG, "Free heap: %d", heap_caps_get_free_size(MALLOC_CAP_DMA));
    for (int i = 0; i < 2; ++i) {
        frame_buffers[i] = static_cast<uint16_t*>(heap_caps_malloc(DISPLAY_BUFFER_SIZE, MALLOC_CAP_DMA));
        if (!frame_buffers[i]) {
            ESP_LOGE(TAG, "Failed to allocate buffer %d", i);
            assert(frame_buffers[i]);
        }
    }

    active_frame_buffer = frame_buffers[0];
    next_frame_buffer   = frame_buffers[1];
}
