idf_component_register(SRCS "st7789.cpp"
        REQUIRES spi_flash esp_driver_spi esp_driver_gpio
        INCLUDE_DIRS "include")
