idf_component_register(SRCS
        "main.cpp"
        "game/game_scene.cpp"
        "game/dino.cpp"
        REQUIRES esp_timer esp_driver_gpio engine
        INCLUDE_DIRS "."
)
