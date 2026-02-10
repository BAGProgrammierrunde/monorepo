#include "button.h"

Button::Button(gpio_num_t pin) : pin(pin) {
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pullup_en(pin);
    // Initialize with current stable state
    lastStableState = (gpio_get_level(pin) == Button::LOW);
}

bool Button::isPressed() {
    return gpio_get_level(pin) == Button::LOW;
    /* TODO fix debounce code - currently not working
    bool currentReading = (gpio_get_level(pin) == Button::LOW);
    uint64_t currentTime = esp_timer_get_time();

    // If the reading has changed, reset the debounce timer
    if (currentReading != lastStableState) {
        lastDebounceTime = currentTime;
    }

    // Only update the stable state if enough time has passed (stable reading)
    if ((currentTime - lastDebounceTime) > DEBOUNCE_TIME_US) {
        lastStableState = currentReading;
    }

    return lastStableState;
    */
}
