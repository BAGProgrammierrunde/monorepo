#include "TestDisplay.hpp"

#include <Arduino.h>

#include "main/Configuration.hpp"
#include "main/Utility.hpp"

#include <DEV_Config.h>
#include <GUI_Paint.h>
#include <EPD.h>
#include <utility/EPD_2in9_V2.h>

namespace Features {
  void testDisplay(const std::shared_ptr<Display> &display) {
#if ENABLE_DISPLAY
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
      for (int y = 1; y < 128; ++y) {
          for (int x = 0; x < 296; x=x+9) {
              Paint_DrawLine(x, y, x+8, y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
              EPD_2IN9_V2_Display_Partial(BlackImage);
              DEV_Delay_ms(20);
          }
      }
      int x = 296;
      for (int y = 1; y < 128; ++y) {
          Paint_DrawPoint(x, y, BLACK, DOT_PIXEL_1X1, DOT_FILL_AROUND);
          EPD_2IN9_V2_Display_Partial(BlackImage);
          if (x == 296) {
              ++x;
          } else {
              --x;
          }
      }
#else
    Utility::printComponentNotEnabledMessage("testDisplay", "ENABLE_DISPLAY");
#endif
  }
}
