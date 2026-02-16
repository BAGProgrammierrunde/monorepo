#pragma once

constexpr std::uint16_t SWAP16Bit(std::uint16_t pX) {
    return ((pX << 8) | (pX >> 8));
}

namespace page {
    class Display {
    private:
        static constexpr unsigned int width = 240;
        static constexpr unsigned int height = 320;
        static constexpr unsigned int pixelBytes = 2;
        static constexpr unsigned int spiClockSpeedHz = 80*1000*1000;
        static constexpr unsigned int maxBufferChunkSendBytes = 30720;

        gpio_num_t pinSDA;
        gpio_num_t pinSCK;
        gpio_num_t pinCS;
        gpio_num_t pinDC;
        gpio_num_t pinRST;
        spi_device_handle_t spi = nullptr;

        std::uint32_t* frameBuffer[2];
        bool curFrameBuffer; // Currently rendered to, other is currently sent to display

        void sendCmd(uint8_t pCmd) {
            spi_transaction_t t = {
                .length = 8,
                .user = (void*)0,
                .tx_buffer = &pCmd
            };
            gpio_set_level(pinDC, 0);
            spi_device_transmit(spi, &t);
        }

        void sendData(const void* pData, int pLength) {
            if (pLength > 0)
            {
                spi_transaction_t t = {
                    .length = (uint16_t)(pLength * 8),
                    .user = (void*)1,
                    .tx_buffer = pData
                };
                gpio_set_level(pinDC, 1);
                spi_device_transmit(spi, &t);
            }
        }

    public:
        Display(gpio_num_t pPinSDA, gpio_num_t pPinSCK, gpio_num_t pPinCS, gpio_num_t pPinDC, gpio_num_t pPinRST) {
            pinSDA = pPinSDA; // MOSI
            pinSCK = pPinSCK; // CLK
            pinCS = pPinCS;
            pinDC = pPinDC;
            pinRST = pPinRST;

            frameBuffer[0] = static_cast<std::uint32_t*>(heap_caps_malloc(width * height * pixelBytes * 2, MALLOC_CAP_DMA));
            frameBuffer[1] = static_cast<std::uint32_t*>(heap_caps_malloc(width * height * pixelBytes * 2, MALLOC_CAP_DMA));
            curFrameBuffer = false;
        }

        void init() {
            // SPI init:
            spi_bus_config_t buscfg = {
                .mosi_io_num = pinSDA,
                .miso_io_num = -1,
                .sclk_io_num = pinSCK,
                .quadwp_io_num = -1,
                .quadhd_io_num = -1,
                .max_transfer_sz = maxBufferChunkSendBytes
            };
            spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

            spi_device_interface_config_t devcfg = {
                .mode = 0,
                .clock_speed_hz = spiClockSpeedHz,
                .spics_io_num = pinCS,
                .queue_size = 5
            };

            gpio_set_direction(pinDC, GPIO_MODE_OUTPUT);
            spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

            // ST7789 init:
            gpio_set_direction(pinRST, GPIO_MODE_OUTPUT);
            gpio_set_level(pinRST, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(pinRST, 1);
            vTaskDelay(pdMS_TO_TICKS(100));

            sendCmd(0x36);
            uint8_t data1[] = {0x00};
            sendData(data1, sizeof(data1));

            sendCmd(0x3A);
            uint8_t data2[] = {0x07};
            sendData(data2, sizeof(data2));

            //if(displayInversion) sendCmd(0x21);

            sendCmd(0x11);
            vTaskDelay(pdMS_TO_TICKS(120));
            sendCmd(0x29);

            setAddressWindow(0, 0, width - 1, height - 1);
        }

        void setAddressWindow(uint16_t pX1, uint16_t pY1, uint16_t pX2, uint16_t pY2) {
            uint8_t data[4];

            sendCmd(0x2A);
            data[0] = pX1 >> 8;
            data[1] = pX1 & 0xFF;
            data[2] = pX2 >> 8;
            data[3] = pX2 & 0xFF;
            sendData(data, 4);

            sendCmd(0x2B);
            data[0] = pY1 >> 8;
            data[1] = pY1 & 0xFF;
            data[2] = pY2 >> 8;
            data[3] = pY2 & 0xFF;
            sendData(data, 4);

            sendCmd(0x2C);
        }

        void switchFrameBuffers() {
            curFrameBuffer = !curFrameBuffer;
        }

        void sendActiveBuffer() {
            size_t offset = 0;
            spi_transaction_t t[5];
            int queued = 0;
            size_t total_words = width * height;
            gpio_set_level(pinDC, 1);

            while (total_words > 0)
            {
                // TODO Check if branchless is faster
                size_t chunk_words = (total_words > maxBufferChunkSendBytes / pixelBytes ? maxBufferChunkSendBytes / pixelBytes : total_words);
                //size_t chunk_words2 = (size_t[2]){(maxBufferChunkSendBytes / DISPLAY_PIXEL_SIZE), total_words}[total_words > (maxBufferChunkSendBytes / DISPLAY_PIXEL_SIZE)];

                t[queued] = {
                    .length = chunk_words * pixelBytes * 8,
                    .user = (void*)1,
                    .tx_buffer = frameBuffer[!curFrameBuffer] + offset
                };

                spi_device_queue_trans(spi, &t[queued], portMAX_DELAY);

                offset += chunk_words;
                total_words -= chunk_words;
                queued++;
            }

            for (int i = 0; i < queued; i++)
            {
                spi_transaction_t* rtrans;
                spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY);
            }
        }

        // RENDERING

        void fill(RGB565 pColor) {
            std::fill_n(frameBuffer[curFrameBuffer], width * height, SWAP16Bit(pColor.color));
        }

        void setPixel(unsigned int pIndex, RGB565 pColor) {
            frameBuffer[curFrameBuffer][pIndex] = SWAP16Bit(pColor.color);
        }

        void IRAM_ATTR drawVerticalLine(int pX, RGB565 pColor) {
            if (pX >= 0 && pX < width) for (int y = 0; y < height; y++) setPixel(pX + y * width, pColor);
        }
        void IRAM_ATTR drawHorizontalLine(int pY, RGB565 pColor) {
            if (pY >= 0 && pY < height) for (int x = 0; x < width; x++) setPixel(x + pY * width, pColor);
        }

        void drawRainbowRect() {
            for (int y = 10; y < 31; y++)
                for (int x = 10; x < 80; x++)
                    setPixel(y + x * width, RGB565(y-10, 0, 0));
            
            for (int y = 10; y < 63; y++)
                for (int x = 100; x < 180; x++)
                    setPixel(y + x * width, RGB565(0, y-10, 0));
            
            for (int y = 10; y < 31; y++)
                for (int x = 190; x < 270; x++)
                    setPixel(y + x * width, RGB565(0, 0, y-10));
        }
    };
}