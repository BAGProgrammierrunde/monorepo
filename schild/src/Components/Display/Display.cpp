#include <Arduino.h>
#include "Display.hpp"

#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>

Display::Display(unsigned int pinBusy, unsigned int pinRst, unsigned int pinDc, unsigned int pinCs, unsigned int pinClk, unsigned int pinDin) : Component(pinBusy, pinRst, pinDc, pinCs, pinClk, pinDin), epd(GxEPD2_290_GDEY029T94(pinCs, pinDc, pinRst, pinBusy)) {
    printf("e-Paper init SUCCESSFUL\r\n");
}

GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> * Display::getEpdRef() {
  return &epd;
}
