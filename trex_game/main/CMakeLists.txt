idf_component_register(SRCS "trex_game.cpp" "main.cpp"
        REQUIRES input gal game esp_timer esp_driver_gpio
        INCLUDE_DIRS "."
)
