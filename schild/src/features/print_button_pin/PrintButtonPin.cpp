#include "PrintButtonPin.hpp"

#include "main/Utility.hpp"

#include "Data_Types/any_callable.hpp"

namespace Features {
  void printButtonPinWhenPressed(const std::shared_ptr<Button> &button) {
#if ENABLE_BUTTONS
    button->setClickFunction([button]() -> void {
        Serial.print("Button with GPIO ");
        Serial.print(button->getPin(0));
        Serial.print(" was pressed");
    });
#else
    Utility::printComponentNotEnabledMessage("printButtonPinWhenPressed", "ENABLE_BUTTONS");
#endif
  }
}
