#pragma once

#include "main/Configuration.hpp"
#include "Components/Button/Button.hpp"
#include "Components/Display/Display.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"
#include "Components/Potentiometer/Potentiometer.hpp"
#include "Components/Joystick/Joystick.hpp"


class Device { // Static class
public:
    // INPUT:
    static Button button1;
    static Button button2;
    static Button button3;
    static Button button4;
    static Joystick joystick;
    static Potentiometer potentiometer;

    // Passive (Sensors & stuff)
    static TemperatureSensor temperatureSensor;

    // OUTPUT:
    // TODO led1 & led2
    static PassiveBuzzer passiveBuzzer;
    static Display display;

    // METHODS:

    Device() = delete;
    static void init();
    //static void handleEvents(); // Mabye later
    static void update();
};
