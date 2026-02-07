#include "st7789.h"

#include "display.h"

#include <array>
#include <driver/gpio.h>
#include <esp_log.h>

#define TAG "ST7789"

#define PIN_NUM_MOSI GPIO_NUM_11 // SDA
#define PIN_NUM_CLK  GPIO_NUM_12 // SCK
#define PIN_NUM_CS   GPIO_NUM_10
#define PIN_NUM_DC   GPIO_NUM_17
#define PIN_NUM_RST  GPIO_NUM_18

#define CLOCK_SPEED_HZ (80 * 1000 * 1000)

void ST7789::init() {
    ESP_LOGD(TAG, "Initialize display driver ST7789..");
    initSPI();
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    sendCmd(0x36);
    uint8_t data1[] = { 0x00 };
    sendData(data1, sizeof(data1));

    sendCmd(0x3A);
    uint8_t data2[] = { 0x05 };
    sendData(data2, sizeof(data2));
#if CONFIG_DISPLAY_INVERSION
    sendCmd(0x21);
#endif
    sendCmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));
    sendCmd(0x29);

    // TODO width / height should not be static -> move to paramters from GAL
    setAddressWindow(0, 0,  240 - 1, 320 - 1);
    ESP_LOGD(TAG, "Initialization successful");
}

// TODO Change snake case to camel case yo
// TODO Check if parameters may be const
// TODO add info about tearing of display driver module design
void ST7789::setOrientation(orientation_t orientation, bool useBgr, uint16_t width, uint16_t height) {
    static constexpr uint8_t ROT2MAD[4] = { 0x00, 0x60, 0xC0, 0xA0 };
    const uint8_t mad                     = ROT2MAD[orientation & 3] | (useBgr ? 0x08 : 0); // D3=RGB/BGR
    sendCmd(0x36);
    sendData(&mad, 1);

    setAddressWindow(0, 0,  width - 1, height - 1);
}

void ST7789::setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    sendCmd(0x2A);
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    sendData(data, 4);

    sendCmd(0x2B);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    sendData(data, 4);

    sendCmd(0x2C);
}

// TODO Add parameters for dataLength and chunkSize
void ST7789::sendDataQueued(uint16_t* data) {
    size_t offset = 0;
    // TODO Transaction array size only works with current setup
    spi_transaction_t t[5];
    int queued         = 0;
    size_t total_words = screenSize;
    gpio_set_level(PIN_NUM_DC, 1);
    while (total_words > 0) {
        // TODO Check if branchless is faster
        const size_t chunk_words = total_words > maxChunkPixels ? maxChunkPixels : total_words;

        t[queued] = {
            .length    = chunk_words * pixelByteSize * 8,
            // TODO Check if nullptr works here maybe
            .user      = reinterpret_cast<void*>(1),
            .tx_buffer = data + offset,
        };

        ESP_ERROR_CHECK(spi_device_queue_trans(spi, &t[queued], portMAX_DELAY));

        offset += chunk_words;
        total_words -= chunk_words;
        queued++;
    }
    for (int i = 0; i < queued; ++i) {
        // TODO Check that rtrans may be moved outside the loop
        spi_transaction_t* rtrans;
        ESP_ERROR_CHECK(spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY));
    }
}

void ST7789::sendCmd(uint8_t cmd) {
    spi_transaction_t t = { .length = 8, .user = nullptr, .tx_buffer = &cmd };
    // TODO Just necessary whenever data changes to cmd
    gpio_set_level(PIN_NUM_DC, 0);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::sendData(const void* data, int len) {
    // DISCUSS necessary?
    if (len == 0)
        return;

    // TODO Check if user may a nullptr?
    spi_transaction_t t = { .length = static_cast<uint16_t>(len * 8), .user = reinterpret_cast<void*>(1), .tx_buffer = data };
    // TODO Just necessary whenever cmd changes to data
    gpio_set_level(PIN_NUM_DC, 1);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}

void ST7789::initSPI() {
    spi_bus_config_t buscfg = { .mosi_io_num     = PIN_NUM_MOSI,
                                .miso_io_num     = -1,
                                .sclk_io_num     = PIN_NUM_CLK,
                                .quadwp_io_num   = -1,
                                .quadhd_io_num   = -1,
                                .max_transfer_sz = maxChunkBytes };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .mode           = 0,
        .clock_speed_hz = CLOCK_SPEED_HZ,
        .spics_io_num   = PIN_NUM_CS,
        .queue_size     = 5,
    };

    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));
}
