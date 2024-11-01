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
    if (isValueCloseTo(650)) return 3;
    if (isValueCloseTo(880)) return 4;
    if (isValueCloseTo(1160)) return 5;
    if (isValueCloseTo(1400)) return 6;
    if (isValueCloseTo(1660)) return 7;
    if (isValueCloseTo(1850)) return 8;
    if (isValueCloseTo(2110)) return 9;
    if (isValueCloseTo(2350)) return 10;
    if (isValueCloseTo(2600)) return 11;
    if (isValueCloseTo(2830)) return 12;
    if (isValueCloseTo(3120)) return 13;
    if (isValueCloseTo(3450)) return 14;
    if (isValueCloseTo(3850)) return 15;

    return 0;
}

bool Buttons::isValueCloseTo(int middle) {
    int range = 100; 
    return (middle - range < this->currentValue && this->currentValue < middle + range);
}

void Buttons::storeValue() {
    this->currentValue = getAnalogValue();
}
