#include <iostream>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <limits>

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_timer.h>

#include "engine/bit_uint.hpp"
#include "engine/Color.hpp"
#include "engine/ClockTimer.hpp"
#include "engine/IntervalTimer.hpp"
//#include "main/Display.hpp"

//void mainn(void* pArgs) {}
//xTaskCreatePinnedToCore(&mainn, "Main", 8192, nullptr, 10, nullptr, 1);

extern "C" int app_main() {
    // BUTTON
    gpio_set_direction(GPIO_NUM_7, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_NUM_7);
    bool buttonPressed = false;
    pa::IntervalTimer<std::int64_t> buttonDebounceTimer(100*1000);

    // DISPLAY
    /*pa::Display display(GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_10, GPIO_NUM_17, GPIO_NUM_18);
    display.init();
    display.fill(pa::RGB565(0, 0, 0)/ *pa::Color::Black.getRGB565()* /);
    //display.drawVerticalLine(160, pa::RGB565()/ *pa::Color::White.getRGB565()* /);
    display.drawRainbowRect();
    display.switchFrameBuffers();
    display.sendActiveBuffer();
    display.fill(pa::RGB565(0, 0, 0));*/

    pa::Color<8,8,8,8> col;
    pa::Color<8,8,8,8>::AT b = pa::Color<8,8,8,8>::AT::sMax;
    pa::Color<8,8,8,8>::AT a = col.getA().sMax;


    // LOOP
    pa::IntervalTimer<std::int64_t> idfIdleTimer(3*1000*1000);
    int64_t curFrameTime = 0;
    bool running = true;
    while (running)
    {
        curFrameTime = esp_timer_get_time();

        // Button events
        if (buttonDebounceTimer.tryUpdate(curFrameTime))
        {
            bool buttonCurPressed = !gpio_get_level(GPIO_NUM_7);
            if (buttonCurPressed != buttonPressed)
            {
                if (buttonCurPressed)
                {
                    std::cout << "PRESSED" << std::endl;
                    /*display.fill(page::RGB565(0, 0, 0));
                    display.drawHorizontalLine(70, pa::RGB565());
                    display.switchFrameBuffers();
                    display.sendActiveBuffer();*/
                }
                else
                {
                    std::cout << "RELEASED\n" << std::endl;
                    /*display.fill(pa::RGB565(0, 0, 0));
                    display.drawHorizontalLine(140, pa::RGB565());
                    display.switchFrameBuffers();
                    display.sendActiveBuffer();*/
                }
                buttonPressed = buttonCurPressed;
            }
        }
        
        // IDF FreeRTOS idling (the delay) every couple of seconds and the "yield"ing here every iteration is kinda required for background processes to take place so we don't completely block the CPU with our main loop :).
        if (idfIdleTimer.tryUpdate(curFrameTime))
        {
            std::cout << "IDLED 1" << std::endl;
            vTaskDelay(1);
        }
        else taskYIELD();
    }

    return 0;
}