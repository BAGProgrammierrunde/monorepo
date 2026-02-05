#include <iostream>

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <esp_timer.h>

//void mainn(void* pArgs) {}
//xTaskCreatePinnedToCore(&mainn, "Main", 8192, nullptr, 10, nullptr, 1);

extern "C" int app_main() {
    gpio_set_direction(GPIO_NUM_7, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_NUM_7);
    bool buttonPressed = false;
    int64_t lastButtonPressTime = 0;
    const unsigned int buttonDebounceIntervalMillis = 100;

    int64_t lastIdfIdleTime = 0;
    const unsigned int idfIdleIntervalMillis = 3000;

    int64_t curFrameTime = 0;
    bool running = true;
    while (running)
    {
        curFrameTime = esp_timer_get_time();

        // Button events
        if (curFrameTime - lastButtonPressTime >= buttonDebounceIntervalMillis)
        {
            bool buttonCurPressed = !gpio_get_level(GPIO_NUM_7);
            if (buttonCurPressed != buttonPressed)
            {
                std::cout << (buttonCurPressed ? "PRESSED" : "RELEASED\n") << std::endl;
                buttonPressed = buttonCurPressed;
            }
            lastButtonPressTime = curFrameTime;
        }
        
        // IDF FreeRTOS idling (the delay) every couple of seconds and the "yield"ing here every iteration is kinda required for background processes to take place so we don't completely block the CPU with our main loop :).
        if (curFrameTime - lastIdfIdleTime < idfIdleIntervalMillis * 1000) taskYIELD();
        else
        {
            //std::cout << "IDLED 1" << std::endl;
            lastIdfIdleTime = curFrameTime;
            vTaskDelay(1);
        }
    }

    return 0;
}