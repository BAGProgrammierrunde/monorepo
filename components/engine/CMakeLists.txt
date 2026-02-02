idf_component_register(
        SRCS
        "display/st7789.cpp"
        "gal/gal.cpp"
        "input/button.cpp"
        "core/scene.cpp"
        "core/system.cpp"
        REQUIRES spi_flash esp_driver_spi esp_driver_gpio esp_timer
        INCLUDE_DIRS "."
)
