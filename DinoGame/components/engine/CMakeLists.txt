idf_component_register(
        SRCS
        "display/st7789.cpp"
        "gal/gal.cpp"
        "input/button.cpp"
        REQUIRES spi_flash esp_driver_spi esp_driver_gpio
        INCLUDE_DIRS "."
)
