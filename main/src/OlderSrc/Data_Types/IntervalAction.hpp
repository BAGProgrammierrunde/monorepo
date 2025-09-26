//
// Created by amentler on 24.04.2025.
//

#pragma once

#include <Arduino.h>
#include "Data_Types/any_callable.hpp"

class IntervalAction {
private:
    unsigned long previousMillis;
    unsigned long interval;
    me::any_callable<void()> action;

public:
    IntervalAction(unsigned long intervalMs = 1000, me::any_callable<void()> callback = me::any_callable<void()>());
    void setInterval(unsigned long newInterval);
    void update();
};
