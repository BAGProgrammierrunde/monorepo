#include "LightupLEDs.h"
#include "features/common/Common.h"

namespace features {
  void lightupLEDs() {
    // TODO Split initialization, setup and feature implementation
#if ENABLE_LEDS
    pinMode(PIN_LED_1, OUTPUT);
    digitalWrite(PIN_LED_1, HIGH);

    pinMode(PIN_LED_2, OUTPUT);
    digitalWrite(PIN_LED_2, HIGH);
#else
    features::printComponentNotEnabledMessage("lightupLEDs", "ENABLE_LEDS");
#endif
  }
}
