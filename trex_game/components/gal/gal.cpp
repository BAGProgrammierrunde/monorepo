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

    draw_line(0, 0, width - 1, height - 1, RED);
    draw_line(width - 1, 0, 0, height - 1, RED);
}

void IRAM_ATTR GAL::fill_background(uint16_t color) {
    display().setFrame(color);
}

void IRAM_ATTR GAL::draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, uint16_t fg, uint16_t bg, int scale) {
    constexpr int DST_W = 320, DST_H = 240;
    if (!sprite || srcWidth <= 0 || srcHeight <= 0 || scale <= 0)
        return;

    const int OUT_W = srcWidth * scale;
    const int OUT_H = srcHeight * scale;

    const int off_x = verticalScroll;
    const int off_y = (DST_H - OUT_H) - 9;

    int first_dest_x = off_x < 0 ? 0 : off_x;
    int last_dest_x  = (off_x + OUT_W > DST_W) ? DST_W : (off_x + OUT_W);
    int vis_w        = last_dest_x - first_dest_x;
    if (vis_w <= 0)
        return;

    const int src_scaled_start   = first_dest_x - off_x;
    const int left_partial_incol = src_scaled_start % scale;
    int left_w                   = (left_partial_incol == 0) ? 0 : (scale - left_partial_incol);
    if (left_w > vis_w)
        left_w = vis_w;

    int remaining       = vis_w - left_w;
    const int full_cols = remaining / scale;
    const int right_w   = remaining - full_cols * scale;

    const int sx_start_full = (src_scaled_start / scale) + (left_w ? 1 : 0);
    auto& disp = display();

    for (int sy = 0; sy < srcHeight; ++sy) {
        const int y0 = off_y + sy * scale;
        if (y0 < 0 || (y0 + (scale - 1)) >= DST_H)
            continue;
        int d_base = y0 * DST_W + first_dest_x;
        const int row_bit_base = sy * srcWidth;
        auto bit_at = [&](int sx) -> bool {
            const int bit_index   = row_bit_base + sx;
            const int byte_idx    = bit_index >> 3;      // /8
            const int bit_in_byte = 7 - (bit_index & 7); // %8 und invertiert
            return (sprite[byte_idx] >> bit_in_byte) & 0x1;
        };
        if (left_w) {
            const int sx     = src_scaled_start / scale;
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * DST_W;
                for (int i = 0; i < left_w; ++i) {
                    disp.setPixel(di++, c);
                }
            }
            d_base += left_w;
        }
        for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * DST_W;
                for (int r = 0; r < scale; ++r) {
                    disp.setPixel(di++, c);
                }
            }
            d_base += scale;
        }
        if (right_w) {
            const int sx     = sx_start_full + full_cols;
            const uint16_t c = bit_at(sx) ? fg : bg;

            int d = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * DST_W;
                for (int i = 0; i < right_w; ++i) {
                    disp.setPixel(di++, c);
                }
            }
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
