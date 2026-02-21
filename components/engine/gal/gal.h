#pragma once

#include "display/display.h"
#include "display/st7789.h"

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
    static void draw_placeholder(const Color& color);
    static void fill_background(const Color& color);
    static void draw_pixels(const Color& color, uint16_t count);
    static void draw(const uint8_t* sprite, int srcWidth, int srcHeight, int verticalScroll, const Color& foregroundColor,
                     const Color& backgroundColor, int scale, bool renderForegroundColorOnly = false);
    static void draw_at(const uint8_t* sprite, int startBitIndex, int srcWidth, int srcHeight, int x, int y, const Color& fg,
                        const Color& bg, int scale, bool renderForegroundColorOnly = false);
    static void switch_frame_buffers();
    static void send_active_buffer();
    static void draw_vertical_line(int x, const Color& color);
    static void draw_horizontal_line(int y, const Color& color);
};
