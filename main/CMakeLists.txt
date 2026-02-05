idf_component_register(SRCS
        "game/dino.cpp"
        "game/game_scene.cpp"
        "main.cpp"
        REQUIRES esp_timer esp_driver_gpio engine
        INCLUDE_DIRS "."
)
