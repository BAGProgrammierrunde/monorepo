#include "DeepSleep.h"
#include "features/common/Common.h"
#include "main/configuration.h"

namespace features {
  void useButtonForDeepSleep(const std::shared_ptr<Button> &deepSleepButton, const std::shared_ptr<Button> &wakeupButton) {
    ulong wakeupPin = wakeupButton->testgetPin(0);
    if (wakeupPin != PIN_BUTTON_4) {
      // TODO Remove this code after captures in button callback are fixed or other solution is found
      Serial.println("The wake-up function currently only works with button 4. Please call 'useButtonForDeepSleep(button1-3, button4)'");
      return;
    }
    deepSleepButton->setCallback([](ulong) -> void {
          Serial.println("Button 1 clicked");
          esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(PIN_BUTTON_4), ESP_EXT1_WAKEUP_ANY_HIGH);
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
