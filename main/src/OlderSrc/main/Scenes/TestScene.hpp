#pragma once

#include <Arduino.h>
#include "Core/Structure/Scene.hpp"

//POTI&LED DEBUG MODE: poti auf > 0 beim start und dann hoch runter drehen
//led debug mode -> poti auf 0 beim start und dann blinken die LEDs
#include "main/Configuration.hpp"
#include "Data_Types/IntervalAction.hpp"

class TestScene : public Scene {
private:
    IntervalAction blinker1;
    IntervalAction blinker2;
    IntervalAction potiOutput;

    int analogValue = 0;
    bool POTI_DEBUG = false;
    bool ledState = HIGH;
    bool ledState2 = HIGH;

    void toggleLED1();
    void toggleLED2();
    void printPotentiometerValue();

public:
    TestScene();
    void update() override;
};