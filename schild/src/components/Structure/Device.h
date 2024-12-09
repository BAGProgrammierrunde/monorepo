#pragma once

#include <memory>
#include <components/button/Button.h>
#include <components/Joystick/Joystick.h>

class Device {
private:
    static std::shared_ptr<Device> instance;
public:
    std::shared_ptr<Button> button1;
    std::shared_ptr<Button> button2;
    std::shared_ptr<Button> button3;
    std::shared_ptr<Button> button4;

    std::shared_ptr<Joystick> joystick;

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
