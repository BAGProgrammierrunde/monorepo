#pragma once

#include <Arduino.h>

#include "Components/Structure/Component.hpp"
#include "data/any_callable.hpp"

class Button : public Component<1> {
public:
  static const ulong DEBOUNCE_TIME = 250;

private:
  bool pressed;
  me::any_callable<void ()> callbackFunc;
  ulong fallingTime;
  ulong lastFallingTime;

public:
  explicit Button(unsigned int pin);
  bool isPressed();
  void setCallback(me::any_callable<void ()> pCallbackFunction);
  void clearCallback();
  void callCallback();
};
