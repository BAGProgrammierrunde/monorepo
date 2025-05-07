#pragma once

#include "Components/Structure/Component.hpp"

class Display : public Component<6> {
private:
public:
  explicit Display(unsigned int pinBusy, unsigned int pinRst, unsigned int pinDc, unsigned int pinCs, unsigned int pinClk, unsigned int pinDin);
};
