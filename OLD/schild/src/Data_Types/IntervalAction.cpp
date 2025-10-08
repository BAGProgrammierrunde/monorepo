//
// Created by amentler on 24.04.2025.
//

#include "IntervalAction.hpp"

IntervalAction::IntervalAction(unsigned long intervalMs, me::any_callable<void()> callback) {
    previousMillis = 0;
    interval = intervalMs;
    action = callback;
}

void IntervalAction::setInterval(unsigned long newInterval) {
    interval = newInterval;
}

void IntervalAction::update() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        action();
    }
}
