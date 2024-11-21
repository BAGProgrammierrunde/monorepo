#include <Arduino.h>
#include "components/DS18B20/DS18B20.h"
#include "components/Joystick/Joystick.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/PassiveBuzzer/Tone.h"
#include "components/PassiveBuzzer/notes.h"
#include "components/PassiveBuzzer/PassiveBuzzer.h"
#include "components/buttons/Buttons.h"
#include "components/buttons/Button.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

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

#define PIN_YELLOW_LED 27
#define PIN_YELLOW_LED_2 26

// Wenn du eine Komponente verwenden möchtest, ersetze das 'false' mit einem 'true'
#define ENABLE_DISPLAY false
#define ENABLE_LEDS false
#define ENABLE_TEMPERATURE false
#define ENABLE_JOYSTICK false
#define ENABLE_POTENTIOMETER false
#define ENABLE_BUZZER false
#define ENABLE_BUTTONS false

void setupDisplay();
void showTemperature();
void readJoystick();
void playMelody();
void playNote();

GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(
        GxEPD2_290_GDEY029T94(/*CS=*/ 0, /*DC=*/ 19, /*RST=*/ 21, /*BUSY=*/ 22));

DS18B20 temperatureSensor;
unsigned long previousMillis = 0;
Joystick joystick;
Potentiometer potentiometer;
PassiveBuzzer passiveBuzzer;
#if ENABLE_BUTTONS
Buttons buttons;
Button button1(9);
Button button2(10);
Button button3(13);
Button button4(5);
#endif

const char HelloWorld[] = "Hello World!";

void setup() {
    Serial.begin(115000);
    while (!Serial) {}
    Serial.println("Starting...");

#if ENABLE_DISPLAY
    setupDisplay();
#endif

#if ENABLE_LEDS
    //LED 1
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    //LED 2
    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);
#endif

#if ENABLE_TEMPERATURE
    temperatureSensor.init(17);
#endif

#if ENABLE_JOYSTICK
    joystick.init(25, 34, 35, 6, 3);
#endif

#if ENABLE_POTENTIOMETER
    potentiometer.init(15);
#endif

#if ENABLE_BUZZER
    passiveBuzzer.init(2);
#endif

#if ENABLE_BUTTONS
    //D/A gewandelte buttons
    pinMode(36, INPUT);
    buttons.init(36);
#endif
}

bool played = false;
int readCounter = 0;
long maxReads = 100;
long readValueTotal = 0;

void loop() {
#if ENABLE_LEDS
    //LEDs AN
    digitalWrite(27, HIGH);
    digitalWrite(26, HIGH);
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
    /*Serial.print("analog: ");
    Serial.print(analogRead(36));
    Serial.print("  digital: ");
    Serial.print(buttons.getDigitalValue());
    Serial.println();
    delay(1000); */

    if (button1.isPressed()) {
        Serial.println("Button 1 gedrückt!");
    }

    if (button2.isPressed()) {
      Serial.println("Button 2 gedrückt!");
    }

    if (button3.isPressed()) {
       Serial.println("Button 3 gedrückt!");
    }

    if (button4.isPressed()) {
       Serial.println("Button 4 gedrückt!");
    }
#endif

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
            {NOTE_E5,  80},  {NOTE_E5, 80},  {NOTE_E5, 360}, {NOTE_E5,   0},
            {NOTE_E5,  80},  {NOTE_E5, 80},  {NOTE_E5, 360}, {NOTE_E5,   0},
            {NOTE_E5,  80},  {NOTE_G5, 80},  {NOTE_C5,  80}, {NOTE_D5,  80},
            {NOTE_E5, 760},  {NOTE_E5,  0},  {NOTE_E5,   0}, {NOTE_E5,   0},
            {NOTE_F5,  80},  {NOTE_F5, 80},  {NOTE_F5,  80}, {NOTE_F5,  80},
            {NOTE_F5,  80},  {NOTE_E5, 80},  {NOTE_E5,  80}, {NOTE_E5,  80},
            {NOTE_E5,  80},  {NOTE_D5, 80},  {NOTE_D5,  80}, {NOTE_E5,  80},  
            {NOTE_D5, 360},  {NOTE_G5,  0},  {NOTE_G5,   0}, {NOTE_G5,   0}
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
    Serial.print(joystick.getYPercent());

    Serial.print(" and X is: ");
    Serial.print(joystick.getXPercent());

    Serial.print(" and Button is: ");
    Serial.println(joystick.getSwitch() ? "ON" : "OFF");
}

int temperature = 0;

const int MILLIS_TO_SECOND = 1000;
const int MINUTE_IN_MILLIS = 60 * MILLIS_TO_SECOND;

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