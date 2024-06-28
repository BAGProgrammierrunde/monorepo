#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

/*
Anschlüsse:
lila    BUSY= IO22
weiß    RST = IO21
grün    DC  = IO19
orange  CS  = IO0   
gelb    CLK = IO18  
blau    DIN = IO23  
braun   GND = GND
grau    VCC = 3.3V
*/

#define PIN_YELLOW_LED 27
#define PIN_YELLOW_LED_2 26

GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/ 0, /*DC=*/ 19, /*RST=*/ 21, /*BUSY=*/ 22));

#include <Arduino.h>
#include <SPI.h>
const char HelloWorld[] = "Hello World!";

void setup()
{
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
}

void loop() {
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);
};