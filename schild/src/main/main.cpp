#include <arduino.h>
#include "Configuration.hpp"
#include "features/IntervalAction.h"


void setup() {
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_POTENTIOMETER, INPUT);
    digitalWrite(PIN_BUZZER, LOW);
    Serial.begin(115200);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(PIN_LED_1, 0);
}
bool ledState = HIGH;
void toggleLED() {
    ledState = ledState == HIGH ? LOW : HIGH;
    digitalWrite(PIN_LED_2, ledState);
    Serial.println(ledState);
}
IntervalAction blinker(1000, toggleLED);

int analogValue = 0;
void printPotentiometerValue() {
    Serial.println(analogValue);
}
IntervalAction potiOutput(1000, printPotentiometerValue);

// the loop function runs over and over again forever
void loop() {
    blinker.update();
    potiOutput.update();
    analogValue = analogRead(PIN_POTENTIOMETER);
    int pwmValue = map(analogValue, 0, 4095, 10, 255);
    ledcWrite(0, pwmValue);
}

