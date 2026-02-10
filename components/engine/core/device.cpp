#include "Device.h"

Device::Device() : m_ButtonAPressed(Button(GPIO_NUM_13)), m_ButtonBPressed(Button(GPIO_NUM_14)) {
}

Display& Device::getDisplay() {
    return m_Display;
}

bool Device::isButtonAPressed() {
    return m_ButtonAPressed.isPressed();
}

bool Device::isButtonBPressed() {
    return m_ButtonBPressed.isPressed();
}
