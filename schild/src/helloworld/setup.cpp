#include <Arduino.h>
#include "components/DS18B20/DS18B20.h"
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

GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(
        GxEPD2_290_GDEY029T94(/*CS=*/ 0, /*DC=*/ 19, /*RST=*/ 21, /*BUSY=*/ 22));

DS18B20 temperatureSensor;

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


    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);

    temperatureSensor.init(17);
}

void loop() {

    digitalWrite(27, HIGH);
    digitalWrite(26, HIGH);

    showTemperature();
};

void showTemperature() {
    delay(1500);
    display.nextPage();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0, 0);
    display.print(temperatureSensor.getTemperature());
    display.print("C");

    Serial.println(temperatureSensor.getTemperature());
}