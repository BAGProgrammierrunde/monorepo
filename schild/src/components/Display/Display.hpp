#pragma once

#include <GxEPD2_BW.h>
#include "Components/Structure/Component.hpp"

class Display : public Component<6> {
private:
  GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> epd;
public:
  explicit Display(unsigned int pinBusy, unsigned int pinRst, unsigned int pinDc, unsigned int pinCs, unsigned int pinClk, unsigned int pinDin);
  void render();
};
