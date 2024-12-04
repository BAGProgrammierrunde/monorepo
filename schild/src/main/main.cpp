#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <driver/rtc_io.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "components/Structure/Component.h"
#include "components/Button/Button.h"
#include "components/Joystick/Joystick.h"
//#include "components/Structure/Device.h"

#include "components/DS18B20/DS18B20.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/PassiveBuzzer/Tone.h"
#include "components/PassiveBuzzer/notes.h"
#include "components/PassiveBuzzer/PassiveBuzzer.h"

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
        default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
    }
}

/*
 * |          Anschlüsse           |
 * | ----------------------------- |
 * | Farbe  | Display | Firebeetle |
 * | ------ | ------- | ---------- |
 * | lila   | BUSY    | IO-22      |
 * | weiß   | RST     | IO-21      |
 * | grün   | DC      | IO-19      |
 * | orange | CS      | IO- 0      |
 * | gelb   | CLK     | IO-18      |
 * | blau   | DIN     | IO-23      |
 * | braun  | GND     | GND        |
 * | grau   | VCC     | 3.3V       |
 * | ------ |-------- | ---------- |
 */

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

#define PIN_LED_1 27
#define PIN_LED_2 26

#define PIN_BUTTON_1 9
#define PIN_BUTTON_2 10
#define PIN_BUTTON_3 13
#define PIN_BUTTON_4 5

#define PIN_WAKE_UP_BUTTON GPIO_NUM_39

// Wenn du eine Komponente verwenden möchtest, ersetze das 'false' mit einem 'true'
#define ENABLE_DISPLAY false
#define ENABLE_LEDS false
#define ENABLE_TEMPERATURE false
#define ENABLE_JOYSTICK false
#define ENABLE_POTENTIOMETER false
#define ENABLE_BUZZER false
#define ENABLE_BUTTONS false

// Lange nicht an ihrer finalen Position Komponenten funktionstest-Funktionen
void setupDisplay();
void showTemperature();
void readJoystick();
void playMelody();
void playNote();

// GLOBALS:
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(
    GxEPD2_290_GDEY029T94(0, 19, 21, 22)); // CS, DC, RST, BUSY
DS18B20 temperatureSensor;
Joystick joystick(25, 34, 35, 0);
Potentiometer potentiometer;
PassiveBuzzer passiveBuzzer;
#if ENABLE_BUTTONS
Button button1(PIN_BUTTON_1);
Button button2(PIN_BUTTON_2);
Button button3(PIN_BUTTON_3);
Button button4(PIN_BUTTON_4);
#endif

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
    Serial.println("Build with C++ version: " + String(__cplusplus));
    print_wakeup_reason();

#if ENABLE_DISPLAY
    setupDisplay();
#endif

#if ENABLE_LEDS
    //LED 1
    pinMode(PIN_LED_1, OUTPUT);
    digitalWrite(PIN_LED_1, HIGH);

    //LED 2
    pinMode(PIN_LED_2, OUTPUT);
    digitalWrite(PIN_LED_2, HIGH);
#endif

#if ENABLE_TEMPERATURE
    temperatureSensor.init(17);
#endif

#if ENABLE_POTENTIOMETER
    potentiometer.init(15);
#endif

#if ENABLE_BUZZER
    passiveBuzzer.init(2);
#endif

#if ENABLE_BUTTONS
    button1.init([](ulong) -> void {
        Serial.println("Button 1 clicked");
        esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(PIN_WAKE_UP_BUTTON),ESP_EXT1_WAKEUP_ANY_HIGH);
        Serial.print("Sleep countdown triggered - sleeping in 2 seconds");
        for (int i = 0; i < 50; i++) {
            Serial.print(".");
            delay(20);
        }
        Serial.println();
        Serial.println("Sleeping now");
        Serial.flush();
        esp_deep_sleep_start();
    });
    button2.init([](ulong) -> void {
        Serial.println("Button 2 clicked");
        String currentMenu = "main_menu";

        if (currentMenu == "main_menu") {
            // do function related to main menu
        } else if (currentMenu == "settings") {
            // do function related to settings
        } else if (currentMenu == "show_data") {
            // do function related to show data
        }
    });
    button3.init([](ulong) -> void {
        Serial.println("Button 3 clicked");
    });
    button4.init([](ulong) -> void {
        Serial.println("Button 4 clicked");
    });
#endif
}

void loop() {
#if ENABLE_LEDS
    //LEDs AN
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
#endif

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

#if ENABLE_BUTTONS

#endif

    Serial.println("loop");
    delay(200);
}

void setupDisplay() {
    Serial.println("GxEPD2 2.9-inch  e-ink display test start1");

    display.init(115200, true, 2, true); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    Serial.println("Display initialized");

    display.setRotation(1);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
    display.firstPage();
    Serial.println("Starting display update");
    do {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.print(random(10000000));
    } while (display.nextPage());
    Serial.println("Display update done");
}


void playMelody() {
    Tone tones[] = {
        {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 360}, {NOTE_E5, 0},
        {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 360}, {NOTE_E5, 0},
        {NOTE_E5, 80}, {NOTE_G5, 80}, {NOTE_C5, 80}, {NOTE_D5, 80},
        {NOTE_E5, 760}, {NOTE_E5, 0}, {NOTE_E5, 0}, {NOTE_E5, 0},
        {NOTE_F5, 80}, {NOTE_F5, 80}, {NOTE_F5, 80}, {NOTE_F5, 80},
        {NOTE_F5, 80}, {NOTE_E5, 80}, {NOTE_E5, 80}, {NOTE_E5, 80},
        {NOTE_E5, 80}, {NOTE_D5, 80}, {NOTE_D5, 80}, {NOTE_E5, 80},
        {NOTE_D5, 360}, {NOTE_G5, 0}, {NOTE_G5, 0}, {NOTE_G5, 0}
    };

    passiveBuzzer.playNotes(tones, 31, 200);

    delay(3000);
}

void playNote() {
    passiveBuzzer.playNote(NOTE_F3, 160);
    delay(1000);
    passiveBuzzer.playNote(NOTE_C5);
    delay(500);
    passiveBuzzer.stopNote();
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
