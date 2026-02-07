idf_component_register(
        SRCS
        "display/driver.cpp"
        "display/st7789.cpp"
        "display/display.cpp"
        "gal/gal.cpp"
        "input/button.cpp"
        "core/scene.cpp"
        "core/system.cpp"
        REQUIRES esp_driver_spi esp_driver_gpio esp_timer
        INCLUDE_DIRS "."
)
