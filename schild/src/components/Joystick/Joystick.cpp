//
// Created by fabian on 17.09.24.
//

#include "Arduino.h"
#include "Joystick.h"

int buttonPin;
int xAxisPin;
int yAxisPin;

void Joystick::init() {
    this->init(25, 34, 35);
}

void Joystick::init(int switchPin, int xPin, int yPin) {
    buttonPin = switchPin;
    xAxisPin = xPin;
    yAxisPin = yPin;

    setupPins();
}

int Joystick::getX() {
    return analogRead(xAxisPin);
}

int Joystick::getXPercent() {
    return map(analogRead(xAxisPin), 0, 4095, 0, 100);;
}

int Joystick::getY() {
    return analogRead(yAxisPin);
}

int Joystick::getYPercent() {
    return map(analogRead(yAxisPin), 0, 4095, 0, 100);
}

int Joystick::getSwitch() {
    return analogRead(buttonPin);
}

void Joystick::setupPins() {
    pinMode(buttonPin, INPUT);
    pinMode(xAxisPin, INPUT);
    pinMode(yAxisPin, INPUT);
}
