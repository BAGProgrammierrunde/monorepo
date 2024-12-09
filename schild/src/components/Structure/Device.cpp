#include "Device.hpp"
#include "main/Configuration.hpp"

std::shared_ptr<Device> Device::instance = nullptr;

std::shared_ptr<Device> Device::getInstance() {
  if (instance == nullptr) {
    instance = std::make_shared<Device>();
  }
  return instance;
}

Device::Device() {
  printEnabledMessage("DISPLAY", ENABLE_DISPLAY);
#ifdef ENABLE_DISPLAY
  instance->display = std::make_shared<Display>(PIN_DISPLAY_BUSY, PIN_DISPLAY_RST, PIN_DISPLAY_DC, PIN_DISPLAY_CS, PIN_DISPLAY_CLK, PIN_DISPLAY_DIN);
#endif

  // TODO led1
  // TODO led2

  // TODO temperature

  printEnabledMessage("JOYSTICK", ENABLE_JOYSTICK);
#ifdef ENABLE_JOYSTICK
  instance->joystick = std::make_shared<Joystick>(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_BUTTON, 0);
#endif

  printEnabledMessage("POTENTIOMETER", ENABLE_POTENTIOMETER);
#ifdef ENABLE_POTENTIOMETER
  instance->potentiometer = std::make_shared<Potentiometer>(PIN_POTENTIOMETER);
#endif

  printEnabledMessage("BUZZER", ENABLE_BUZZER);
#ifdef ENABLE_BUZZER
  instance->passiveBuzzer = std::make_shared<PassiveBuzzer>(PIN_BUZZER);
#endif

  printEnabledMessage("BUTTONS", ENABLE_BUTTONS);
#ifdef ENABLE_BUTTONS
  instance->button1 = std::make_shared<Button>(PIN_BUTTON_1);
  instance->button2 = std::make_shared<Button>(PIN_BUTTON_2);
  instance->button3 = std::make_shared<Button>(PIN_BUTTON_3);
  instance->button4 = std::make_shared<Button>(PIN_BUTTON_4);
#endif
}

void Device::printEnabledMessage(const String &name, const bool &enableDefine) {
  Serial.println(name + " is " + (enableDefine ? "ENABLED": "DISABLED"));
}
