#pragma once

#include <driver/gpio.h>

#include <esp_log.h>

class Button {
  private:
    static constexpr int LOW = 0;
    static constexpr uint64_t DEBOUNCE_TIME_US = 50000; // 50ms debounce time

    gpio_num_t pin;
    bool lastStableState = false;
    uint64_t lastDebounceTime = 0;

  public:
    Button(gpio_num_t pin);

    bool isPressed();
};
