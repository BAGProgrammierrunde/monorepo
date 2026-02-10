#pragma once

#include "display/st7789.h"
#include "display/display.h"

#define SWAP16(x) ((uint16_t)((((uint16_t)(x)) << 8) | (((uint16_t)(x)) >> 8)))
#define WHITE     SWAP16(0xFFFFu)
#define BLACK     SWAP16(0x0000u)
#define RED       SWAP16(0xF800u)

class GAL {
  private:
    static inline Display* display;
    static inline ST7789::orientation_t current_rotation = ST7789::landscape;
    static inline uint16_t width;
    static inline uint16_t height;
    static inline orientation_t current_orientation = PORTRAIT;

  public:
    GAL() = delete;

    static void init(Display* pDisplay, uint16_t pWidth, uint16_t pHeight, orientation_t orientation);
    static void set_orientation(orientation_t orientation);
    static void draw_placeholder(uint16_t color);
    static void fill_background(uint16_t color);
    static void draw_pixels(uint16_t color, uint16_t count);
    static void draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, uint16_t foregroundColor,
                     uint16_t backgroundColor, int scale, bool renderForegroundColorOnly = false);
    static void draw_at(const uint8_t* sprite, int startBitIndex, int srcWidth, int srcHeight, int x, int y, uint16_t fg, uint16_t bg, int scale,
                        bool renderForegroundColorOnly = false);
    static void switch_frame_buffers();
    static void send_active_buffer();
    static void draw_vertical_line(int x, uint16_t color);
    static void draw_horizontal_line(int y, uint16_t color);
};
