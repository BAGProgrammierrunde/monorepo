#pragma once

#include "any_callable.h"
#include "poly_value.h"
#include "scene.h"

#include <freertos/FreeRTOS.h>

class System {
  private:
    // The width and height should remain in the system until the complete device code has been separated
    static constexpr unsigned int displayWidth  = 240;
    static constexpr unsigned int displayHeight = 320;

    static inline TaskHandle_t mainTaskHandle = nullptr;
    static inline TaskHandle_t gameTaskHandle = nullptr;

    static inline PolyValue<Scene> scene;
    static inline AnyCallable<void()> delayedSceneSwitchFunc;

    static void createGameTask();
    static void gameTask(void* pvParameters);

  public:
    static void init();
    static void start();

    template <typename SceneT, typename... Params> static void start(Params... pArgs) {
        setScene<SceneT>(pArgs...);
        start();
    }

    template <typename SceneT, typename... Params> static void setScene(Params... pConstructorArgs) {
        delayedSceneSwitchFunc = [pConstructorArgs...]() -> void {
            scene.clear(); // To ensure current scene is destroyed (destructor-call) before new scene is created (constructor-call below)
            scene.setInPlace(new SceneT(pConstructorArgs...)); // It's okay that uncaptured but static scene obj is used
            scene->start();
        };
    }
};
