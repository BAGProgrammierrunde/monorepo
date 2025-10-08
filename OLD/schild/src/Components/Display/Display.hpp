#pragma once

#include "Components/Structure/Component.hpp"

class Display : public Component<6> {
private:

public:
    Display(unsigned int _pPinBusy, unsigned int _pPinRst, unsigned int _pPinDc, unsigned int _pPinCs, unsigned int _pPinClk, unsigned int _pPinDin);
    // TODO: Ja das halt hier wirklich implementieren dann :)
};
