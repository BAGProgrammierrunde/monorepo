#include "gal.h"

#include "display/display.h"

#include <algorithm>
#include <cmath>
#include <esp_log.h>
#include <iostream>

#define TAG "GAL"

void GAL::init(Display* pDisplay, const uint16_t pWidth, const uint16_t pHeight, orientation_t orientation) {
    display             = pDisplay;
    width               = pWidth;
    height              = pHeight;
    current_orientation = orientation;
}

void GAL::set_orientation(const orientation_t orientation) {
    if (current_orientation == orientation)
        return;

    if (((current_orientation == PORTRAIT || current_orientation == PORTRAIT_INVERTED) &&
         (orientation == LANDSCAPE || orientation == LANDSCAPE_INVERTED)) ||
        ((current_orientation == LANDSCAPE || current_orientation == LANDSCAPE_INVERTED) &&
         (orientation == PORTRAIT || orientation == PORTRAIT_INVERTED))) {
        const uint16_t tmpWidth = width;
        width                   = height;
        height                  = tmpWidth;
    }

    current_orientation = orientation;
    ESP_LOGD(TAG, "Orientation changed to %d", orientation);
    ESP_LOGD(TAG, "Display size: %dx%d", width, height);
}

void IRAM_ATTR GAL::draw_placeholder(uint16_t color) {
    fill_background(color);

    const int pixels = width * height;

    for (int x = 0; x < width; ++x) {
        display->setPixel(x, color);
        display->setPixel(pixels - width + x, color);
    }

    for (int y = 0; y < height; ++y) {
        int row = y * width;
        display->setPixel(row, RED);               // x = 0
        display->setPixel(row + (width - 1), RED); // x = width - 1
    }

    // --- Hilfsfunktion: Bresenham-Linie ---
    auto draw_line = [&](int x0, int y0, int x1, int y1, uint16_t color) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;

        while (true) {
            // Bounds-Check (Sicherheit, falls etwas außerhalb liegt)
            if ((unsigned)x0 < (unsigned)width && (unsigned)y0 < (unsigned)height) {
                display->setPixel(y0 * width + x0, color);
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
    display->setFrame(color);
}

void IRAM_ATTR GAL::draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, uint16_t fg, uint16_t bg, int scale,
                         bool renderForegroundColorOnly) {
    if (!sprite || srcWidth <= 0 || srcHeight <= 0 || scale <= 0)
        return;

    const int OUT_W = srcWidth * scale;
    const int OUT_H = srcHeight * scale;

    const int off_x = verticalScroll;
    const int off_y = (height - OUT_H) - 9;

    int first_dest_x = off_x < 0 ? 0 : off_x;
    int last_dest_x  = (off_x + OUT_W > width) ? width : (off_x + OUT_W);
    int vis_w        = last_dest_x - first_dest_x;
    if (vis_w <= 0)
        return;

    const int src_scaled_start   = first_dest_x - off_x;
    const int left_partial_incol = src_scaled_start % scale;
    int left_w                   = (left_partial_incol == 0) ? 0 : (scale - left_partial_incol);
    if (left_w > vis_w)
        left_w = vis_w;

    int remaining           = vis_w - left_w;
    const int full_cols     = remaining / scale;
    const int right_w       = remaining - full_cols * scale;
    const int sx_start_full = (src_scaled_start / scale) + (left_w ? 1 : 0);

    for (int sy = 0; sy < srcHeight; ++sy) {
        const int y0 = off_y + sy * scale;
        if (y0 < 0 || (y0 + (scale - 1)) >= height)
            continue;
        int d_base             = y0 * width + first_dest_x;
        const int row_bit_base = sy * srcWidth;
        auto bit_at            = [&](int sx) -> bool {
            const int bit_index   = row_bit_base + sx;
            const int byte_idx    = bit_index >> 3;      // /8
            const int bit_in_byte = 7 - (bit_index & 7); // %8 und invertiert
            return (sprite[byte_idx] >> bit_in_byte) & 0x1;
        };
        if (left_w) {
            const int sx     = src_scaled_start / scale;
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d            = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * width;
                for (int i = 0; i < left_w; ++i) {
                    if (!renderForegroundColorOnly || c != bg) {
                        display->setPixel(di++, c);
                    }
                }
            }
            d_base += left_w;
        }
        for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d            = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * width;
                for (int r = 0; r < scale; ++r) {
                    if (!renderForegroundColorOnly || c != bg) {
                        display->setPixel(di++, c);
                    }
                }
            }
            d_base += scale;
        }
        if (right_w) {
            const int sx     = sx_start_full + full_cols;
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d            = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * width;
                for (int i = 0; i < right_w; ++i) {
                    if (!renderForegroundColorOnly || c != bg) {
                        display->setPixel(di++, c);
                    }
                }
            }
        }
    }
}

void IRAM_ATTR GAL::draw_at(const uint8_t* sprite, int startBitIndex, int srcWidth, int srcHeight, int x, int y, uint16_t fg, uint16_t bg,
                            int scale, bool renderForegroundColorOnly) {
    if (!sprite || srcWidth <= 0 || srcHeight <= 0 || scale <= 0) {
        return;
    }

    const int out_w = srcWidth * scale;
    const int out_h = srcHeight * scale;

    const int off_x = x;
    const int off_y = y;

    if (off_y >= height || (off_y + out_h) <= 0) {
        return;
    }

    int first_dest_x = off_x < 0 ? 0 : off_x;
    int last_dest_x  = (off_x + out_w > width) ? width : (off_x + out_w);
    int vis_w        = last_dest_x - first_dest_x;
    if (vis_w <= 0) {
        return;
    }

    const int src_scaled_start   = first_dest_x - off_x;
    const int left_partial_incol = src_scaled_start % scale;
    int left_w                   = (left_partial_incol == 0) ? 0 : (scale - left_partial_incol);
    if (left_w > vis_w) {
        left_w = vis_w;
    }

    int remaining           = vis_w - left_w;
    const int full_cols     = remaining / scale;
    const int right_w       = remaining - full_cols * scale;
    const int sx_start_full = (src_scaled_start / scale) + (left_w ? 1 : 0);
    const int first_sy      = std::max(0, (-off_y + scale - 1) / scale);
    const int last_sy       = std::min(srcHeight - 1, (height - scale - off_y) / scale);
    if (last_sy < first_sy) {
        return;
    }

    const int fb_w = std::min(width, height);
    const int fb_h = std::max(width, height);

    auto set_pixel_oriented = [&](int lx, int ly, uint16_t color) {
        int px = lx;
        int py = ly;
        switch (current_orientation) {
        case PORTRAIT:
            break;
        case PORTRAIT_INVERTED:
            px = fb_w - 1 - lx;
            py = fb_h - 1 - ly;
            break;
        case LANDSCAPE:
            px = fb_w - 1 - ly;
            py = lx;
            break;
        case LANDSCAPE_INVERTED:
            px = ly;
            py = fb_h - 1 - lx;
            break;
        default:
            break;
        }

        if ((unsigned)px < (unsigned)fb_w && (unsigned)py < (unsigned)fb_h) {
            // TODO remove me later - just for debugging
            if (display->getPixel(py * fb_w + px) != RED)
                display->setPixel(py * fb_w + px, color);
        }
    };

    if (current_orientation == PORTRAIT) {
        for (int sy = first_sy; sy <= last_sy; ++sy) {
            const int y0           = off_y + sy * scale;
            int d_base             = y0 * width + first_dest_x;
            const int row_bit_base = startBitIndex + sy * srcWidth;

            auto bit_at = [&](int sx) -> bool {
                const int bit_index   = row_bit_base + sx;
                const int byte_idx    = bit_index >> 3;      // / 8
                const int bit_in_byte = 7 - (bit_index & 7); // % 8, invertiert
                return (sprite[byte_idx] >> bit_in_byte) & 0x1;
            };

            auto paint_run = [&](int run_w, uint16_t color) {
                if (run_w <= 0) {
                    return;
                }
                if (renderForegroundColorOnly && color == bg) {
                    d_base += run_w;
                    return;
                }
                int d = d_base;
                for (int s = 0; s < scale; ++s) {
                    int di = d + s * width;
                    for (int i = 0; i < run_w; ++i) {
                        // TODO remove me later - just for debugging
                        if (display->getPixel(di+1) != RED)
                            display->setPixel(di++, color);
                    }
                }
                d_base += run_w;
            };

            if (left_w) {
                const uint16_t c = bit_at(src_scaled_start / scale) ? fg : bg;
                paint_run(left_w, c);
            }

            for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
                const uint16_t c = bit_at(sx) ? fg : bg;
                paint_run(scale, c);
            }

            if (right_w) {
                const uint16_t c = bit_at(sx_start_full + full_cols) ? fg : bg;
                paint_run(right_w, c);
            }
        }
        return;
    }

    for (int sy = first_sy; sy <= last_sy; ++sy) {
        const int y0           = off_y + sy * scale;
        int run_x              = first_dest_x;
        const int row_bit_base = startBitIndex + sy * srcWidth;

        auto bit_at = [&](int sx) -> bool {
            const int bit_index   = row_bit_base + sx;
            const int byte_idx    = bit_index >> 3;      // / 8
            const int bit_in_byte = 7 - (bit_index & 7); // % 8, invertiert
            return (sprite[byte_idx] >> bit_in_byte) & 0x1;
        };

        auto paint_run = [&](int run_w, uint16_t color) {
            if (run_w <= 0) {
                return;
            }
            if (renderForegroundColorOnly && color == bg) {
                run_x += run_w;
                return;
            }
            for (int s = 0; s < scale; ++s) {
                for (int i = 0; i < run_w; ++i) {
                    set_pixel_oriented(run_x + i, y0 + s, color);
                }
            }
            run_x += run_w;
        };

        if (left_w) {
            const uint16_t c = bit_at(src_scaled_start / scale) ? fg : bg;
            paint_run(left_w, c);
        }

        for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
            const uint16_t c = bit_at(sx) ? fg : bg;
            paint_run(scale, c);
        }

        if (right_w) {
            const uint16_t c = bit_at(sx_start_full + full_cols) ? fg : bg;
            paint_run(right_w, c);
        }
    }
}

constexpr unsigned int coordsToIndex(int pX, int pY, int pW) {
    return (pX + pY * pW);
}

constexpr unsigned int indexToCoordY(int pIndex, int pW) {
    return std::floor(pIndex / pW);
}
constexpr unsigned int indexToCoordX(int pIndex, int pW) {
    return (pIndex % pW);
}

void IRAM_ATTR GAL::draw_pixels(uint16_t color, uint16_t count) {
    fill_background(BLACK);

    for (int i = 0; i < count; ++i) {
        display->setPixel(i, color);
    }
}

void GAL::switch_frame_buffers() {
    display->switchFrameBuffers();
}

void GAL::send_active_buffer() {
    display->sendActiveBuffer();
}

void IRAM_ATTR GAL::draw_vertical_line(int x, uint16_t color) {
    if (x < 0 || x >= width) {
        return;
    }
    int index = x;
    for (int y = 0; y < height; ++y) {
        display->setPixel(index, color);
        index += width;
    }
}

void IRAM_ATTR GAL::draw_horizontal_line(int y, uint16_t color) {
    if (y < 0 || y >= height) {
        return;
    }
    int index = y * width;
    for (int x = 0; x < width; ++x) {
        display->setPixel(index + x, color);
    }
}
