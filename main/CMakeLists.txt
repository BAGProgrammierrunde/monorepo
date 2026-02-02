idf_component_register(SRCS
        "main.cpp"
        "game/dino.cpp"
        "game/game_scene.cpp"
        REQUIRES esp_timer esp_driver_gpio engine
        INCLUDE_DIRS "."
)
