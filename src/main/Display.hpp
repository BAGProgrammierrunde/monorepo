#define SWAP16(x) (((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define WHITE SWAP16(0xFFFF)
#define BLACK SWAP16(0x0000)
#define RED SWAP16(0xF800)

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

    uint16_t* frameBuffer[2];
    bool curFrameBuffer; // Currently rendered to, other is currently sent to display

public:
    Display(gpio_num_t pPinSDA, gpio_num_t pPinSCK, gpio_num_t pPinCS, gpio_num_t pPinDC, gpio_num_t pPinRST) {
        pinSDA = pPinSDA; // MOSI
        pinSCK = pPinSCK; // CLK
        pinCS = pPinCS;
        pinDC = pPinDC;
        pinRST = pPinRST;

        frameBuffer[0] = static_cast<uint16_t*>(heap_caps_malloc(width * height * pixelBytes, MALLOC_CAP_DMA));
        frameBuffer[1] = static_cast<uint16_t*>(heap_caps_malloc(width * height * pixelBytes, MALLOC_CAP_DMA));
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
            .mode           = 0,
            .clock_speed_hz = spiClockSpeedHz,
            .spics_io_num   = pinCS,
            .queue_size     = 5,
        };

        gpio_set_direction(pinDC, GPIO_MODE_OUTPUT);
        ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));

        // ST7789 init:
        gpio_set_direction(pinRST, GPIO_MODE_OUTPUT);
        gpio_set_level(pinRST, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(pinRST, 1);
        vTaskDelay(pdMS_TO_TICKS(100));

        st7789_send_cmd(0x36);
        uint8_t data1[] = { 0x00 };
        st7789_send_data(data1, sizeof(data1));

        st7789_send_cmd(0x3A);
        uint8_t data2[] = { 0x05 };
        st7789_send_data(data2, sizeof(data2));

        //if(displayInversion) st7789_send_cmd(0x21);

        st7789_send_cmd(0x11);
        vTaskDelay(pdMS_TO_TICKS(120));
        st7789_send_cmd(0x29);

        set_address_window(0, 0, width - 1, height - 1);
    }

    void st7789_send_cmd(uint8_t cmd) {
        spi_transaction_t t = { .length = 8, .user = (void*)0, .tx_buffer = &cmd };
        gpio_set_level(pinDC, 0);
        spi_device_transmit(spi, &t);
    }

    void st7789_send_data(const void* data, int len) {
        if (len == 0) return;

        spi_transaction_t t = { .length = (uint16_t)(len * 8), .user = (void*)1, .tx_buffer = data };
        gpio_set_level(pinDC, 1);
        ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
    }

    void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        uint8_t data[4];

        st7789_send_cmd(0x2A);
        data[0] = x0 >> 8;
        data[1] = x0 & 0xFF;
        data[2] = x1 >> 8;
        data[3] = x1 & 0xFF;
        st7789_send_data(data, 4);

        st7789_send_cmd(0x2B);
        data[0] = y0 >> 8;
        data[1] = y0 & 0xFF;
        data[2] = y1 >> 8;
        data[3] = y1 & 0xFF;
        st7789_send_data(data, 4);

        st7789_send_cmd(0x2C);
    }

    void switchFrameBuffers() {
        curFrameBuffer = !curFrameBuffer;
    }

    void send_active_buffer() {
        size_t offset = 0;
        spi_transaction_t t[5];
        int queued         = 0;
        size_t total_words = width * height;
        gpio_set_level(pinDC, 1);
        while (total_words > 0) {
            // TODO Check if branchless is faster
            size_t chunk_words = total_words > (maxBufferChunkSendBytes / pixelBytes) ? (maxBufferChunkSendBytes / pixelBytes) : total_words;

            // size_t chunk_words2 = (size_t[2]){(maxBufferChunkSendBytes / DISPLAY_PIXEL_SIZE), total_words}[total_words > (maxBufferChunkSendBytes /
            // DISPLAY_PIXEL_SIZE)];

            t[queued] = {
                .length    = chunk_words * pixelBytes * 8,
                .user      = (void*)1,
                .tx_buffer = frameBuffer[!curFrameBuffer] + offset,
            };

            ESP_ERROR_CHECK(spi_device_queue_trans(spi, &t[queued], portMAX_DELAY));

            offset += chunk_words;
            total_words -= chunk_words;
            queued++;
        }
        for (int i = 0; i < queued; ++i) {
            spi_transaction_t* rtrans;
            ESP_ERROR_CHECK(spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY));
        }
    }

    // RENDERING

    void fill(uint16_t color) {
        const size_t n = width * height;
        std::fill_n(frameBuffer[curFrameBuffer], n, color);
    }

    void setPixel(int index, uint16_t color) {
        frameBuffer[curFrameBuffer][index] = color;
    }

    void IRAM_ATTR draw_vertical_line(int x, uint16_t color) {
        constexpr int screenW = width;
        constexpr int screenH = height;
        if (x < 0 || x >= screenW) {
            return;
        }
        int index  = x;
        for (int y = 0; y < screenH; ++y) {
            setPixel(index, color);
            index += screenW;
        }
    }

    void IRAM_ATTR draw_horizontal_line(int y, uint16_t color) {
        constexpr int screenW = width;
        constexpr int screenH = height;
        if (y < 0 || y >= screenH) {
            return;
        }
        int index  = y * screenW;
        for (int x = 0; x < screenW; ++x) {
            setPixel(index + x, color);
        }
    }
};