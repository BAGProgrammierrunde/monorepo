#include "Buttons.h"

void Buttons::init(int buttonsPin) { 
    pinMode(buttonsPin, INPUT);
    this->buttonsPin = buttonsPin;
}

int Buttons::getAnalogValue() {
    return analogRead(buttonsPin);
}

int Buttons::getDigitalValue() {
    storeValue();

    if (isValueCloseTo(130)) return 1;
    if (isValueCloseTo(380)) return 2;
    if (isValueCloseTo(620)) return 3;
    if (isValueCloseTo(880)) return 4;
    if (isValueCloseTo(1130)) return 5;
    if (isValueCloseTo(1380)) return 6;
    if (isValueCloseTo(1600)) return 7;
    if (isValueCloseTo(1850)) return 8;
    if (isValueCloseTo(2100)) return 9;
    if (isValueCloseTo(2300)) return 10;
    if (isValueCloseTo(2470)) return 11;
    if (isValueCloseTo(2750)) return 12;
    if (isValueCloseTo(2950)) return 13;
    if (isValueCloseTo(3250)) return 14;
    if (isValueCloseTo(3410)) return 15;

    return 0;
}

bool Buttons::isValueCloseTo(int middle) {
    int range = 80; 
    return (middle - range < this->currentValue && this->currentValue < middle + range);
}

void Buttons::storeValue() {
    this->currentValue = getAnalogValue();
}
