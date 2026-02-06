#include <iostream>

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_timer.h>

#include "Display.hpp"

//void mainn(void* pArgs) {}
//xTaskCreatePinnedToCore(&mainn, "Main", 8192, nullptr, 10, nullptr, 1);

extern "C" int app_main() {
    // BUTTON
    gpio_set_direction(GPIO_NUM_7, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_NUM_7);
    bool buttonPressed = false;
    int64_t lastButtonPressTime = 0;
    const unsigned int buttonDebounceIntervalMillis = 100;

    // DISPLAY
    Display display(GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_10, GPIO_NUM_17, GPIO_NUM_18);
    display.init();
    display.fill(BLACK);
    display.draw_vertical_line(160, WHITE);
    display.switchFrameBuffers();
    display.send_active_buffer();
    display.fill(BLACK);

    // LOOP
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
                if (buttonCurPressed)
                {
                    std::cout << "PRESSED" << std::endl;
                    display.fill(BLACK);
                    display.draw_horizontal_line(70, WHITE);
                    display.switchFrameBuffers();
                    display.send_active_buffer();
                }
                else
                {
                    std::cout << "RELEASED\n" << std::endl;
                    display.fill(BLACK);
                    display.draw_horizontal_line(140, WHITE);
                    display.switchFrameBuffers();
                    display.send_active_buffer();
                }
                

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