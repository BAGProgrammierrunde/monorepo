#pragma once

#include <driver/spi_master.h>

// TODO Put pin configuration into template parameters
class ST7789 {
  private:
    static constexpr uint8_t pixelByteSize = 2;
    static constexpr uint32_t screenSize = 240 * 320;
    static constexpr uint32_t bufferSize = screenSize * pixelByteSize;
    static constexpr uint16_t maxChunkPixels = 15360;
    static constexpr uint16_t maxChunkBytes = maxChunkPixels * pixelByteSize;

    spi_device_handle_t spi = nullptr;

    void initSPI();

  public:
    typedef enum {
        portrait  = 0,
        landscape = 1,
        // DISCUSS naming
        portrait_rotated  = 2,
        landscape_rotated = 3,
    } orientation_t;

    void init();
    void setOrientation(orientation_t orientation, bool useBgr, uint16_t width, uint16_t height);
    void setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void sendDataQueued(uint16_t* data);
    void sendCmd(uint8_t cmd);
    void sendData(const void* data, int len);
};
