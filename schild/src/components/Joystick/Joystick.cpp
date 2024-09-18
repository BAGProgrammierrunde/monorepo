//
// Created by fabian on 17.09.24.
//

#include "Arduino.h"
#include "Joystick.h"

int buttonPin;
int xAxisPin;
int yAxisPin;

int xAxisOffset;
int yAxisOffset;

/// <summary>Initializes the joystick with the default pins.<br>
/// The default pins are:<br>
/// - switch = 25<br>
/// - x = 34<br>
/// - y = 35<br>
/// <para><br>
/// Optional xOffset: The offset for the x-axis in percent <br>
/// Optional yOffset: The offset for the y-axis in percent
/// </para>
/// </summary>
void Joystick::init(int xOffset, int yOffset) {
    this->init(25, 34, 35, xOffset, yOffset);
}

/// <summary>Initializes the joystick with specified pins.<br>
/// <para><br>
/// switchPin: The pin for the button of the joystick.<br>
/// xPin: The pin for the x-axis of the joystick.<br>
/// yPin: The pin for the y-axis of the joystick.
/// Optional xOffset: The offset for the x-axis in percent <br>
/// Optional yOffset: The offset for the y-axis in percent
/// </para>
/// </summary>
void Joystick::init(int switchPin, int xPin, int yPin, int xOffset, int yOffset) {
    buttonPin = switchPin;
    xAxisPin = xPin;
    yAxisPin = yPin;

    xAxisOffset = xOffset;
    yAxisOffset = yOffset;

    setupPins();
}

/// <summary>Returns the value of the x-axis.<br>
/// Value range is: 0 - 4095
/// </summary>
int Joystick::getX() {
    return analogRead(xAxisPin);
}

/// <summary>Returns the value of the x-axis in percent.<br>
/// <para>Optional offset: the offset (+ or -) in percent to be 50% in middle.</para>
/// </summary>
int Joystick::getXPercent(int offset) {
    int value = analogRead(xAxisPin);

    return getPercent(value, (offset == 0 ? xAxisOffset : offset));
}

/// <summary>Returns the value of the y-axis.<br>
/// Value range is: 0 - 4095
/// </summary>
int Joystick::getY() {
    return analogRead(yAxisPin);
}

/// <summary>Returns the value of the y-axis in percent.<br>
/// <para>Optional offset: the offset (+ or -) in percent to be 50% in middle.</para>
/// </summary>
int Joystick::getYPercent(int offset) {
    int value = analogRead(yAxisPin);

    return getPercent(value, (offset == 0 ? yAxisOffset : offset));
}

/// <summary>Returns wether or not the button / switch is pressed.<br>
/// </summary>
bool Joystick::getSwitch() {
    return !digitalRead(buttonPin);
}

// ====================================================================================================================
//                                                 Private
// ====================================================================================================================

void Joystick::setupPins() {
    pinMode(buttonPin, INPUT);
    pinMode(xAxisPin, INPUT);
    pinMode(yAxisPin, INPUT);
}

int Joystick::getPercent(int value, int offset) {
    if (value <= 2047) {
        return map(value, 0, 2047, 0, 50 + offset);
    }else {
        return map(value, 2048, 4095, 50 + offset, 100);
    }
}
