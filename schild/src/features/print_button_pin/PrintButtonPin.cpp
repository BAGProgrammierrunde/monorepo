#include "PrintButtonPin.hpp"

namespace Features {
    void printButtonPinWhenPressed(const std::shared_ptr<Button> &button) {
        button->setCallback([](Button* pThisRef) -> void {
            Serial.print("Button with GPIO ");
            Serial.print("{CURRENTLY UNKNOWN}");
            Serial.print(" was pressed");
        });
    }
}
