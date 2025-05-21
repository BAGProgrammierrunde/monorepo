#pragma once

#include <memory>
#include "Components/Button/Button.hpp"
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "Components/joystick/Joystick.hpp"
#include "Components/Display/Display.hpp"

namespace Features {
    void useButtonForDeepSleep(const Button& pDeepSleepButton, const Button& pWakeupButton);
    void lightupLEDs();
    void playMelody(const PassiveBuzzer& pBuzzer);
    void playNote(const PassiveBuzzer& pBuzzer);
    void displayTemperature(const Display& pDisplay, const TemperatureSensor& pTemperatureSensor);
    void testJoystick(const Joystick& pJoystick);
    void testDisplay(/*const Display& pDisplay*/);
}