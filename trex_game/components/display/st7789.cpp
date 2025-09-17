#include "st7789.h"

#include "ground.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>
#include <string.h>

#define TAG "ST7789"

#define PIN_NUM_MOSI  GPIO_NUM_11
#define PIN_NUM_CLK   GPIO_NUM_12
#define PIN_NUM_CS    GPIO_NUM_10
#define PIN_NUM_DC    GPIO_NUM_17
#define PIN_NUM_RST   GPIO_NUM_18
#define PIN_NUM_BCKL  GPIO_NUM_8

#define CLOCK_SPEED_HZ (80 * 1000 * 1000)

#define MAX_CHUNK_BYTES 30720
#define PIXEL_SIZE     2

#define SWAP16(x) (((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define WHITE   SWAP16(0xFFFF)
#define BLACK   SWAP16(0x0000)
#define RED     SWAP16(0xF800)

#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * PIXEL_SIZE)

void ST7789::info() {
  ESP_LOGI(TAG, "Hello from display");
}

void ST7789::init() {
    ESP_LOGI(TAG, "Initialize display ST7789..");
    init_buffers();
    spi_init();
    st7789_init();
    ESP_LOGI(TAG, "Initialization DONE");
}

static volatile bool spi_ready = true;

void IRAM_ATTR ST7789::spi_post_cb(spi_transaction_t *trans) {
    spi_ready = true;
}

void IRAM_ATTR ST7789::draw_vertical_line(int x, uint16_t color) {
    memset(next_frame_buffer, BLACK, BUFFER_SIZE);

    if (x > -1) rotation = 0;

    if (rotation == 0) {
        for (int y = 0; y < LCD_HEIGHT; ++y) {
            next_frame_buffer[y * LCD_WIDTH + x] = WHITE;
            // next_frame_buffer[getRotatedIndex(x, y, LCD_WIDTH * LCD_HEIGHT, rotation)] = y < 100 ? WHITE : BLACK;
        }
    } else if (rotation == 1) {
        for (int y = 0; y < LCD_WIDTH; ++y) {
            next_frame_buffer[y + (LCD_HEIGHT - 1 - x) * LCD_WIDTH] = y < 100 ? WHITE : BLACK;
        }
    } else if (rotation == 2) {
        for (int y = 0; y < LCD_HEIGHT; ++y) {
            next_frame_buffer[(LCD_HEIGHT - 1 - y) * LCD_WIDTH + (LCD_WIDTH - 1 - x)] = y < 100 ? WHITE : BLACK;
        }
    } else if (rotation == 3) {
        for (int y = 0; y < LCD_WIDTH; ++y) {
            next_frame_buffer[(LCD_WIDTH - 1 - y) + x * LCD_WIDTH] = y < 100 ? WHITE : BLACK;
        }
    }
}

void IRAM_ATTR ST7789::draw_color(uint16_t color) {
    // memset(next_frame_buffer, color, BUFFER_SIZE);
    const size_t n = LCD_WIDTH * LCD_HEIGHT;
    std::fill_n(next_frame_buffer, n, color);
}

// void IRAM_ATTR ST7789::draw_cactus_1() {
//     memset(next_frame_buffer, BLACK, BUFFER_SIZE);
//
//     int cactus_width = 90; // 180
//     int cactus_height = 38; // 76
//
//     for (int y = 0; y < cactus_height; ++y) {
//         for (int i = 0; i < cactus_width; ++i) {
//             next_frame_buffer[i*2+LCD_WIDTH*y*2] = cactus_1[i+cactus_width*y];
//         }
//     }
//
//     for (int y = 0; y < cactus_height * 2; ++y) {
//         for (int x = 0; x < cactus_width * 2; ++x) {
//             next_frame_buffer[x+LCD_WIDTH*y] = cactus_1[(x/2)+cactus_width*(y/2)];
//         }
//     }
// }
//
// void IRAM_ATTR ST7789::draw_cactus_1() {
//     constexpr int SRC_W = 90, SRC_H = 38, SCALE = 2;
//     constexpr int DST_W = LCD_WIDTH; // 240
//     constexpr int DST_H = 320;
//
//     const uint16_t* src = cactus_1;
//     uint16_t* dst       = next_frame_buffer;
//
//     // Hintergrund füllen (korrekt für 16-Bit):
//     for (int i = 0; i < DST_W * DST_H; ++i) dst[i] = BLACK;
//
//     const int OUT_W = SRC_W * SCALE;   // 180
//     const int OUT_H = SRC_H * SCALE;   // 76
//
//     // Zentriert platzieren (oder setze off_x/off_y auf 0 für oben-links)
//     const int off_x = (DST_W - OUT_W) / 2;  // 30
//     const int off_y = (DST_H - OUT_H) / 2;  // 122
//
//     for (int sy = 0; sy < SRC_H; ++sy) {
//         const uint16_t* s = src + sy * SRC_W;
//         uint16_t* d0 = dst + (off_y + sy * SCALE) * DST_W + off_x;
//         uint16_t* d1 = d0 + DST_W;
//
//         for (int sx = 0; sx < SRC_W; ++sx) {
//             uint16_t c = s[sx];
//             // 2×2 Block schreiben
//             d0[0] = c; d0[1] = c;
//             d1[0] = c; d1[1] = c;
//             d0 += 2;   d1 += 2;
//         }
//     }
// }

void IRAM_ATTR ST7789::draw_cactus_1() {
    draw_color(BLACK);

    // TODO draw cactus
}

void ST7789::draw_placeholder_test(uint16_t color) {
    draw_color(BLACK);

    const int width  = LCD_HEIGHT;   // ggf. wegen Rotation so belassen
    const int height = LCD_WIDTH;
    const int pixels = width * height;

    // --- Rahmen oben/unten ---
    for (int x = 0; x < width; ++x) {
        next_frame_buffer[x] = RED;                          // y = 0
        next_frame_buffer[pixels - width + x] = RED;         // y = height - 1
    }

    // --- Rahmen links/rechts (Fix: rechter Rand) ---
    for (int y = 0; y < height; ++y) {
        int row = y * width;
        next_frame_buffer[row] = RED;                        // x = 0
        next_frame_buffer[row + (width - 1)] = RED;          // x = width - 1
    }

    // --- Hilfsfunktion: Bresenham-Linie ---
    auto draw_line = [&](int x0, int y0, int x1, int y1, uint16_t color) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;

        while (true) {
            // Bounds-Check (Sicherheit, falls etwas außerhalb liegt)
            if ((unsigned)x0 < (unsigned)width && (unsigned)y0 < (unsigned)height) {
                next_frame_buffer[y0 * width + x0] = color;
            }
            if (x0 == x1 && y0 == y1) break;
            int e2 = err << 1;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    };

    // --- Diagonalen für das "X" ---
    draw_line(0,           0,            width - 1, height - 1, RED); // \  von oben links nach unten rechts
    draw_line(width - 1,   0,            0,         height - 1, RED); // /  von oben rechts nach unten links
}
void ST7789::draw_pixels(uint16_t color, uint16_t count) {
    draw_color(BLACK);

    for (int i = 0; i < count; ++i) {
        next_frame_buffer[i] = color;
    }
}

void ST7789::rotate(rotation_t rotation) {
    st7789_set_rotation(rotation, false,  LCD_WIDTH, LCD_HEIGHT, 0, 0);
}

void ST7789::switch_frame_buffers() {
    uint16_t *tmp_buffer = active_frame_buffer;
    active_frame_buffer = next_frame_buffer;
    next_frame_buffer = tmp_buffer;
}

void ST7789::send_active_buffer() {
    size_t offset = 0;
    spi_transaction_t t[5];
    int queued = 0;
    size_t total_words = LCD_WIDTH * LCD_HEIGHT;
    gpio_set_level(PIN_NUM_DC, 1);
    while (total_words > 0) {
        size_t chunk_words = total_words > (MAX_CHUNK_BYTES / PIXEL_SIZE) ? (MAX_CHUNK_BYTES / PIXEL_SIZE) : total_words;

        t[queued] = {
            .length = chunk_words * PIXEL_SIZE * 8,
            .user = (void*)1,
            .tx_buffer = active_frame_buffer + offset,
        };

        spi_ready = false;
        ESP_ERROR_CHECK(spi_device_queue_trans(spi, &t[queued], portMAX_DELAY));

        offset += chunk_words;
        total_words -= chunk_words;
        queued++;
    }
    for (int i = 0; i < queued; ++i) {
        spi_transaction_t *rtrans;
        ESP_ERROR_CHECK(spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY));
    }
    spi_ready = true;
}

void ST7789::st7789_send_cmd(uint8_t cmd) {
    spi_transaction_t t = {
        .length = 8,
        .user = (void*)0,
        .tx_buffer = &cmd
    };
    gpio_set_level(PIN_NUM_DC, 0);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::st7789_send_data(const void* data, int len) {
    if (len == 0) return;

    spi_transaction_t t = {
        .length = (uint16_t)(len * 8),
        .user = (void*)1,
        .tx_buffer = data
    };
    gpio_set_level(PIN_NUM_DC, 1);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    st7789_send_cmd(0x2A);
    data[0] = x0 >> 8; data[1] = x0 & 0xFF;
    data[2] = x1 >> 8; data[3] = x1 & 0xFF;
    st7789_send_data(data, 4);

    st7789_send_cmd(0x2B);
    data[0] = y0 >> 8; data[1] = y0 & 0xFF;
    data[2] = y1 >> 8; data[3] = y1 & 0xFF;
    st7789_send_data(data, 4);

    st7789_send_cmd(0x2C);
}

void ST7789::st7789_set_rotation(uint8_t rot, bool use_bgr,
                         uint16_t w, uint16_t h,
                         uint16_t x_off, uint16_t y_off)
{
    static const uint8_t ROT2MAD[4] = {0x00, 0x60, 0xC0, 0xA0};
    uint8_t mad = ROT2MAD[rot & 3] | (use_bgr ? 0x08 : 0); // D3=RGB/BGR
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
    uint8_t data1[] = {0x00};
    st7789_send_data(data1, sizeof(data1));

    st7789_send_cmd(0x3A);
    uint8_t data2[] = {0x05};
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
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = MAX_CHUNK_BYTES
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .mode = 0,
        .clock_speed_hz = CLOCK_SPEED_HZ,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 5,
    };

    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));
}

void ST7789::init_buffers() {
    ESP_LOGI(TAG, "BUFFER_SIZE = %d", BUFFER_SIZE);
    ESP_LOGI(TAG, "Free heap: %d", heap_caps_get_free_size(MALLOC_CAP_DMA));
    for (int i = 0; i < 2; ++i) {
        frame_buffers[i] = static_cast<uint16_t*>(heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA));
        if (!frame_buffers[i]) {
            ESP_LOGE(TAG, "Failed to allocate buffer %d", i);
            assert(frame_buffers[i]);
        }
    }

    active_frame_buffer = frame_buffers[0];
    next_frame_buffer = frame_buffers[1];
}
