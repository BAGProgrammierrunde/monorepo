#include "gal.h"

#include <string.h>

#include <algorithm>
#include <array>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ground.h"

#define TAG "GAL"

#define CLOCK_SPEED_HZ (80 * 1000 * 1000)

#define MAX_CHUNK_BYTES 30720
#define PIXEL_SIZE      2

#define SWAP16(x) (((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define WHITE     SWAP16(0xFFFF)
#define BLACK     SWAP16(0x0000)
#define RED       SWAP16(0xF800)

#define BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * PIXEL_SIZE)

void GAL::init() {
    // DISCUSS Feels wrong here - see header file
    display().init();
}

void IRAM_ATTR GAL::draw_placeholder(uint16_t color) {
    fill_background(color);

    const int width  = LCD_HEIGHT;
    const int height = LCD_WIDTH;
    const int pixels = width * height;

    for (int x = 0; x < width; ++x) {
        display().setPixel(x, color);
        display().setPixel(pixels - width + x, color);
    }

    for (int y = 0; y < height; ++y) {
        int row = y * width;
        display().setPixel(row, RED);               // x = 0
        display().setPixel(row + (width - 1), RED); // x = width - 1
    }

    // --- Hilfsfunktion: Bresenham-Linie ---
    auto draw_line = [&](int x0, int y0, int x1, int y1, uint16_t color) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;

        while (true) {
            // Bounds-Check (Sicherheit, falls etwas außerhalb liegt)
            if ((unsigned)x0 < (unsigned)width && (unsigned)y0 < (unsigned)height) {
                display().setPixel(y0 * width + x0, color);
            }
            if (x0 == x1 && y0 == y1)
                break;
            int e2 = err << 1;
            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
    };

    draw_line(0, 0, width - 1, height - 1,
              RED); // \  von oben links nach unten rechts
    draw_line(width - 1, 0, 0, height - 1,
              RED); // /  von oben rechts nach unten links
}

void IRAM_ATTR GAL::fill_background(uint16_t color) {
    display().setFrame(color);
}

void IRAM_ATTR GAL::draw_cactus_1_2() {
    fill_background(BLACK);

    int cactus_width  = 90; // 180
    int cactus_height = 38; // 76

    for (int y = 0; y < cactus_height; ++y) {
        for (int i = 0; i < cactus_width; ++i) {
            display().setPixel(i * 2 + LCD_WIDTH * y * 2, cactus_1[i + cactus_width * y]);
        }
    }

    for (int y = 0; y < cactus_height * 2; ++y) {
        for (int x = 0; x < cactus_width * 2; ++x) {
            display().setPixel(x + LCD_WIDTH * y, cactus_1[(x / 2) + cactus_width * (y / 2)]);
        }
    }
}

void IRAM_ATTR GAL::draw_cactus_1() {
    constexpr int SRC_W = 90, SRC_H = 38, SCALE = 3;
    constexpr int DST_W = 320;
    constexpr int DST_H = 240;

    const uint16_t* src = cactus_1;

    fill_background(BLACK);

    const int OUT_W = SRC_W * SCALE; // 180
    const int OUT_H = SRC_H * SCALE; // 76

    // Zentriert platzieren (oder setze off_x/off_y auf 0 für oben-links)
    const int off_x = (DST_W - OUT_W) / 2; // 30
    const int off_y = (DST_H - OUT_H) / 2; // 122

    for (int sy = 0; sy < SRC_H; ++sy) {
        const uint16_t* s = src + sy * SRC_W;
        int d0            = (off_y + sy * SCALE) * DST_W + off_x;
        int d1            = d0 + DST_W;
        int d2            = d1 + DST_W;

        for (int sx = 0; sx < SRC_W; ++sx) {
            uint16_t c = s[sx];
            display().setPixel(d0++, c);
            display().setPixel(d0++, c);
            display().setPixel(d0++, c);
            display().setPixel(d1++, c);
            display().setPixel(d1++, c);
            display().setPixel(d1++, c);
            display().setPixel(d2++, c);
            display().setPixel(d2++, c);
            display().setPixel(d2++, c);
        }
    }
}

void IRAM_ATTR GAL::draw_pixels(uint16_t color, uint16_t count) {
    fill_background(BLACK);

    for (int i = 0; i < count; ++i) {
        display().setPixel(i, color);
    }
}

void GAL::rotate(rotation_t rotation) {
    display().rotate(rotation);
}
void GAL::switch_frame_buffers() {
    display().switch_frame_buffers();
}

void GAL::send_active_buffer() {
    display().send_active_buffer();
}
