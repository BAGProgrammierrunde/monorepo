idf_component_register(SRCS
        "main.cpp"
        "game/dino.cpp"
        "game/game_scene.cpp"
        REQUIRES esp_timer esp_driver_gpio engine esp_wifi esp_event esp_netif nvs_flash
        INCLUDE_DIRS "."
)
