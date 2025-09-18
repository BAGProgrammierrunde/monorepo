#pragma once

#include "st7789.h"

class GAL {
  private:
    static ST7789& display() {
        // DISCUSS Display should be instantiated in other place
        static ST7789 display;
        return display;
    }

  public:
    GAL() = delete;

    static void init();
    static void draw_placeholder(uint16_t color);
    static void fill_background(uint16_t color);
    static void draw_pixels(uint16_t color, uint16_t count);
    static void draw_cactus_1();
    static void draw_cactus_1_2();

    static void rotate(rotation_t rotation);
    static void switch_frame_buffers();
    static void send_active_buffer();
};
