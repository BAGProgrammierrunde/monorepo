#include <Arduino.h>
#include "components/DS18B20/DS18B20.h"
#include "components/Joystick/Joystick.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/PassiveBuzzer/Tone.h"
#include "components/PassiveBuzzer/notes.h"
#include "components/PassiveBuzzer/PassiveBuzzer.h"
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

const char HelloWorld[] = "Hello World!";

void setup() {
    Serial.begin(115000);
    while (!Serial);
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


    //LED 1
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    //LED 2
    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);

    //da gewandelte buttons
    pinMode(36, INPUT);

    temperatureSensor.init(17);
    joystick.init(25, 34, 35, 6, 3);
    potentiometer.init(15);
    passiveBuzzer.init(2);
}

bool played = false;


int readCounter = 0;
long maxReads = 100;
long readValueTotal = 0;
void loop() {
    
    //LEDs AN
    //digitalWrite(27, HIGH);
    //digitalWrite(26, HIGH);

    Serial.println(potentiometer.getValue());
    
    readValueTotal = readValueTotal + analogRead(36);
    readCounter = readCounter + 1;
    if (readCounter == maxReads) {
        Serial.println(readValueTotal / maxReads);
        readCounter = 0;
        readValueTotal = 0;
    }

    showTemperature();

    readJoystick();
    /*if (!played) {
        playMelody();
        played = true;
    }*/

    //playNote();
};

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

void showTemperature() {
    int currentTemperature = temperatureSensor.getTemperature();
    if (currentTemperature != temperature) {        
        temperature = currentTemperature;
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= 1300) {
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