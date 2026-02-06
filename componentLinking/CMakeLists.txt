# Sadly required by ESP-IDF to have a second CMakeLists... ;(
# Have to add esp idf related components we want to use to "REQUIRES" list here and only then can include
idf_component_register(SRCS "../src/main/main.cpp" INCLUDE_DIRS "../src" REQUIRES esp_driver_gpio esp_driver_spi esp_timer)