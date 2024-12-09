#include "PrintButtonPin.h"

namespace features {
  void printButtonPinWhenPressed(const std::shared_ptr<Button> &button) {
    button->setCallback([](ulong) -> void {
    Serial.print("Button with GPIO ");
    // TODO We need captures in button callback
    Serial.print("{CURRENTLY UNKNOWN}");
    Serial.print(" was pressed");
  });
  }
}
