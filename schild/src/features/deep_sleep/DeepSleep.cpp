#include "DeepSleep.hpp"

#include <driver/rtc_io.h>
#include "main/Configuration.hpp"

namespace Features {
    void useButtonForDeepSleep(const std::shared_ptr<Button> &deepSleepButton, const std::shared_ptr<Button> &wakeupButton) {
        deepSleepButton->setCallback([](Button* pThisRef) -> void {
            Serial.println("Button 1 clicked");
            esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(pThisRef->getPin(0)), ESP_EXT1_WAKEUP_ANY_HIGH);
            Serial.print("Sleep countdown triggered - sleeping in 2 seconds");
            for (int i = 0; i < 50; i++) {
                Serial.print(".");
                delay(20);
            }
            Serial.println();
            Serial.println("Sleeping now");
            Serial.flush();
            esp_deep_sleep_start();
        });
    }
}
