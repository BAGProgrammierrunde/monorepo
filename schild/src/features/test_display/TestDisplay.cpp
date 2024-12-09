#include <Fonts/FreeMonoBold9pt7b.h>

#include "TestDisplay.h"

namespace Features {
// TODO Change button to display
  void testDisplay(const std::shared_ptr<Button> &button) {
#if ENABLE_DISPLAY
    Serial.println("GxEPD2 2.9-inch  e-ink display test start1");
    display->init(115200, true, 2, true); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    Serial.println("Display initialized");

    display->setRotation(1);
    display->setTextColor(GxEPD_BLACK);
    display->setFullWindow();
    display->firstPage();
    Serial.println("Starting display update");
    do {
      display->fillScreen(GxEPD_WHITE);
      display->setCursor(0, 0);
      display->setTextSize(2);
      display->print(random(10000000));
    } while (display->nextPage());
    Serial.println("Display update done");
#else
    printComponentNotEnabledMessage("testDisplay", "ENABLE_DISPLAY");
#endif
  }
}
