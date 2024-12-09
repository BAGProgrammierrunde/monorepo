#include "Temperature.h"
#include "components/DS18B20/DS18B20.h"
#include "features/common/Common.h"

namespace Features {
  void displayTemperature(const std::shared_ptr<Display> &display, const std::shared_ptr<DS18B20> &temperatureSensor) {
#if ENABLE_TEMPERATURE
    float currentTemperature = temperatureSensor->getTemperature();
    int currentTemperatureInt = round(currentTemperature);

    if (currentTemperatureInt != temperature) {
      temperature = currentTemperatureInt;
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= MINUTE_IN_MILLIS) {
        previousMillis = currentMillis;
        display.nextPage();
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(0, 0);
        display.print(currentTemperature);
        display.print("C");

        Serial.println(currentTemperature);
      }
    }
#else
    printComponentNotEnabledMessage("displayTemperature", "ENABLE_TEMPERATURE");
#endif
  }
}
