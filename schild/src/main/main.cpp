#include <Arduino.h>

#include "Scenes/TestScene.hpp"
#include "Core/Structure/Device.hpp"

////////////////////////////////////////////////////////////////////
//POTI&LED DEBUG MODE: poti auf > 0 beim start und dann hoch runter drehen
//led debug mode -> poti auf 0 beim start und dann blinken die LEDs
#include "Configuration.hpp"
#include "Data_Types/IntervalAction.hpp"

int analogValue = 0;
bool POTI_DEBUG = false;
bool ledState = HIGH;
bool ledState2 = HIGH;

void toggleLED1() {
    ledState = ledState == HIGH ? LOW : HIGH;
    digitalWrite(PIN_LED_1, ledState);
    Serial.println(ledState);
}

void toggleLED2() {
    ledState2 = ledState2 == HIGH ? LOW : HIGH;
    digitalWrite(PIN_LED_2, ledState2);
    Serial.println(ledState2);
}

void printPotentiometerValue() {
    Serial.println(analogValue);
}

IntervalAction blinker1(1000, toggleLED1);
IntervalAction blinker2(1000, toggleLED2);
IntervalAction potiOutput(1000, printPotentiometerValue);
////////////////////////////////////////////////////////////////////


void setup() {
    Serial.begin(115200);
    Device::getInstance().scene.set(TestScene("HI :O"));

    ////////////////////////////////////////////////////////////////
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
    ////////////////////////////////////////////////////////////////
}

void loop() {
    Device::getInstance().update();

    ////////////////////////////////////////////////////////////////
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
    ////////////////////////////////////////////////////////////////
}
