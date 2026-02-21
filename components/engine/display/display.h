#pragma once

#include "st7789.h"
#include "color.h"

#include <driver/spi_master.h>

class Display {
public:
    static constexpr uint32_t s_Width = 320;
    static constexpr uint32_t s_Height = 240;

  private:
    // TODO pixelByteSize depends on st7789
    static constexpr uint8_t pixelByteSize = ColorSize;
    // TODO screenSize depends on device (specific connected display hardware)
    static constexpr uint32_t screenSize = s_Width * s_Height;
    // TODO bufferSize depends on see above
    static constexpr uint32_t bufferSize = screenSize * pixelByteSize;

    // TODO Should driver be a value, pointer or reference?
    ST7789 driver;
    // TODO snake case
    Color* active_frame_buffer = nullptr;
    Color* next_frame_buffer   = nullptr;

    Color* frame_buffers[2] = {};

    void initDriver();
    void initBuffers();

  public:
    void init();
    Color getPixel(int index);
    void setPixel(int index, const Color& color);
    void setFrame(Color color);
    void setOrientation(ST7789::orientation_t orientation, uint16_t width, uint16_t height);
    void switchFrameBuffers();
    // TODO rename send to transmit
    void sendActiveBuffer();
    // TODO just for debugging
    bool waitASec = false;
};
