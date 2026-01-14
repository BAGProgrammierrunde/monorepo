#include <Arduino.h>
#include "Display.hpp"

Display::Display(unsigned int _pPinBusy, unsigned int _pPinRst, unsigned int _pPinDc, unsigned int _pPinCs, unsigned int _pPinClk, unsigned int _pPinDin)
 : Component<6>({_pPinBusy, _pPinRst, _pPinDc, _pPinCs, _pPinClk, _pPinDin}) {
    printf("E-Paper init SUCCESSFUL\r\n");
}
