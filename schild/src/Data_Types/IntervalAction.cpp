//
// Created by amentler on 24.04.2025.
//

#include "IntervalAction.hpp"

IntervalAction::IntervalAction(unsigned long intervalMs, std::function<void()> callback)
  : previousMillis(0), interval(intervalMs), action(callback)
{
}

void IntervalAction::update() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (action) {
            action();
        }
    }
}

void IntervalAction::setInterval(unsigned long newInterval) {
    interval = newInterval;
}