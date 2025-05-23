#pragma once

#include <Arduino.h>

#include "Core/Structure/Scene.hpp"

class TestScene : public Scene {
private:

public:
    TestScene(String pName);
    void update() override;
};