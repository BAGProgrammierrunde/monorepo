#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <driver/rtc_io.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "configuration.h"
#include "components/Structure/Component.h"
#include "components/Button/Button.h"
#include "components/Joystick/Joystick.h"
#include <components/Structure/Device.h>

#include "components/DS18B20/DS18B20.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/PassiveBuzzer/Tone.h"
#include "components/PassiveBuzzer/notes.h"
#include "components/PassiveBuzzer/PassiveBuzzer.h"
#include "features/common/Common.h"
#include "features/deep_sleep/DeepSleep.h"
#include "features/lightup_leds/LightupLEDs.h"
#include "features/melody/Melody.h"
#include "features/print_button_pin/PrintButtonPin.h"
#include "features/test_display/TestDisplay.h"

// Lange nicht an ihrer finalen Position Komponenten funktionstest-Funktionen
void setupDisplay();
void showTemperature();
void readJoystick();
void playMelody();
void playNote();

// GLOBALS:
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(
    GxEPD2_290_GDEY029T94(PIN_DISPLAY_CS, PIN_DISPLAY_DC, PIN_DISPLAY_RST, PIN_DISPLAY_BUSY));
DS18B20 temperatureSensor;
Joystick joystick(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_BUTTON, 0);
Potentiometer potentiometer;
PassiveBuzzer passiveBuzzer;
unsigned long previousMillis = 0;
const char HelloWorld[] = "Hello World!";

bool played = false;
int readCounter = 0;
long maxReads = 100;
long readValueTotal = 0;

int temperature = 0;

const int MILLIS_TO_SECOND = 1000;
const int MINUTE_IN_MILLIS = 60 * MILLIS_TO_SECOND;

void setup() {
    Serial.begin(115000);
    delay(100);
    while (!Serial) {}


    Device::getInstance(); // -> Works

    features::printStartMessage();

    const auto device = Device::getInstance();

    features::useButtonForDeepSleep(device->button1, device->button4);
    features::lightupLEDs();
    features::printButtonPinWhenPressed(device->button2);
    features::printButtonPinWhenPressed(device->button3);
}

void loop() {
#if ENABLE_TEMPERATURE
    showTemperature();
#endif

#if ENABLE_POTENTIOMETER
    Serial.println(potentiometer.getValue());
#endif

#if ENABLE_JOYSTICK
    readJoystick();
#endif

#if ENABLE_BUZZER
    if (!played) {
        playMelody();
        played = true;

        playNote();
    }
#endif

    Serial.print(".");
    delay(500);
}



// Um schnellere Ergebnisse zu erhalten muss der Rest auskommentiert werden!
void readJoystick() {
    Serial.print("Y is: ");
    Serial.print(joystick.getY());

    Serial.print(" and X is: ");
    Serial.print(joystick.getX());

    Serial.print(" and Button is: ");
    Serial.println(joystick.button.isPressed() ? "ON" : "OFF");
}

void showTemperature() {
    float currentTemperature = temperatureSensor.getTemperature();
    int currentTemperatureInt = round(currentTemperature);

    if (currentTemperatureInt != temperature) {
        temperature = currentTemperatureInt;
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= MINUTE_IN_MILLIS) {
            previousMillis = currentMillis;
            display.nextPage();
            display.fillScreen(GxEPD_WHITE);
            display.setCursor(0, 0);
            display.print(currentTemperature);
            display.print("C");

            Serial.println(currentTemperature);
        }
    }
}
