#pragma once

#include "st7789.h"

#include <driver/spi_master.h>

class Display {
  private:
    // TODO pixelByteSize depends on st7789
    static constexpr uint8_t pixelByteSize = 2;
    // TODO screenSize depends on device (specific connected display hardware)
    static constexpr uint32_t screenSize = 240 * 320;
    // TODO bufferSize depends on see above
    static constexpr uint32_t bufferSize = screenSize * pixelByteSize;

    // TODO Should driver be a value, pointer or reference?
    ST7789 driver;
    // TODO snake case
    uint16_t* active_frame_buffer = nullptr;
    uint16_t* next_frame_buffer   = nullptr;

    uint16_t* frame_buffers[2] = {};

    void initDriver();
    void initBuffers();

  public:
    void init();
    uint16_t getPixel(int index);
    void setPixel(int index, uint16_t color);
    void setFrame(uint16_t color);
    void setOrientation(ST7789::orientation_t orientation, uint16_t width, uint16_t height);
    void switchFrameBuffers();
    // TODO rename send to transmit
    void sendActiveBuffer();
    // TODO just for debugging
    bool waitASec = false;
};
