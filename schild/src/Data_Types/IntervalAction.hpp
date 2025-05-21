//
// Created by amentler on 24.04.2025.
//

#pragma once

#include <Arduino.h>
#include <functional>

class IntervalAction {
private:
    unsigned long previousMillis;
    unsigned long interval;
    std::function<void()> action;

public:
    IntervalAction(unsigned long intervalMs, std::function<void()> callback);

    void update();
    void setInterval(unsigned long newInterval);
};
