#pragma once

#include "display/display.h"
#include "input/button.h"

class Device {
    Display m_Display;
    Button m_ButtonAPressed;
    Button m_ButtonBPressed;

  public:
    Device();
    Display& getDisplay();
    bool isButtonAPressed();
    bool isButtonBPressed();
};
