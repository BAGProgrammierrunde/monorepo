#pragma once

#include <driver/spi_master.h>

typedef enum {
    ROT_0      = 0,
    ROT_90_CW  = 1,
    ROT_180    = 2,
    ROT_270_CW = 3,
} rotation_t;

class ST7789 {
  private:
    static constexpr uint8_t pixelByteSize = 2;
    static constexpr uint32_t screenSize = 240 * 320;
    static constexpr uint32_t bufferSize = screenSize * pixelByteSize;
    static constexpr uint16_t maxChunkPixels = 15360;
    static constexpr uint16_t maxChunkBytes = maxChunkPixels * pixelByteSize;

    uint16_t* active_frame_buffer = nullptr;
    uint16_t* next_frame_buffer   = nullptr;

    uint16_t* frame_buffers[2] = {};

    spi_device_handle_t spi = nullptr;

    int rotation = ROT_0;

    void init_buffers();
    void spi_init();
    void st7789_init();
    void st7789_set_rotation(uint8_t rot, bool use_bgr, uint16_t width, uint16_t height);

  public:
    void setPixel(int index, uint16_t color);
    void setFrame(uint16_t color);
    void init();
    void spi_post_cb(spi_transaction_t* trans);
    void rotate(rotation_t rotation, uint16_t width, uint16_t height);
    void switch_frame_buffers();
    void send_active_buffer();
    void st7789_send_cmd(uint8_t cmd);
    void st7789_send_data(const void* data, int len);
    void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
};
