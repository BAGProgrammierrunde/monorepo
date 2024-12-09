#include <Arduino.h>

#include "Display.h"

Display::Display(unsigned int pinBusy, unsigned int pinRst, unsigned int pinDc, unsigned int pinCs, unsigned int pinClk, unsigned int pinDin) : Component(pinBusy, pinRst, pinDc, pinCs, pinClk, pinDin), epd(GxEPD2_290_GDEY029T94(pinCs, pinDc, pinRst, pinBusy)) {

}

void Display::render() {
  Serial.println("Display::render() called");
}
