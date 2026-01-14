#pragma once

#include <Arduino.h>
#include "Core/Structure/Scene.hpp"

class MainScene : public Scene {
private:

public:
    MainScene();
    void update() override;
};