#include "gal.h"

#include <iostream>

#include <algorithm>
#include <cmath>

#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "GAL"

orientation_t GAL::current_orientation = PORTRAIT;

void GAL::init() {
    // DISCUSS Feels wrong here - see header file
    display().init();
}

void GAL::set_orientation(const orientation_t orientation) {
    current_orientation = orientation;
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

void IRAM_ATTR GAL::draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, uint16_t fg, uint16_t bg, int scale,
                         bool renderForegroundColorOnly) {
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
    auto& disp              = display();

    for (int sy = 0; sy < srcHeight; ++sy) {
        const int y0 = off_y + sy * scale;
        if (y0 < 0 || (y0 + (scale - 1)) >= DST_H)
            continue;
        int d_base             = y0 * DST_W + first_dest_x;
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
                int di = d + s * DST_W;
                for (int i = 0; i < left_w; ++i) {
                    if (!renderForegroundColorOnly || c != bg) {
                        disp.setPixel(di++, c);
                    }
                }
            }
            d_base += left_w;
        }
        for (int k = 0, sx = sx_start_full; k < full_cols; ++k, ++sx) {
            const uint16_t c = bit_at(sx) ? fg : bg;
            int d            = d_base;
            for (int s = 0; s < scale; ++s) {
                int di = d + s * DST_W;
                for (int r = 0; r < scale; ++r) {
                    if (!renderForegroundColorOnly || c != bg) {
                        disp.setPixel(di++, c);
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
                int di = d + s * DST_W;
                for (int i = 0; i < right_w; ++i) {
                    if (!renderForegroundColorOnly || c != bg) {
                        disp.setPixel(di++, c);
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

    const bool landscape = (current_orientation == LANDSCAPE) || (current_orientation == LANDSCAPE_INVERTED);
    const int dst_w      = landscape ? LCD_HEIGHT : LCD_WIDTH;
    const int dst_h      = landscape ? LCD_WIDTH : LCD_HEIGHT;

    const int out_w = srcWidth * scale;
    const int out_h = srcHeight * scale;

    const int off_x = x;
    const int off_y = y;

    if (off_y >= dst_h || (off_y + out_h) <= 0) {
        return;
    }

    int first_dest_x = off_x < 0 ? 0 : off_x;
    int last_dest_x  = (off_x + out_w > dst_w) ? dst_w : (off_x + out_w);
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

    int remaining       = vis_w - left_w;
    const int full_cols = remaining / scale;
    const int right_w   = remaining - full_cols * scale;

    const int sx_start_full = (src_scaled_start / scale) + (left_w ? 1 : 0);
    auto& disp              = display();

    const int first_sy = std::max(0, (-off_y + scale - 1) / scale);
    const int last_sy  = std::min(srcHeight - 1, (dst_h - scale - off_y) / scale);
    if (last_sy < first_sy) {
        return;
    }

    if (current_orientation == PORTRAIT) {
        for (int sy = first_sy; sy <= last_sy; ++sy) {
            const int y0           = off_y + sy * scale;
            int d_base             = y0 * dst_w + first_dest_x;
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
                    int di = d + s * dst_w;
                    for (int i = 0; i < run_w; ++i) {
                        disp.setPixel(di++, color);
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

    auto set_pixel_oriented = [&](int lx, int ly, uint16_t color) {
        int px = lx;
        int py = ly;
        switch (current_orientation) {
        case PORTRAIT_INVERTED:
            px = LCD_WIDTH - 1 - lx;
            py = LCD_HEIGHT - 1 - ly;
            break;
        case LANDSCAPE:
            px = LCD_WIDTH - 1 - ly;
            py = lx;
            break;
        case LANDSCAPE_INVERTED:
            px = ly;
            py = LCD_HEIGHT - 1 - lx;
            break;
        default:
            break;
        }
        disp.setPixel(py * LCD_WIDTH + px, color);
    };

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
                const int ly = y0 + s;
                for (int i = 0; i < run_w; ++i) {
                    const int lx = run_x + i;
                    set_pixel_oriented(lx, ly, color);
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

// void IRAM_ATTR GAL::draw_bytes_at(int pX, int pY, int pTextureWidth, int pTextureHeight, int pScale, const uint8_t* pTexture, uint16_t
// pFgColor, uint16_t pBgColor, bool pRenderForegroundColorOnly) {
//     for (int texIndex = 0; texIndex < pTextureWidth * pTextureHeight; texIndex++) {
//         if (pTexture[texIndex] == 0xff) {
//             if (!pRenderForegroundColorOnly) {
//                 int index = coordsToIndex(pX, pY, LCD_HEIGHT) + ((texIndex / pTextureWidth) * LCD_HEIGHT) + (texIndex % pTextureWidth);
//                 display().setPixel(index, pBgColor);
//             }
//         } else {
//             int index = coordsToIndex(pX, pY, LCD_HEIGHT) + ((texIndex / pTextureWidth) * LCD_HEIGHT) + (texIndex % pTextureWidth);
//             display().setPixel(index, pFgColor);
//         }
//     }
// }

void IRAM_ATTR GAL::draw_bytes_at(float pPosX, float pPosY, float pTextureWidth, float pTextureHeight, float pScaleX, float pScaleY,
                                  const uint8_t* pTexture, uint16_t pFgColor, uint16_t pBgColor, bool pIgnoreBg) {
    /*
    unsigned int posIndex = coordsToIndex(pPosX, pPosY, LCD_HEIGHT);
    unsigned int lastTextIndex = pTextureWidth * pScaleX * pTextureHeight * pScaleY;
    for (unsigned int texIndex = 0; texIndex < lastTextIndex; texIndex++) {
        if (pTexture[texIndex] != 0xff)
            display().setPixel(posIndex + indexToCoordY(texIndex, pTextureWidth * pScaleX) / pScaleY * LCD_HEIGHT + indexToCoordX(texIndex,
    pTextureWidth * pScaleX) / pScaleX, pFgColor); else if (!pIgnoreBg) display().setPixel(posIndex + indexToCoordY(texIndex, pTextureWidth
    * pScaleX) / pScaleY * LCD_HEIGHT + indexToCoordX(texIndex, pTextureWidth * pScaleX) / pScaleX, pBgColor);
    }
    */

    // unsigned int posIndex     = coordsToIndex(pPosX, pPosY, pTextureWidth);
    // unsigned int lastTexIndex = pTextureWidth * pScaleX * pTextureHeight * pScaleY;
    // for (unsigned int texIndex = 0; texIndex < lastTexIndex; texIndex++) {
    //     unsigned int texCoordX = indexToCoordX(texIndex, pTextureWidth * pScaleY);
    //     unsigned int texCoordY = indexToCoordY(texIndex, pTextureWidth * pScaleX);
    //
    //     if (pTexture[coordsToIndex(std::round(texCoordX / pScaleX), std::round(texCoordY / pScaleY), pTextureWidth)] != 0xff)
    //         display().setPixel(posIndex + texCoordY * LCD_HEIGHT + texCoordX, pFgColor);
    //     else if (!pIgnoreBg)
    //         display().setPixel(posIndex + texCoordY * LCD_HEIGHT + texCoordX, pBgColor);
    // }

    // unsigned int posIndex = coordsToIndex(pPosX, pPosY, LCD_HEIGHT);
    // for (unsigned int texY = 0; texY < pTextureHeight * pScaleY; texY++)
    // {
    //     for (unsigned int texX = 0; texX < pTextureWidth * pScaleX; texX++)
    //     {
    //         if (pTexture[coordsToIndex(std::round(texX / pScaleX), std::round(texY / pScaleY), pTextureWidth)] != 0xff) display().setPixel(posIndex + texY * LCD_HEIGHT + texX, pFgColor);
    //         else if (!pIgnoreBg) display().setPixel(posIndex + texY * LCD_HEIGHT + texX, pBgColor);
    //     }
    // }

    constexpr unsigned int screenW = LCD_HEIGHT;
    constexpr unsigned int screenH = LCD_WIDTH;
    unsigned int posIndex = coordsToIndex(pPosX, pPosY, screenW);

    float texYMin = (pPosY >= 0 ? 0 : -pPosY);
    float texYMax = (pPosY + pTextureHeight * pScaleY <= screenH ? pTextureHeight * pScaleY : (pPosY + pTextureHeight * pScaleY) - screenH);
    float texXMin = (pPosX >= 0 ? 0 : -pPosX);
    float texXMax = (pPosX + pTextureWidth * pScaleX <= screenW ? pTextureWidth * pScaleX : (pPosX + pTextureWidth * pScaleX) - screenW);
    for (unsigned int texY = texYMin; texY < texYMax; texY++)
    {
        for (unsigned int texX = texXMin; texX < texXMax; texX++)
        {
            if (pTexture[coordsToIndex(std::round(((float)texX) / pScaleX), std::round(((float)texY) / pScaleY), pTextureWidth)] != 0xff) display().setPixel(posIndex + texY * screenW + texX, pFgColor);
            else if (!pIgnoreBg) display().setPixel(posIndex + texY * screenW + texX, pBgColor);
        }
    }
}

/*
void IRAM_ATTR GAL::draw_bytes_at(int pX, int pY, int pTextureWidth, int pTextureHeight, int pScale, const uint8_t* pTexture, uint16_t
pFgColor, uint16_t pBgColor, bool pIgnoreBg) { constexpr int DST_W = LCD_HEIGHT; constexpr int DST_H = LCD_WIDTH;

    if (!pTexture || pTextureWidth <= 0 || pTextureHeight <= 0 || pScale <= 0)
        return;

    auto& disp = display();

    for (int texY = 0; texY < pTextureHeight; ++texY) {
        const int dstY0 = pY + texY * pScale;
        const int dstY1 = dstY0 + pScale;
        if (dstY0 >= DST_H || dstY1 <= 0)
            continue;

        const int yStart = std::max(dstY0, 0);
        const int yEnd   = std::min(dstY1, DST_H);

        for (int texX = 0; texX < pTextureWidth; ++texX) {
            const uint8_t texVal = pTexture[texY * pTextureWidth + texX];
            const bool drawFg    = texVal != 0xff;
            if (!drawFg && pIgnoreBg)
                continue;

            const int dstX0 = pX + texX * pScale;
            const int dstX1 = dstX0 + pScale;
            if (dstX0 >= DST_W || dstX1 <= 0)
                continue;

            const int xStart = std::max(dstX0, 0);
            const int xEnd   = std::min(dstX1, DST_W);
            const uint16_t c = drawFg ? pFgColor : pBgColor;

            for (int dy = yStart; dy < yEnd; ++dy) {
                int idx = coordsToIndex(xStart, dy, LCD_HEIGHT);
                for (int dx = xStart; dx < xEnd; ++dx) {
                    disp.setPixel(idx++, c);
                }
            }
        }
    }
}
*/

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

void GAL::set_fullscreen() {
    display().set_address_window(0, 0, LCD_HEIGHT - 1, LCD_WIDTH - 1);
}

void IRAM_ATTR GAL::draw_vertical_line(int x, uint16_t color) {
    constexpr int screenW = LCD_WIDTH;
    constexpr int screenH = LCD_HEIGHT;
    if (x < 0 || x >= screenW) {
        return;
    }
    auto& disp = display();
    int index  = x;
    for (int y = 0; y < screenH; ++y) {
        disp.setPixel(index, color);
        index += screenW;
    }
}

void IRAM_ATTR GAL::draw_horizontal_line(int y, uint16_t color) {
    constexpr int screenW = LCD_WIDTH;
    constexpr int screenH = LCD_HEIGHT;
    if (y < 0 || y >= screenH) {
        return;
    }
    auto& disp = display();
    int index  = y * screenW;
    for (int x = 0; x < screenW; ++x) {
        disp.setPixel(index + x, color);
    }
}
