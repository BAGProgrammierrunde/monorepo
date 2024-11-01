
#ifndef SCHILD_BUTTONS_H
#define SCHILD_BUTTONS_H

#include "Arduino.h"

class Buttons {
public:
    void init(int buttonsPin);
    int getDigitalValue();
private:    
    int getAnalogValue();
    void storeValue();
    bool isValueCloseTo(int middle);
    int buttonsPin;
    int currentValue;
};


#endif //SCHILD_BUTTONS_H
