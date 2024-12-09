#pragma once

#include <Arduino.h>

namespace Utility {
    void printComponentNotEnabledMessage(String functionName, String defineName);
    void printWakeupReason();
}
