#include "TestScene.hpp"

#include "Core/Structure/Device.hpp"

void TestScene::toggleLED1() {
    ledState = ledState == HIGH ? LOW : HIGH;
        digitalWrite(PIN_LED_1, ledState);
        Serial.println(ledState);
}
void TestScene::toggleLED2() {
    ledState2 = ledState2 == HIGH ? LOW : HIGH;
    digitalWrite(PIN_LED_2, ledState2);
    Serial.println(ledState2);
}
void TestScene::printPotentiometerValue() {
    Serial.println(analogValue);
}

TestScene::TestScene() : Scene() {
    blinker1 = IntervalAction(1000, &TestScene::toggleLED1);
    blinker2 = IntervalAction(1000, &TestScene::toggleLED2);
    potiOutput = IntervalAction(1000, &TestScene::printPotentiometerValue);

    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_POTENTIOMETER, INPUT);
    analogValue = analogRead(PIN_POTENTIOMETER);
    digitalWrite(PIN_BUZZER, LOW);

    if (analogValue > 0)
    {
        POTI_DEBUG = true;
        ledcSetup(0, 5000, 8);
        ledcAttachPin(PIN_LED_1, 0);
        ledcSetup(1, 5000, 8);
        ledcAttachPin(PIN_LED_2, 1);
    }
    else
    {
        pinMode(PIN_LED_1, OUTPUT);
        pinMode(PIN_LED_2, OUTPUT);
    }
}

void TestScene::update() {
    if (POTI_DEBUG)
    {
        potiOutput.update();
        analogValue = analogRead(PIN_POTENTIOMETER);

        int pwmValue = 0;
        if (analogValue > 0) pwmValue = map(analogValue, 0, 4095, 10, 255);

        ledcWrite(0, pwmValue);
        ledcWrite(1, pwmValue);
    }
    else
    {
        blinker1.update();
        blinker2.update();
    }

    ////////////////

    if (Device::get().button1.isPressed()) Device::get().setScene<MainScene>();
}