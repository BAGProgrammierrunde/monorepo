#include "Temperature.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "main/Utility.hpp"

namespace Features {
  void displayTemperature(const std::shared_ptr<Display> &display, const std::shared_ptr<TemperatureSensor> &temperatureSensor) {
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
    Utility::printComponentNotEnabledMessage("displayTemperature", "ENABLE_TEMPERATURE");
#endif
  }
}
