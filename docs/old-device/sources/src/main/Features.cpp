#include "Features.hpp"

#include <Arduino.h>
#include <driver/rtc_io.h>
#include <DEV_Config.h>
#include <GUI_Paint.h>
#include <EPD.h>
#include <utility/EPD_2in9_V2.h>

#include "Components/PassiveBuzzer/predefinedNoteTones.hpp"
#include "Data_Types/any_callable.hpp"
#include "main/Configuration.hpp"
#include "Core/Utility.hpp"

namespace Features {
    //////////////////////////////////////////////////////////////// DeepSleep
    void useButtonForDeepSleep(Button* pDeepSleepButtonRef, Button* pWakeupButtonRef) {
        pDeepSleepButtonRef->setClickFunction([pWakeupButtonRef]() -> void {
            Serial.println("Button 1 clicked");
            esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(pWakeupButtonRef->getPin(0)), ESP_EXT1_WAKEUP_ANY_HIGH);
            Serial.print("Sleep countdown triggered - sleeping in 2 seconds");
            for (int i = 0; i < 50; i++)
            {
                Serial.print(".");
                delay(20);
            }
            Serial.println();
            Serial.println("Sleeping now");
            Serial.flush();
            esp_deep_sleep_start();
        });
    }

    //////////////////////////////////////////////////////////////// Light LEDs
    void lightupLEDs() {
        // TODO Split initialization, setup and feature implementation
        pinMode(PIN_LED_1, OUTPUT);
        digitalWrite(PIN_LED_1, HIGH);
        
        pinMode(PIN_LED_2, OUTPUT);
        digitalWrite(PIN_LED_2, HIGH);
    }

    //////////////////////////////////////////////////////////////// Melody
    void playMelody(PassiveBuzzer* pBuzzerRef) {
        unsigned int npm = 200; // NotePauseMillis
        pBuzzerRef->playMelody(Melody({
            {Note(NOTE_E5, npm), 80 }, {Note(NOTE_E5, npm), 80}, {Note(NOTE_E5, npm), 360}, {Note(NOTE_E5, npm), 0 },
            {Note(NOTE_E5, npm), 80 }, {Note(NOTE_E5, npm), 80}, {Note(NOTE_E5, npm), 360}, {Note(NOTE_E5, npm), 0 },
            {Note(NOTE_E5, npm), 80 }, {Note(NOTE_G5, npm), 80}, {Note(NOTE_C5, npm), 80 }, {Note(NOTE_D5, npm), 80},
            {Note(NOTE_E5, npm), 760}, {Note(NOTE_E5, npm), 0 }, {Note(NOTE_E5, npm), 0  }, {Note(NOTE_E5, npm), 0 },
            {Note(NOTE_F5, npm), 80 }, {Note(NOTE_F5, npm), 80}, {Note(NOTE_F5, npm), 80 }, {Note(NOTE_F5, npm), 80},
            {Note(NOTE_F5, npm), 80 }, {Note(NOTE_E5, npm), 80}, {Note(NOTE_E5, npm), 80 }, {Note(NOTE_E5, npm), 80},
            {Note(NOTE_E5, npm), 80 }, {Note(NOTE_D5, npm), 80}, {Note(NOTE_D5, npm), 80 }, {Note(NOTE_E5, npm), 80},
            {Note(NOTE_D5, npm), 360}, {Note(NOTE_G5, npm), 0 }, {Note(NOTE_G5, npm), 0  }, {Note(NOTE_G5, npm), 0 }
        }));
        delay(1000);
    }

    void playNote(PassiveBuzzer* pBuzzerRef) {
        pBuzzerRef->playNote(Note(NOTE_F3, 160));
        delay(1000);
        pBuzzerRef->playNote(Note(NOTE_C5));
        delay(500);
        pBuzzerRef->stopPlaying();
    }

    //////////////////////////////////////////////////////////////// Display temperature
    void displayTemperature(Display* pDisplayRef, TemperatureSensor* pTemperatureSensorRef) {
        /*float currentTemperature = pTemperatureSensorRef->getTemperature();
        int currentTemperatureInt = round(currentTemperature);

        if (currentTemperatureInt != currentTemperature)
        {
            currentTemperature = currentTemperatureInt;
            unsigned long currentMillis = millis();

            if (currentMillis - previousMillis >= MINUTE_IN_MILLIS)
            {
                previousMillis = currentMillis;
                pDisplayRef->nextPage();
                pDisplayRef->fillScreen(GxEPD_WHITE);
                pDisplayRef->setCursor(0, 0);
                pDisplayRef->print(currentTemperature);
                pDisplayRef->print("C");

                Serial.println(currentTemperature);
            }
        }*/
    }

    //////////////////////////////////////////////////////////////// Joystick

    void testJoystick(Joystick* pJoystickRef) {
        Serial.print("Y is: ");
        Serial.print(pJoystickRef->getY());

        Serial.print(" and X is: ");
        Serial.print(pJoystickRef->getX());

        Serial.print(" and Button is: ");
        Serial.println(pJoystickRef->getButtonRef()->isPressed() ? "ON" : "OFF");
    }

    //////////////////////////////////////////////////////////////// Display
    void testDisplay(/*Display* pDisplayRef*/) {
        static const uint16_t WIDTH = 128;
        static const uint16_t WIDTH_VISIBLE = WIDTH;
        static const uint16_t HEIGHT = 296;

        UBYTE* BlackImage;

        Serial.println("GxEPD2 2.9-inch  e-ink display");

        // Initialize display
        DEV_Module_Init();
        EPD_2IN9_V2_Init();
        // EPD_2IN9_V2_Clear();

        DEV_Delay_ms(500);

        UWORD Imagesize = (EPD_2IN9_V2_WIDTH % 8 == 0 ? EPD_2IN9_V2_WIDTH / 8 : EPD_2IN9_V2_WIDTH / 8 + 1) * EPD_2IN9_V2_HEIGHT;
        if((BlackImage = (UBYTE*)malloc(Imagesize)) == NULL)
        {
            printf("Failed to apply for black memory...\r\n");
            while(1); // ????? wtf
        }
        printf("Paint_NewImage\r\n");
        Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
        Paint_SelectImage(BlackImage);
        Paint_Clear(WHITE);
        EPD_2IN9_V2_Display(BlackImage);
        for (int y = 1; y < 128; ++y)
        {
            for (int x = 0; x < 296; x=x+9)
            {
                Paint_DrawLine(x, y, x+8, y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
                EPD_2IN9_V2_Display_Partial(BlackImage);
                DEV_Delay_ms(20);
            }
        }
        int x = 296;
        for (int y = 1; y < 128; ++y)
        {
            Paint_DrawPoint(x, y, BLACK, DOT_PIXEL_1X1, DOT_FILL_AROUND);
            EPD_2IN9_V2_Display_Partial(BlackImage);
            (x == 296 ? x++ : x--);
        }
    }
}