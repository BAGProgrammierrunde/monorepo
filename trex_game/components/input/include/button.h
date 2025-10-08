#pragma once

#include <driver/gpio.h>
#include <esp_log.h>

class Button {
  private:
    static constexpr int LOW = 0;
    gpio_num_t pin;

  public:
    Button(gpio_num_t pin);

    bool isPressed();
};
