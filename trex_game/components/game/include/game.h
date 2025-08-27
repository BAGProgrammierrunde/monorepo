#pragma once
#include "st7789.h"

class Game {
private:
  ST7789 &display;
public:
  Game(ST7789 &display);
  void loop();
};
