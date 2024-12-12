#include "TestDisplay.hpp"

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "main/Configuration.hpp"
#include "main/Utility.hpp"

namespace Features {
  void testDisplay(const std::shared_ptr<Display> &display) {
#if ENABLE_DISPLAY
    Serial.println("GxEPD2 2.9-inch  e-ink display");
    display->getEpdRef()->init(115200, true, 2, true); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    Serial.println("Display initialized");

    display->getEpdRef()->setRotation(1);
    display->getEpdRef()->setTextColor(GxEPD_BLACK);
    display->getEpdRef()->setFullWindow();
    display->getEpdRef()->firstPage();
    Serial.println("Starting display update");
    do {
      display->getEpdRef()->fillScreen(GxEPD_WHITE);
      display->getEpdRef()->setCursor(0, 0);
      display->getEpdRef()->setTextSize(2);
      display->getEpdRef()->print(random(10000000));
    } while (display->getEpdRef()->nextPage());
    Serial.println("Display update done");
#else
    Utility::printComponentNotEnabledMessage("testDisplay", "ENABLE_DISPLAY");
#endif
  }
}
