#pragma once

#include <memory>

#include <components/Button/Button.h>
#include <components/Display/Display.h>
#include <components/DS18B20/DS18B20.h>
#include <components/PassiveBuzzer/PassiveBuzzer.h>
#include <components/Potentiometer/Potentiometer.h>
#include <components/Joystick/Joystick.h>

class Device {
private:
    static std::shared_ptr<Device> instance;

    static void printEnabledMessage(const String &name, const bool &enableDefine);
public:
    std::shared_ptr<Display> display;

    // TODO led1
    // TODO led2

    // TODO temperature

    std::shared_ptr<DS18B20> temperatureSensor;

    std::shared_ptr<Joystick> joystick;

    std::shared_ptr<Potentiometer> potentiometer;

    std::shared_ptr<PassiveBuzzer> passiveBuzzer;

    std::shared_ptr<Button> button1;
    std::shared_ptr<Button> button2;
    std::shared_ptr<Button> button3;
    std::shared_ptr<Button> button4;

    explicit Device();

    static std::shared_ptr<Device> getInstance();

    /**
     * Singletons should not be cloneable.
     */
    Device(Device &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const Device &) = delete;
};
