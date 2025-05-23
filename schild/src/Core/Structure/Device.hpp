#pragma once

#include "main/Configuration.hpp"
#include "Components/LED/LED.hpp"
#include "Components/Button/Button.hpp"
#include "Components/Display/Display.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"
#include "Components/Potentiometer/Potentiometer.hpp"
#include "Components/Joystick/Joystick.hpp"
#include "Scene.hpp"


class Device { // Singleton
public:
    // Input
    Button button1;
    Button button2;
    Button button3;
    Button button4;
    Joystick joystick;
    Potentiometer potentiometer;

    // Passive (Sensors & stuff)
    TemperatureSensor temperatureSensor;

    // Output
    LED led1;
    LED led2;
    PassiveBuzzer passiveBuzzer;
    Display display;

    me::poly_value<Scene> scene;


private:
    Device();

public:
    // Singleton setup biolerplate, don't worry about this
    Device(const Device& pOther) = delete; // Copy constructor
    Device(Device&& pOther) noexcept = delete; // Move constructor
    void operator=(const Device& pOther) = delete; // Assignment operator
    void operator=(Device&& pOther) noexcept = delete; // Move-assignment operator
    static Device& getInstance();

    void update();
};
