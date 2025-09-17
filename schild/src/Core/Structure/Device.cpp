#include "Device.hpp"

Device::Device() : temperatureSensor(PIN_TEMPERATURE),
                    button1(PIN_BUTTON_1),
                    button2(PIN_BUTTON_2),
                    button3(PIN_BUTTON_3),
                    button4(PIN_BUTTON_4),
                    joystick(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_BUTTON, 0),
                    potentiometer(PIN_POTENTIOMETER),
                    led1(PIN_LED_1),
                    led2(PIN_LED_2),
                    passiveBuzzer(PIN_BUZZER),
                    display(PIN_DISPLAY_BUSY, PIN_DISPLAY_RST, PIN_DISPLAY_DC, PIN_DISPLAY_CS, PIN_DISPLAY_CLK, PIN_DISPLAY_DIN) {
    Serial.begin(BAUD_RATE);
}

Device& Device::get() {
    static Device device;
    return device;
}

void Device::update() {
    if (delayedSceneSwitchFunc.has())
    {
        delayedSceneSwitchFunc();
        delayedSceneSwitchFunc.clear();
    }
    if (scene.has()) scene.get()->update();
}
