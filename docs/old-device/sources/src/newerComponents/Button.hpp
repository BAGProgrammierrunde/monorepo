#pragma once

class Button : public Component<1> {
private:

public:
    Button(gpio_num_t pPin) : Component(pPin) {
        gpio_set_direction(pins[0], GPIO_MODE_INPUT);
        gpio_pullup_en(pins[0]);
    }

    bool isPressed() const {
        return (gpio_get_level(pins[0]) == 0);
    }
};
