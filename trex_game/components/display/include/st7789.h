#pragma once

#include "driver/spi_master.h"

#define LCD_WIDTH     240
#define LCD_HEIGHT    320

class ST7789
{
private:
  uint16_t *active_frame_buffer = nullptr;
  uint16_t *next_frame_buffer = nullptr;

  uint16_t *frame_buffers[2] = {};

  spi_device_handle_t spi = nullptr;

  int rotation = 0;

  void init_buffers();
  void spi_init();
  void st7789_init();
public:
  void init();
  void switch_frame_buffers();
  void spi_post_cb(spi_transaction_t *trans);
  void draw_vertical_line(int x, uint16_t color);
  void send_active_buffer();
  void st7789_send_cmd(uint8_t cmd);
  void st7789_send_data(const void *data, int len);
  void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void info();
};
