#include "Device.h"

#include <main/configuration.h>

std::shared_ptr<Device> Device::instance = nullptr;

std::shared_ptr<Device> Device::getInstance() {
  if (instance == nullptr) {
    instance = std::make_shared<Device>();
  }
  return instance;
}

Device::Device() {
#ifdef ENABLE_BUTTONS
  instance->button1 = std::make_shared<Button>(PIN_BUTTON_1);
  instance->button2 = std::make_shared<Button>(PIN_BUTTON_2);
  instance->button3 = std::make_shared<Button>(PIN_BUTTON_3);
  instance->button4 = std::make_shared<Button>(PIN_BUTTON_4);
#endif
#ifdef ENABLE_JOYSTICK
  instance->joystick = std::make_shared<Joystick>(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_BUTTON, 0);
#endif
}
