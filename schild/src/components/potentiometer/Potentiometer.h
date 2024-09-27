//
// Created by fabian on 17.09.24.
//

#ifndef SCHILD_POTENTIOMETER_H
#define SCHILD_POTENTIOMETER_H

#include "Arduino.h"

class Potentiometer {
public:
    void init(int potentiometerPin);
    int getValue();
private:    
    int potentiometerPin;
};


#endif //SCHILD_POTENTIOMETER_H
