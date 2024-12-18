#include "TestDisplay.hpp"

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "main/Configuration.hpp"
#include "main/Utility.hpp"

#include <DEV_Config.h>
#include <GUI_Paint.h>
#include <EPD.h>
#include <utility/EPD_2in9_V2.h>

namespace Features {
  void testDisplay(const std::shared_ptr<Display> &display) {
#if ENABLE_DISPLAY
#if ENABLE_DISPLAY_GX_EPD2
display->getEpdRef()->init(115200, true, 2, true); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    Serial.println("Display initialized");
    display->getEpdRef()->setRotation(1);
    display->getEpdRef()->setTextColor(GxEPD_BLACK);
    display->getEpdRef()->setFullWindow();
    display->getEpdRef()->firstPage();
    Serial.println("Starting display update");
    do {
      display->getEpdRef()->fillScreen(GxEPD_WHITE);
        // left line
        display->getEpdRef()->fillRect(0, 0, 1, GxEPD2_290_GDEY029T94::WIDTH, GxEPD_BLACK);
        // bottom line
        display->getEpdRef()->fillRect(0, GxEPD2_290_GDEY029T94::WIDTH - 1, GxEPD2_290_GDEY029T94::HEIGHT, 1, GxEPD_BLACK);
        // right line
        display->getEpdRef()->fillRect(GxEPD2_290_GDEY029T94::HEIGHT - 1, 0, 1, GxEPD2_290_GDEY029T94::WIDTH, GxEPD_BLACK);
        // top line
        display->getEpdRef()->fillRect(0, 0, GxEPD2_290_GDEY029T94::HEIGHT, 1, GxEPD_BLACK);

        display->getEpdRef()->fillRect(0, GxEPD2_290_GDEY029T94::WIDTH - 20, GxEPD2_290_GDEY029T94::HEIGHT, 1, GxEPD_BLACK);
        display->getEpdRef()->setTextSize(2);
        const int edge = 2;
        const int edges = edge * 2;
        const int column = (GxEPD2_290_GDEY029T94::HEIGHT - edges) / 4;
        display->getEpdRef()->setCursor(edge + 24, GxEPD2_290_GDEY029T94::WIDTH - 17);
        display->getEpdRef()->print("Up");
        display->getEpdRef()->fillRect(edge + column, GxEPD2_290_GDEY029T94::WIDTH - 19, 1, 18, GxEPD_BLACK);
        display->getEpdRef()->setCursor(edge + column + 14, GxEPD2_290_GDEY029T94::WIDTH - 17);
        display->getEpdRef()->print("Down");
        display->getEpdRef()->fillRect(edge + column * 2, GxEPD2_290_GDEY029T94::WIDTH - 19, 1, 18, GxEPD_BLACK);
        display->getEpdRef()->setCursor(edge + column * 2 + 2, GxEPD2_290_GDEY029T94::WIDTH - 17);
        display->getEpdRef()->print("Select");
        display->getEpdRef()->fillRect(edge + column * 3, GxEPD2_290_GDEY029T94::WIDTH - 19, 1, 18, GxEPD_BLACK);
        display->getEpdRef()->setCursor(edge + column * 3 + 8, GxEPD2_290_GDEY029T94::WIDTH - 17);
        display->getEpdRef()->print("Sleep");
    } while (display->getEpdRef()->nextPage());

      display->getEpdRef()->display(true);
      display->getEpdRef()->refresh(true);

      display->getEpdRef()->setPartialWindow(20, 20, 120, 30);
      do {
          display->getEpdRef()->setCursor(20, 20);
          display->getEpdRef()->print("Hallo");
      } while (display->getEpdRef()->nextPage());
      delay(800);
      display->getEpdRef()->setPartialWindow(20, 20, 200, 30);
      do {
          display->getEpdRef()->fillRect(20, 20, 120, 30, GxEPD_WHITE);
          display->getEpdRef()->setCursor(80, 20);
          display->getEpdRef()->print("BAG");
      } while (display->getEpdRef()->nextPage());
      delay(800);
      display->getEpdRef()->setPartialWindow(20, 20, 200, 70);
      do {
          display->getEpdRef()->fillRect(80, 20, 100, 30, GxEPD_WHITE);
          display->getEpdRef()->setCursor(20, 50);
          display->getEpdRef()->print("Programmierrunde");
      } while (display->getEpdRef()->nextPage());
      delay(800);
      */
    Serial.println("Display update done");
#else
      static const uint16_t WIDTH = 128;
      static const uint16_t WIDTH_VISIBLE = WIDTH;
      static const uint16_t HEIGHT = 296;

      UBYTE *BlackImage;

      Serial.println("GxEPD2 2.9-inch  e-ink display");

      // Initialize display
      DEV_Module_Init();
      EPD_2IN9_V2_Init();
      // EPD_2IN9_V2_Clear();

      DEV_Delay_ms(500);

      UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0)? (EPD_2IN9_V2_WIDTH / 8 ): (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
      if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
          printf("Failed to apply for black memory...\r\n");
          while(1);
      }
      printf("Paint_NewImage\r\n");
      Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 90, WHITE);
      Paint_SelectImage(BlackImage);
      Paint_Clear(WHITE);
      EPD_2IN9_V2_Display(BlackImage);
      for (int y = 0; y < 128; ++y) {
          for (int x = 0; x < 296; x=x+9) {
              Paint_DrawLine(x, y, x+8, y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
              EPD_2IN9_V2_Display_Partial(BlackImage);
              // DEV_Delay_ms(500);
          }
      }
      // EPD_2IN9_V2_Display(BlackImage);
#endif
#else
    Utility::printComponentNotEnabledMessage("testDisplay", "ENABLE_DISPLAY");
#endif
  }
}
