#pragma once

#include "scene.h"

#include <freertos/FreeRTOS.h>

class System {
private:
    static inline TaskHandle_t mainTaskHandle = nullptr;
    static inline TaskHandle_t gameTaskHandle = nullptr;

    static inline Scene* scene = nullptr;

    static void createGameTask();
    static void gameTask(void* pvParameters);
public:
    static void init();
    static void run(Scene* scene);
};
