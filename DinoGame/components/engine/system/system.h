#pragma once

#include "game/game.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hal/wdt_hal.h>

class System {
private:
    static TaskHandle_t mainTaskHandle;
    static TaskHandle_t gameTaskHandle;

    Game* game = nullptr;

    void createGameTask();
    static void gameTask(void* pvParameters);
public:
    void init(Game &pGame);
    void run();

};
