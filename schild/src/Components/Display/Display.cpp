#include <Arduino.h>
#include "Display.hpp"

Display::Display(
    unsigned int pinBusy,
    unsigned int pinRst,
    unsigned int pinDc,
    unsigned int pinCs,
    unsigned int pinClk,
    unsigned int pinDin) : Component({pinBusy, pinRst, pinDc, pinCs, pinClk, pinDin})
{
    printf("e-Paper init SUCCESSFUL\r\n");
}
