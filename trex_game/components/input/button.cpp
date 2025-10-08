#include "button.h"

Button::Button(gpio_num_t pin) : pin(pin) {
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pullup_en(pin);
}

bool Button::isPressed() {
    // TODO Debounce implementieren
    return gpio_get_level(pin) == Button::LOW;
}
