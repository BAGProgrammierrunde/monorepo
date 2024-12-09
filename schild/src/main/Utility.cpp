#include "Utility.hpp"

namespace Utility {
  void printComponentNotEnabledMessage(String functionName, String defineName) {
    Serial.printf("You cannot use the method '%s' if the necessary component is not enabled in 'configuration.h'. Please enable with '#define %s true'\n", functionName.c_str(), defineName.c_str());
  }

  void printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();
    Serial.print("The ESP was ");
    switch (wakeupReason) {
      case ESP_SLEEP_WAKEUP_UNDEFINED:       Serial.println("freshly booted"); break;
      case ESP_SLEEP_WAKEUP_EXT0:            Serial.println("woken up by external signal using RTC_IO"); break;
      case ESP_SLEEP_WAKEUP_EXT1:            Serial.println("woken up by external signal using RTC_CNTL"); break;
      case ESP_SLEEP_WAKEUP_TIMER:           Serial.println("woken up by timer"); break;
      case ESP_SLEEP_WAKEUP_TOUCHPAD:        Serial.println("woken up by touchpad"); break;
      case ESP_SLEEP_WAKEUP_ULP:             Serial.println("woken up by ULP program"); break;
      case ESP_SLEEP_WAKEUP_GPIO:            Serial.println("woken up by GPIO"); break;
      case ESP_SLEEP_WAKEUP_UART:            Serial.println("woken up by UART"); break;
      case ESP_SLEEP_WAKEUP_WIFI:            Serial.println("woken up by WIFI"); break;
      case ESP_SLEEP_WAKEUP_COCPU:           Serial.println("woken up by COCPU int"); break;
      case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG: Serial.println("woken up by COCPU crash"); break;
      case ESP_SLEEP_WAKEUP_BT:              Serial.println("woken up by BT"); break;
      default:                               Serial.printf("started due to an unknown reason (code=%d)\n", wakeupReason);
    }
  }
}
