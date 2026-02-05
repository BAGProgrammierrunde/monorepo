#pragma once

class LED : public Component<1> {
private:

public:
    LED(gpio_num_t pPin) : Component(pPin) {
        gpio_set_direction(pins[0], GPIO_MODE_OUTPUT);
        gpio_set_level(pins[0], 0);
    }

    void setState(bool pState) {
        gpio_set_level(pins[0], pState);
    }
};
