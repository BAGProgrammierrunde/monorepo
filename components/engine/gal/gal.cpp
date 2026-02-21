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

void IRAM_ATTR GAL::draw_placeholder(const Color& color) {
    fill_background(BLACK);

    if (width <= 10 || height <= 10) {
        return;
    }

    constexpr int margin  = 5;
    const int left        = margin;
    const int right       = static_cast<int>(width) - 1 - margin;
    const int top         = margin;
    const int bottom      = static_cast<int>(height) - 1 - margin;
    const int bufferWidth = static_cast<int>(height);

    auto to_ccw_buffer_index = [&](int x, int y) -> int {
        // Logical (x,y) with rotated width/height -> CCW-rotated linear buffer index.
        return (static_cast<int>(width) - 1 - x) * bufferWidth + y;
    };

    for (int x = left; x <= right; ++x) {
        display->setPixel(to_ccw_buffer_index(x, top), color);
        display->setPixel(to_ccw_buffer_index(x, bottom), color);
    }

    for (int y = top; y <= bottom; ++y) {
        display->setPixel(to_ccw_buffer_index(left, y), color);
        display->setPixel(to_ccw_buffer_index(right, y), color);
    }
}

void IRAM_ATTR GAL::fill_background(const Color& color) {
    display->setFrame(color);
}

void IRAM_ATTR GAL::draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, const Color& fg, const Color& bg,
                         int scale, bool renderForegroundColorOnly) {
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
            const int sx  = src_scaled_start / scale;
            const Color c = bit_at(sx) ? fg : bg;
            int d         = d_base;
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
            const Color c = bit_at(sx) ? fg : bg;
            int d         = d_base;
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
            const int sx  = sx_start_full + full_cols;
            const Color c = bit_at(sx) ? fg : bg;
            int d         = d_base;
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

void IRAM_ATTR GAL::draw_at(const uint8_t* sprite, int startBitIndex, int srcWidth, int srcHeight, int x, int y, const Color& fg,
                            const Color& bg, int scale, bool renderForegroundColorOnly) {
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

    auto set_pixel_oriented = [&](int lx, int ly, Color color) {
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

            auto paint_run = [&](int run_w, Color color) {
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
                        if (display->getPixel(di + 1) != RED)
                            display->setPixel(di++, color);
                    }
                }
                d_base += run_w;
            };

            if (left_w) {
                const Color c = bit_at(src_scaled_start / scale) ? fg : bg;
                paint_run(left_w, c);
            }

            for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
                const Color c = bit_at(sx) ? fg : bg;
                paint_run(scale, c);
            }

            if (right_w) {
                const Color c = bit_at(sx_start_full + full_cols) ? fg : bg;
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

        auto paint_run = [&](int run_w, Color color) {
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
            const Color c = bit_at(src_scaled_start / scale) ? fg : bg;
            paint_run(left_w, c);
        }

        for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
            const Color c = bit_at(sx) ? fg : bg;
            paint_run(scale, c);
        }

        if (right_w) {
            const Color c = bit_at(sx_start_full + full_cols) ? fg : bg;
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

void IRAM_ATTR GAL::draw_pixels(const Color& color, uint16_t count) {
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

void IRAM_ATTR GAL::draw_vertical_line(int x, const Color& color) {
    if (x < 0 || x >= width) {
        return;
    }
    int index = x;
    for (int y = 0; y < height; ++y) {
        display->setPixel(index, color);
        index += width;
    }
}

void IRAM_ATTR GAL::draw_horizontal_line(int y, const Color& color) {
    if (y < 0 || y >= height) {
        return;
    }
    int index = y * width;
    for (int x = 0; x < width; ++x) {
        display->setPixel(index + x, color);
    }
}
